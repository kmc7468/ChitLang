#include <chit/Parser.hpp>

#include <chit/ast/Function.hpp>
#include <chit/ast/Type.hpp>

#include <cassert>
#include <string_view>
#include <utility>

namespace chit {
	Parser::Parser(std::span<const Token> tokens) noexcept
		: m_Tokens(tokens) {
		m_Current = m_Tokens.begin();
	}

	void Parser::Parse() {
		assert(m_Current == m_Tokens.begin());
		assert(m_RootNode == nullptr);

		m_RootNode = std::make_unique<RootNode>();

		while (m_Current < m_Tokens.end()) {
			if (auto statement = ParseStatement(); statement) {
				m_RootNode->Statements.push_back(std::move(statement));
			} else {
				break;
			}
		}
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

#define ACCEPT(n, t) const auto n = AcceptToken(t); n

	std::unique_ptr<StatementNode> Parser::ParseStatement() {
		if (AcceptToken(TokenType::Semicolon)) {
			// Ignore empty statement
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
				// TODO
			}
		}

		return nullptr;
	}
	std::unique_ptr<TypeNode> Parser::ParseType() {
		if (ACCEPT(voidToken, TokenType::Void)) {
			return std::unique_ptr<TypeNode>(new IdentifierTypeNode(voidToken->Data));
		} else if (ACCEPT(intToken, TokenType::Int)) {
			return std::unique_ptr<TypeNode>(new IdentifierTypeNode(intToken->Data));
		} else {
			return nullptr;
		}
	}

	std::unique_ptr<BlockNode> Parser::ParseBlock() {
		if (!AcceptToken(TokenType::LeftBrace))
			return nullptr;

		std::vector<std::unique_ptr<StatementNode>> statements;

		while (!AcceptToken(TokenType::RightBrace)) {
			statements.push_back(ParseStatement());
		}

		return std::unique_ptr<BlockNode>(new BlockNode(std::move(statements)));
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
}