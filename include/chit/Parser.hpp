#pragma once

#include <chit/Message.hpp>
#include <chit/Token.hpp>
#include <chit/ast/Node.hpp>

#include <memory>
#include <span>
#include <vector>

namespace chit {
	class Parser final {
	private:
		std::span<const Token> m_Tokens;
		std::span<const Token>::iterator m_Current;

		std::unique_ptr<RootNode> m_RootNode;
		std::vector<Message> m_Messages;

	public:
		explicit Parser(std::span<const Token> tokens) noexcept;
		Parser(Parser&& other) noexcept = default;
		~Parser() = default;

	public:
		Parser& operator=(Parser&& other) noexcept = default;

	public:
		void Parse();
		const RootNode* GetRootNode() const noexcept;
		std::span<const Message> GetMessages() const noexcept;

	private:
		const Token* AcceptToken(TokenType tokenType) noexcept;

		std::unique_ptr<ExpressionNode> ParseExpression();
		std::unique_ptr<ExpressionNode> ParseAssignment();
		std::unique_ptr<ExpressionNode> ParseFunctionCall();
		std::unique_ptr<ExpressionNode> ParseSimpleExpression();

		std::unique_ptr<StatementNode> ParseStatement();
		std::unique_ptr<StatementNode> ParseReturn();
		std::unique_ptr<StatementNode> ParseFunctionDeclaration(
			std::unique_ptr<TypeNode> returnTypeNode,
			const Token* nameToken);
		std::unique_ptr<StatementNode> ParseVariableDeclaration(
			std::unique_ptr<TypeNode> typeNode,
			const Token* nameToken);

		std::unique_ptr<BlockNode> ParseBlock();

		std::unique_ptr<TypeNode> ParseType();
	};
}