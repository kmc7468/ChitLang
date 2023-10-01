#include <chit/Parser.hpp>

#include <chit/Generator.hpp>
#include <chit/Type.hpp>
#include <chit/ast/Declaration.hpp>
#include <chit/ast/Expression.hpp>
#include <chit/ast/Statement.hpp>
#include <chit/ast/Type.hpp>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <utility>

namespace chit {
	Parser::Parser(std::span<const Token> tokens) noexcept
		: m_Tokens(tokens) {
		m_Current = m_Tokens.begin();
	}

	void Parser::Parse() {
		assert(m_Current == m_Tokens.begin());
		assert(m_RootContext == nullptr);
		assert(m_RootNode == nullptr);

		m_RootNode = std::make_unique<RootNode>();

		while (m_Current < m_Tokens.end()) {
			if (auto statement = ParseStatement(); statement) {
				m_RootNode->Statements.push_back(std::move(statement));
			} else {
				break;
			}
		}

		m_RootContext = std::unique_ptr<ParserContext>(new ParserContext{
			.Messages = m_Messages,
		});

		m_RootNode->Analyze(*m_RootContext);
	}
	const RootNode* Parser::GetRootNode() const noexcept {
		return m_RootNode.get();
	}
	std::span<const Message> Parser::GetMessages() const noexcept {
		return m_Messages;
	}

	const Token* Parser::AcceptToken(TokenType tokenType) noexcept {
		if (m_Current->Type == tokenType) return &*m_Current++;
		else return nullptr;
	}
	const Token& Parser::PrevToken() const noexcept {
		return *(m_Current - 1);
	}

#define ACCEPT(n, t) const auto n = AcceptToken(t); n

	std::unique_ptr<TypeNode> Parser::ParseType() {
		if (ACCEPT(voidToken, TokenType::Void)) {
			return std::unique_ptr<TypeNode>(new IdentifierTypeNode(voidToken->Data));
		} else {
			return ParseBuiltinType();
		}
	}

	std::unique_ptr<TypeNode> Parser::ParseBuiltinType() {
		std::vector<std::u8string_view> names;

		while (true) {
			switch (m_Current->Type) {
			case TokenType::Int:
			case TokenType::Long:
			case TokenType::Signed:
			case TokenType::Unsigned:
				names.push_back((m_Current++)->Data);

				break;

			default:
				goto generateNode;
			}
		}

	generateNode:
		if (!names.empty()) {
			return std::unique_ptr<TypeNode>(new IdentifierTypeNode(std::move(names)));
		} else {
			return nullptr;
		}
	}

