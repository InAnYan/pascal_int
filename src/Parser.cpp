#include <pscpch.hpp>
#include <Parser.hpp>
#include <ReportsManager.hpp>

using namespace std;

#define REQUIRE(_type) require(TokenType::_type)
#define MATCHING(_type) match(TokenType::_type).type != nullToken.type
#define IS_NULL(token) token.type == TokenType::NONE

namespace Pascal
{
	using namespace AST;
	
	unique_ptr<ProgramNode> Parser::parseProgram()
	{
		REQUIRE(PROGRAM);

		Token name = REQUIRE(IDENTIFIER);
		REQUIRE(SEMICOLON);
		unique_ptr<BlockNode> blk = parseBlock();

		REQUIRE(DOT);
		
		return make_unique<ProgramNode>(name, move(blk));
	}

	unique_ptr<BlockNode> Parser::parseBlock()
	{
		unique_ptr<vector<unique_ptr<VarDeclNode>>> varDecls =
			make_unique<vector<unique_ptr<VarDeclNode>>>();

		unique_ptr<vector<unique_ptr<ProcDeclNode>>> procDecls =
			make_unique<vector<unique_ptr<ProcDeclNode>>>();
	
		while (true)
		{
			if (MATCHING(VAR))
			{
				do
				{
					parseVarDecls(*varDecls);
					REQUIRE(SEMICOLON);
				}
				while (currentToken().type == TokenType::IDENTIFIER);
			}
			else if (MATCHING(PROCEDURE))
			{
				procDecls->push_back(parseProcDecl());
				REQUIRE(SEMICOLON);
			}
			else
			{
				break;
			}
		}
	    
		return make_unique<BlockNode>(move(varDecls), move(procDecls), parseCompound());
	}

	inline std::unique_ptr<ProcDeclNode> Parser::parseProcDecl()
	{
		Token id = REQUIRE(IDENTIFIER);

		unique_ptr<vector<unique_ptr<ParamNode>>> paramDecls =
			make_unique<vector<unique_ptr<ParamNode>>>();
		
		if (MATCHING(OPEN_PAREN))
		{
			do
			{
				parseParam(*paramDecls);
			} while (MATCHING(SEMICOLON));
			REQUIRE(CLOSE_PAREN);
		}
	    
		REQUIRE(SEMICOLON);
		return make_unique<ProcDeclNode>(id, move(paramDecls), parseBlock());
	}
	
	inline void Parser::parseParam(std::vector<std::unique_ptr<AST::ParamNode>>& res)
	{
		vector<Token_t> ids;
		ids.push_back(REQUIRE(IDENTIFIER));
		while (MATCHING(COMMA))
		{
			ids.push_back(REQUIRE(IDENTIFIER));
		}

		REQUIRE(COLON);

		TypeNode type(REQUIRE(IDENTIFIER));
		
		for (auto const& element : ids)
		{
			res.push_back(make_unique<ParamNode>(
								   make_unique<VariableNode>(element),
								   make_unique<TypeNode>(type)
								   ));
		}
	}
	
	inline void Parser::parseVarDecls(vector<unique_ptr<VarDeclNode>>& res)
	{
		vector<Token_t> ids;
		ids.push_back(REQUIRE(IDENTIFIER));
		while (MATCHING(COMMA))
		{
			ids.push_back(REQUIRE(IDENTIFIER));
		}

		REQUIRE(COLON);

		TypeNode type(REQUIRE(IDENTIFIER));
		
		for (auto const& element : ids)
		{
			res.push_back(make_unique<VarDeclNode>(
								   make_unique<VariableNode>(element),
								   make_unique<TypeNode>(type)
								   ));
		}
	}

	unique_ptr<CompoundNode> Parser::parseCompound()
	{
		REQUIRE(BEGIN);
		
		unique_ptr<vector<unique_ptr<StatementNode>>> statements =
			make_unique<vector<unique_ptr<StatementNode>>>();
		do
		{
			statements->push_back(parseStatement());
		}
		while (MATCHING(SEMICOLON));
		
		REQUIRE(END);

		return make_unique<CompoundNode>(move(statements));
	}

	unique_ptr<StatementNode> Parser::parseStatement()
	{
		if (currentToken().type == TokenType::BEGIN)
			return parseCompound();
		else if (currentToken().type == TokenType::IDENTIFIER)
			return parseAssignment();
		else
			return make_unique<AST::NullStatementNode>();
	}

	unique_ptr<AssignmentNode> Parser::parseAssignment()
	{
		unique_ptr<VariableNode> var = make_unique<VariableNode>(REQUIRE(IDENTIFIER));
		REQUIRE(ASSIGNMENT);
		unique_ptr<Node> expr = parseExpr();

		return make_unique<AssignmentNode>(move(var), move(expr));
	}

	unique_ptr<Node> Parser::parseExpr()
	{
		unique_ptr<Node> left = parseMultDiv();

		while (true)
		{
		    Token_t operation = match({ TokenType::PLUS, TokenType::MINUS });
			if (IS_NULL(operation))
				break;

			unique_ptr<Node> right = parseMultDiv();
			left = make_unique<BinOpNode>(move(left), move(right), operation);
		}
		
		return left;
	}

	unique_ptr<Node> Parser::parseMultDiv()
	{
		unique_ptr<Node> left = parseUnary();

		while (true)
		{
			Token_t operation = match({
					TokenType::PRODUCT, TokenType::DIVISION, TokenType::MOD
				});
			if (IS_NULL(operation))
				break;
			
			unique_ptr<Node> right = parseUnary();
			left = make_unique<BinOpNode>(move(left), move(right), operation);
		}
		
		return left;
	}

	unique_ptr<Node> Parser::parseUnary()
	{
		if (MATCHING(MINUS) || MATCHING(PLUS))
		{
			Token t = previousToken();
			return make_unique<UnaryOpNode>(parseUnary(), t);
		}
		else if (MATCHING(NUMBER_LITERAL))
		{
			return make_unique<NumberNode>(previousToken());
		}
		else if (MATCHING(IDENTIFIER))
		{
			return make_unique<VariableNode>(previousToken());
		}
		else if (MATCHING(OPEN_PAREN))
		{
			unique_ptr<Node> expr = parseExpr();
			REQUIRE(CLOSE_PAREN);
			return expr;
		}
		else
		{
			ReportsManager::ReportError(currentToken().pos, ErrorType::UNEXPECTED_WORD);
			return nullptr;
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

	Token Parser::match(vector<TokenType> const& types)
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

	inline Token Parser::require(TokenType type)
	{
		Token res = match(type);
		if (IS_NULL(res))
			ReportsManager::ReportError(currentToken().pos, ErrorType::EXPECTED, tokenTypeToString(type));
		return res;
	}

	Token Parser::require(vector<TokenType> const& types)
	{
		Token res = match(types);
		if (IS_NULL(res))
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

#undef REQUIRE
#undef MATCHING
#undef IS_NULL
