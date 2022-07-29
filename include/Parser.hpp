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

		const Token_t nullToken = { TokenType::NONE, "", 0 };
	private:
		TokenList m_Tokens;
		size_t m_ParserPos = 0;
		
		inline Token match(TokenType type);
		inline Token require(TokenType type);

		Token match(std::vector<TokenType> const& types);
		Token require(std::vector<TokenType> const& types);
		
		inline Token currentToken();
		inline Token previousToken();

		std::unique_ptr<AST::BlockNode> parseBlock();
		std::unique_ptr<AST::CompoundNode> parseCompound();
		std::unique_ptr<AST::StatementNode> parseStatement();
		std::unique_ptr<AST::AssignmentNode> parseAssignment();
		std::unique_ptr<AST::ProcDeclNode> parseProcDecl();
		
		std::unique_ptr<AST::Node> parseExpr();
		std::unique_ptr<AST::Node> parseMultDiv();
		std::unique_ptr<AST::Node> parseUnary();

		inline void parseVarDecls(std::vector<std::unique_ptr<AST::VarDeclNode>>& res);
		inline void parseParam(std::vector<std::unique_ptr<AST::ParamNode>>& res);
	};

	class ParserRequiredException : public std::exception
	{
	public:
		ParserRequiredException(TokenType expected, size_t where)
			: m_Expected(expected), m_Where(where) {}

		const char*     what()  const throw() { return "expected"; }
		const TokenType type()  const throw() { return m_Expected; }
		const size_t    where() const throw() { return m_Where; }

	private:
		TokenType m_Expected;
		size_t m_Where;
	};

	class ParserUnexpectedException : public std::exception
	{
	public:
		ParserUnexpectedException(size_t where)
			: m_Where(where) {}

		const char*  what()  const throw() { return "unexpected"; }
		const size_t where() const throw() { return m_Where; }

	private:
		size_t m_Where;
	};
}

#endif