	std::unique_ptr<ExpressionNode> Parser::ParseExpression() {
		return ParseAssignment();
	}
	std::unique_ptr<ExpressionNode> Parser::ParseAssignment() {
		auto leftNode = ParseAddition();
		if (!leftNode)
			return nullptr;

		if (AcceptToken(TokenType::Assignment)) {
			if (auto rightNode = ParseAssignment(); rightNode) {
				return std::unique_ptr<ExpressionNode>(new BinaryOperatorNode(
					TokenType::Assignment,
					std::move(leftNode),
					std::move(rightNode)
				));
			} else {
				return nullptr;
			}
		} else {
			return leftNode;
		}
	}
	std::unique_ptr<ExpressionNode> Parser::ParseAddition(
		std::unique_ptr<ExpressionNode> leftNode) {

		if (!leftNode &&
			!(leftNode = ParseMultiplication())) {

			return nullptr;
		}

		if (AcceptToken(TokenType::Addition) || AcceptToken(TokenType::Subtraction)) {
			const auto prevTokenType = PrevToken().Type;

			if (auto rightNode = ParseMultiplication(); rightNode) {
				return ParseAddition(std::unique_ptr<ExpressionNode>(new BinaryOperatorNode(
					prevTokenType,
					std::move(leftNode),
					std::move(rightNode)
				)));
			} else {
				return nullptr;
			}
		} else {
			return leftNode;
		}
	}
	std::unique_ptr<ExpressionNode> Parser::ParseMultiplication(
		std::unique_ptr<ExpressionNode> leftNode) {

		if (!leftNode &&
			!(leftNode = ParseFunctionCall())) {

			return nullptr;
		}

		if (AcceptToken(TokenType::Multiplication)		||
			AcceptToken(TokenType::Division)			||
			AcceptToken(TokenType::Modulo)) {

			const auto prevTokenType = PrevToken().Type;

			if (auto rightNode = ParseFunctionCall(); rightNode) {
				return ParseMultiplication(std::unique_ptr<ExpressionNode>(new BinaryOperatorNode(
					prevTokenType,
					std::move(leftNode),
					std::move(rightNode)
				)));
			} else {
				return nullptr;
			}
		} else {
			return leftNode;
		}
	}
	std::unique_ptr<ExpressionNode> Parser::ParseFunctionCall() {
		auto functionNode = ParseSimpleExpression();
		if (!functionNode)
			return nullptr;

		if (AcceptToken(TokenType::LeftParenthesis)) {
			std::vector<std::unique_ptr<ExpressionNode>> arguments;

			while (!AcceptToken(TokenType::RightParenthesis)) {
				if (!arguments.empty() && !AcceptToken(TokenType::Comma)) {
					m_Messages.push_back({
						.Type = MessageType::Error,
						.Data = u8"Expected ','",
						.Line = m_Current->Line,
						.Column = m_Current->Column,
					});

					return nullptr;
				}

				arguments.push_back(ParseExpression());
			}

			return std::unique_ptr<ExpressionNode>(new FunctionCallNode(
				std::move(functionNode),
				std::move(arguments)
			));
		} else {
			return functionNode;
		}
	}
	std::unique_ptr<ExpressionNode> Parser::ParseSimpleExpression() {
		if (ACCEPT(nameToken, TokenType::Identifier)) {
			return std::unique_ptr<ExpressionNode>(new IdentifierNode(nameToken->Data));
		} else if (ACCEPT(integerToken, TokenType::DecInteger)) {
			return ParseInteger(integerToken);
		} else {
			return nullptr;
		}
	}

