#ifndef PASCAL_PARSER_HPP
#define PASCAL_PARSER_HPP

#include <Lexer.hpp>
#include <AST.hpp>

namespace Pascal
{
	class Parser
	{
	public:
		Parser(TokenList tokens)
			: m_Tokens(tokens) {}

		std::unique_ptr<AST::ProgramNode> parseProgram();

		std::unique_ptr<AST::BlockNode> parseBlock();
		std::unique_ptr<AST::CompoundNode> parseCompound();
		std::unique_ptr<AST::StatementNode> parseStatement();
		std::unique_ptr<AST::AssignmentNode> parseAssignment();
		std::unique_ptr<AST::ProcDeclNode> parseProcDecl();
		
		std::unique_ptr<AST::Node> parseExpr();
		std::unique_ptr<AST::Node> parseMultDiv();
		std::unique_ptr<AST::Node> parseUnary();
	private:
		TokenList m_Tokens;
		size_t m_ParserPos = 0;
		
		inline Token match(TokenType type);
		Token match(std::vector<TokenType> const& types);
		inline bool matching(TokenType type);
		
		inline Token require(TokenType type);
		Token require(std::vector<TokenType> const& types);
		
		inline Token currentToken();
		inline Token previousToken();
		
		inline void parseVarDecls(std::vector<std::unique_ptr<AST::VarDeclNode>>& res);
		inline void parseParam(std::vector<std::unique_ptr<AST::ParamNode>>& res);
	};
}

#endif
