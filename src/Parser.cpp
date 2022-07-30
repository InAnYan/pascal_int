#include "AST.hpp"
#include "Lexer.hpp"
#include <memory>
#include <pscpch.hpp>
#include <Parser.hpp>
#include <ReportsManager.hpp>

namespace Pascal
{	
	std::unique_ptr<AST::ProgramNode> Parser::parseProgram()
	{
		require(TokenType::PROGRAM);

		Token name = require(TokenType::IDENTIFIER);
		require(TokenType::SEMICOLON);
		std::unique_ptr<AST::BlockNode> blk = parseBlock();

		require(TokenType::DOT);
		
		return std::make_unique<AST::ProgramNode>(name, move(blk));
	}

	std::unique_ptr<AST::BlockNode> Parser::parseBlock()
	{
	    std::vector<std::unique_ptr<AST::VarDeclNode>> varDecls;
		std::vector<std::unique_ptr<AST::ProcDeclNode>> procDecls;
	
		while (true)
		{
			if (matching(TokenType::VAR))
			{
				do
				{
					parseVarDecls(varDecls);
					require(TokenType::SEMICOLON);
				}
				while (currentToken().type == TokenType::IDENTIFIER);
			}
			else if (matching(TokenType::PROCEDURE))
			{
				procDecls.push_back(parseProcDecl());
				require(TokenType::SEMICOLON);
			}
			else
			{
				break;
			}
		}
	    
		return std::make_unique<AST::BlockNode>(move(varDecls), move(procDecls), parseCompound());
	}

	inline std::unique_ptr<AST::ProcDeclNode> Parser::parseProcDecl()
	{
		Token id = require(TokenType::IDENTIFIER);

	    std::vector<std::unique_ptr<AST::ParamNode>> paramDecls;  
		
		if (matching(TokenType::OPEN_PAREN))
		{
			do
			{
				parseParam(paramDecls);
			} while (matching(TokenType::SEMICOLON));
			require(TokenType::CLOSE_PAREN);
		}
	    
		require(TokenType::SEMICOLON);
		return std::make_unique<AST::ProcDeclNode>(id, move(paramDecls), parseBlock());
	}
	
	inline void Parser::parseParam(std::vector<std::unique_ptr<AST::ParamNode>>& res)
	{
		std::vector<Token_t> ids;
		ids.push_back(require(TokenType::IDENTIFIER));
		while (matching(TokenType::COMMA))
		{
			ids.push_back(require(TokenType::IDENTIFIER));
		}

		require(TokenType::COLON);

		AST::TypeNode type(require(TokenType::IDENTIFIER));
		
		for (auto const& element : ids)
		{
			res.push_back(std::make_unique<AST::ParamNode>(
								   std::make_unique<AST::VariableNode>(element),
								   std::make_unique<AST::TypeNode>(type)
								   ));
		}
	}
	
	inline void Parser::parseVarDecls(std::vector<std::unique_ptr<AST::VarDeclNode>>& res)
	{
		std::vector<Token_t> ids;
		ids.push_back(require(TokenType::IDENTIFIER));
		while (matching(TokenType::COMMA))
		{
			ids.push_back(require(TokenType::IDENTIFIER));
		}

		require(TokenType::COLON);

		AST::TypeNode type(require(TokenType::IDENTIFIER));
		
		for (auto const& element : ids)
		{
			res.push_back(std::make_unique<AST::VarDeclNode>(
							  std::make_unique<AST::VariableNode>(element),
							  std::make_unique<AST::TypeNode>(type)
							  ));
		}
	}

	std::unique_ptr<AST::CompoundNode> Parser::parseCompound()
	{
		require(TokenType::BEGIN);
		
		std::vector<std::unique_ptr<AST::StatementNode>> statements;
		
		do
		{
			statements.push_back(parseStatement());
		}
		while (matching(TokenType::SEMICOLON));
		
		require(TokenType::END);

		return std::make_unique<AST::CompoundNode>(move(statements));
	}

	std::unique_ptr<AST::StatementNode> Parser::parseStatement()
	{
		if (currentToken().type == TokenType::BEGIN)
		{
			return parseCompound();
		}
		else if (currentToken().type == TokenType::IDENTIFIER)
		{
			Token id = match(TokenType::IDENTIFIER);
			Token temp = match({ TokenType::ASSIGNMENT, TokenType::OPEN_PAREN });
			switch (temp.type)
			{
			case TokenType::ASSIGNMENT:
			{
				return std::make_unique<AST::AssignmentNode>(
					std::make_unique<AST::VariableNode>(id),
					parseExpr()
					);
				break;
			}
			case TokenType::OPEN_PAREN:
			{
				std::vector<std::unique_ptr<AST::Node>> params;
				if (!matching(TokenType::CLOSE_PAREN))
				{
					do
					{
					    params.push_back(parseExpr());
					} while (matching(TokenType::COMMA));

					require(TokenType::CLOSE_PAREN);
				}
				return std::make_unique<AST::ProcCallNode>(id, std::move(params));
				break;
			}
			default:
				ReportsManager::ReportError(temp.pos, ErrorType::ILLEGAL_STATEMENT);
				return std::make_unique<AST::NullStatementNode>();
			}
		}
		else if (currentToken().type == TokenType::SEMICOLON)
		{
			ReportsManager::ReportError(currentToken().pos, ErrorType::ILLEGAL_STATEMENT);
			return std::make_unique<AST::NullStatementNode>();
		}
		else
		{
			return std::make_unique<AST::NullStatementNode>();
		}
	}