	std::unique_ptr<ExpressionNode> Parser::ParseInteger(
		const Token* integerToken) {

		bool isUnsigned = false, isLong = false, isLongLong = false;

		if (!ParseIntegerSuffix(integerToken, isUnsigned, isLong, isLongLong))
			return nullptr;

		unsigned long long value;
		TypePtr type;

		try {
			value = std::stoull(std::string(
				integerToken->Data.begin(), integerToken->Data.end()));
		} catch (...) {
			m_Messages.push_back({
				.Type = MessageType::Error,
				.Data = u8"Too large integer constant",
				.Line = integerToken->Line,
				.Column = integerToken->Column,
			});

			return nullptr;
		}

		if (value > 9223372036854775807) {							// 2^63 - 1: unsigned long long
			if (isUnsigned) {
				type = BuiltinType::UnsignedLongLongInt;
			}
		} else if (value > 4294967295) {							// 2^32 - 1: long long
			if (isUnsigned) {
				type = BuiltinType::UnsignedLongLongInt;
			} else {
				type = BuiltinType::LongLongInt;
			}
		} else if (value > 2147483648) {							// 2^31 - 1: unsigned long, unsigned int
			if (isUnsigned) {
				if (isLongLong) {
					type = BuiltinType::UnsignedLongLongInt;
				} else if (isLong) {
					type = BuiltinType::UnsignedLongInt;
				} else {
					type = BuiltinType::UnsignedInt;
				}
			} else {
				type = BuiltinType::LongLongInt;
			}
		} else {
			if (isUnsigned) {
				if (isLongLong) {
					type = BuiltinType::UnsignedLongLongInt;
				} else if (isLong) {
					type = BuiltinType::UnsignedLongInt;
				} else {
					type = BuiltinType::UnsignedInt;
				}
			} else {
				if (isLongLong) {
					type = BuiltinType::LongLongInt;
				} else if (isLong) {
					type = BuiltinType::LongInt;
				} else {
					type = BuiltinType::Int;
				}
			}
		}

		if (type == BuiltinType::Int) {
			return std::unique_ptr<ExpressionNode>(new IntConstantNode(
				static_cast<std::int32_t>(value)
			));
		} else if (type == BuiltinType::UnsignedInt) {
			return std::unique_ptr<ExpressionNode>(new UnsignedIntConstantNode(
				static_cast<std::uint32_t>(value)
			));
		} else if (type == BuiltinType::LongInt) {
			return std::unique_ptr<ExpressionNode>(new LongIntConstantNode(
				static_cast<std::int32_t>(value)
			));
		} else if (type == BuiltinType::UnsignedLongInt) {
			return std::unique_ptr<ExpressionNode>(new UnsignedLongIntConstantNode(
				static_cast<std::uint32_t>(value)
			));
		} else if (type == BuiltinType::LongLongInt) {
			return std::unique_ptr<ExpressionNode>(new LongLongIntConstantNode(
				static_cast<std::int64_t>(value)
			));
		} else if (type == BuiltinType::UnsignedLongLongInt) {
			return std::unique_ptr<ExpressionNode>(new UnsignedLongLongIntConstantNode(
				static_cast<std::uint64_t>(value)
			));
		} else {
			m_Messages.push_back({
				.Type = MessageType::Error,
				.Data = u8"Too large integer constant",
				.Line = integerToken->Line,
				.Column = integerToken->Column,
			});

			return nullptr;
		}
	}
	bool Parser::ParseIntegerSuffix(
		const Token* integerToken,
		bool& isUnsigned, bool& isLong, bool& isLongLong) {

		for (std::size_t i = 0; i < integerToken->Suffix.size(); ++i) {
			if (integerToken->Suffix[i] == u8'u' ||
				integerToken->Suffix[i] == u8'U') {

				if (isUnsigned) {
					m_Messages.push_back({
						.Type = MessageType::Error,
						.Data = u8"Duplicated unsigned-suffix",
						.Line = integerToken->Line,
						.Column = integerToken->Column,
					});

					return false;
				} else {
					isUnsigned = true;
				}
			} else if (
				integerToken->Suffix[i] == u8'l' ||
				integerToken->Suffix[i] == u8'L') {

				if (isLong || isLongLong) {
					m_Messages.push_back({
						.Type = MessageType::Error,
						.Data = u8"Duplicated long-suffix",
						.Line = integerToken->Line,
						.Column = integerToken->Column,
					});

					return false;
				} else if (i + 1 != integerToken->Suffix.size() &&
					integerToken->Suffix[i + 1] == integerToken->Suffix[i]) {

					isLongLong = true;
					++i;
				} else {
					isLong = true;
				}
			} else {
				m_Messages.push_back({
					.Type = MessageType::Error,
					.Data = u8"Invalid suffix",
					.Line = integerToken->Line,
					.Column = integerToken->Column,
				});

				return false;
			}
		}

		return true;
	}