	std::unique_ptr<AST::AssignmentNode> Parser::parseAssignment()
	{
		std::unique_ptr<AST::VariableNode> var = std::make_unique<AST::VariableNode>(require(TokenType::IDENTIFIER));
		require(TokenType::ASSIGNMENT);
		std::unique_ptr<AST::Node> expr = parseExpr();

		return std::make_unique<AST::AssignmentNode>(move(var), move(expr));
	}

	std::unique_ptr<AST::Node> Parser::parseExpr()
	{
		std::unique_ptr<AST::Node> left = parseMultDiv();

		while (true)
		{
		    Token_t operation = match({ TokenType::PLUS, TokenType::MINUS });
			if (operation.type == TokenType::NONE)
				break;

			std::unique_ptr<AST::Node> right = parseMultDiv();
			left = std::make_unique<AST::BinOpNode>(move(left), move(right), operation);
		}
		
		return left;
	}

	std::unique_ptr<AST::Node> Parser::parseMultDiv()
	{
		std::unique_ptr<AST::Node> left = parseUnary();

		while (true)
		{
			Token_t operation = match({
					TokenType::PRODUCT, TokenType::DIVISION, TokenType::MOD
				});
			if (operation.type == TokenType::NONE)
				break;
			
			std::unique_ptr<AST::Node> right = parseUnary();
			left = std::make_unique<AST::BinOpNode>(move(left), move(right), operation);
		}
		
		return left;
	}

	std::unique_ptr<AST::Node> Parser::parseUnary()
	{
		if (matching(TokenType::MINUS) || matching(TokenType::PLUS))
		{
			Token t = previousToken();
			return std::make_unique<AST::UnaryOpNode>(parseUnary(), t);
		}
		else if (matching(TokenType::NUMBER_LITERAL))
		{
			return std::make_unique<AST::NumberNode>(previousToken());
		}
		else if (matching(TokenType::IDENTIFIER))
		{
			return std::make_unique<AST::VariableNode>(previousToken());
		}
		else if (matching(TokenType::OPEN_PAREN))
		{
			std::unique_ptr<AST::Node> expr = parseExpr();
			require(TokenType::CLOSE_PAREN);
			return expr;
		}
		else
		{
			ReportsManager::ReportError(currentToken().pos, ErrorType::UNEXPECTED_WORD);
			return std::make_unique<AST::NullStatementNode>();
		}
	}

	inline Token Parser::match(TokenType type)
	{
		if (currentToken().type == type)
		{
			m_ParserPos++;
			return previousToken();
		}
		else
		{
			return nullToken;
		}	
	}

	Token Parser::match(std::vector<TokenType> const& types)
	{
		Token curTok = currentToken();

		for (auto e : types)
		{
			if (curTok.type == e)
			{
				m_ParserPos++;
				return curTok;
			}
		}

		return nullToken;
	}

	inline bool Parser::matching(TokenType type)
	{
		return match(type).type != TokenType::NONE;
	}

	inline Token Parser::require(TokenType type)
	{
		Token res = match(type);
		if (res.type == TokenType::NONE)
			ReportsManager::ReportError(currentToken().pos, ErrorType::EXPECTED, tokenTypeToString(type));
		return res;
	}

	Token Parser::require(std::vector<TokenType> const& types)
	{
		Token res = match(types);
		if (res.type == TokenType::NONE)
		{
			std::string err = "";
		    for (unsigned i = 0; i < types.size(); i++)
			{
				err += tokenTypeToString(types[i]);
				if (i != types.size() - 1)
					err += " or ";
			}
			ReportsManager::ReportError(currentToken().pos, ErrorType::EXPECTED, err);
		}
		return res;
	}

	inline Token Parser::currentToken()
	{
		if (m_ParserPos < m_Tokens->size())
			return (*m_Tokens)[m_ParserPos];
		else
			return nullToken;
	}

	inline Token Parser::previousToken()
	{
		if (m_ParserPos != 0)
			return (*m_Tokens)[m_ParserPos - 1];
		else
			return nullToken;
	}
}