	std::unique_ptr<StatementNode> Parser::ParseStatement() {
		if (AcceptToken(TokenType::Semicolon)) {
			return std::unique_ptr<StatementNode>(new EmptyStatementNode());
		} else if (AcceptToken(TokenType::Return)) {
			return ParseReturn();
		} else if (auto typeNode = ParseType(); typeNode) {
			const auto nameToken = AcceptToken(TokenType::Identifier);
			if (!nameToken) {
				m_Messages.push_back({
					.Type = MessageType::Error,
					.Data = u8"Expected identifier",
					.Line = m_Current->Line,
					.Column = m_Current->Column,
				});

				return nullptr;
			}

			if (AcceptToken(TokenType::LeftParenthesis)) {
				return ParseFunctionDeclaration(std::move(typeNode), nameToken);
			} else {
				return ParseVariableDeclaration(std::move(typeNode), nameToken);
			}
		} else if (auto exprNode = ParseExpression(); exprNode) {
			if (!AcceptToken(TokenType::Semicolon)) {
				m_Messages.push_back({
					.Type = MessageType::Error,
					.Data = u8"Expected ';'",
					.Line = m_Current->Line,
					.Column = m_Current->Column,
				});

				return nullptr;
			}

			return std::unique_ptr<StatementNode>(new ExpressionStatementNode(
				std::move(exprNode)
			));
		} else {
			return nullptr;
		}
	}
	std::unique_ptr<StatementNode> chit::Parser::ParseReturn() {
		auto exprNode = ParseExpression();
		if (!exprNode) {
			m_Messages.push_back({
				.Type = MessageType::Error,
				.Data = u8"Expected expression",
				.Line = m_Current->Line,
				.Column = m_Current->Column,
			});

			return nullptr;
		}

		if (!AcceptToken(TokenType::Semicolon)) {
			m_Messages.push_back({
				.Type = MessageType::Error,
				.Data = u8"Expected ';'",
				.Line = m_Current->Line,
				.Column = m_Current->Column,
			});

			return nullptr;
		}

		return std::unique_ptr<StatementNode>(new ReturnNode(
			std::move(exprNode)
		));
	}
	std::unique_ptr<StatementNode> Parser::ParseFunctionDeclaration(
		std::unique_ptr<TypeNode> returnTypeNode,
		const Token* nameToken) {

		std::vector<std::pair<
			std::u8string_view,
			std::unique_ptr<TypeNode>>> parameters;

		while (!AcceptToken(TokenType::RightParenthesis)) {
			if (!parameters.empty() && !AcceptToken(TokenType::Comma)) {
				m_Messages.push_back({
					.Type = MessageType::Error,
					.Data = u8"Expected ','",
					.Line = m_Current->Line,
					.Column = m_Current->Column,
				});

				return nullptr;
			}

			auto paramTypeNode = ParseType();
			if (!paramTypeNode) {
				m_Messages.push_back({
					.Type = MessageType::Error,
					.Data = u8"Expected type",
					.Line = m_Current->Line,
					.Column = m_Current->Column,
				});

				return nullptr;
			}

			if (ACCEPT(paramNameToken, TokenType::Identifier)) {
				parameters.push_back({ paramNameToken->Data, std::move(paramTypeNode) });
			} else {
				parameters.push_back({ std::u8string_view{}, std::move(paramTypeNode) });
			}
		}

		std::unique_ptr<FunctionDeclarationNode> funcDeclNode(new FunctionDeclarationNode(
			std::move(returnTypeNode),
			nameToken->Data,
			std::move(parameters)
		));

		if (AcceptToken(TokenType::Semicolon)) {
			return std::unique_ptr<StatementNode>(funcDeclNode.release());
		} else if (auto blockNode = ParseBlock(); blockNode) {
			return std::unique_ptr<StatementNode>(new FunctionDefinitionNode(
				std::move(funcDeclNode),
				std::move(blockNode)
			));
		} else {
			m_Messages.push_back({
				.Type = MessageType::Error,
				.Data = u8"Expected ';'",
				.Line = m_Current->Line,
				.Column = m_Current->Column,
			});

			return nullptr;
		}
	}
	std::unique_ptr<StatementNode> Parser::ParseVariableDeclaration(
		std::unique_ptr<TypeNode> typeNode,
		const Token* nameToken) {

		if (AcceptToken(TokenType::Semicolon)) {
			return std::unique_ptr<StatementNode>(new VariableDeclarationNode(
				std::move(typeNode),
				nameToken->Data
			));
		} else if (!AcceptToken(TokenType::Assignment)) {
			m_Messages.push_back({
				.Type = MessageType::Error,
				.Data = u8"Expected ';'",
				.Line = m_Current->Line,
				.Column = m_Current->Column,
			});

			return nullptr;
		} else if (auto exprNode = ParseExpression(); exprNode) {
			if (!AcceptToken(TokenType::Semicolon)) {
				m_Messages.push_back({
					.Type = MessageType::Error,
					.Data = u8"Expected ';'",
					.Line = m_Current->Line,
					.Column = m_Current->Column,
				});

				return nullptr;
			}

			return std::unique_ptr<StatementNode>(new VariableDeclarationNode(
				std::move(typeNode),
				nameToken->Data,
				std::move(exprNode)
			));
		} else {
			m_Messages.push_back({
				.Type = MessageType::Error,
				.Data = u8"Expected expression",
				.Line = m_Current->Line,
				.Column = m_Current->Column,
			});

			return nullptr;
		}
	}

	std::unique_ptr<BlockNode> Parser::ParseBlock() {
		if (!AcceptToken(TokenType::LeftBrace))
			return nullptr;

		std::vector<std::unique_ptr<StatementNode>> statements;

		while (!AcceptToken(TokenType::RightBrace)) {
			if (auto statement = ParseStatement(); statement) {
				statements.push_back(std::move(statement));
			} else {
				return nullptr;
			}
		}

		return std::unique_ptr<BlockNode>(new BlockNode(std::move(statements)));
	}
}