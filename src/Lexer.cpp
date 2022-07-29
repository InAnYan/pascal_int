#include <pscpch.hpp>
#include <Lexer.hpp>
#include <ReportsManager.hpp>

using namespace std;

namespace Pascal
{
	TokenList Tokenize(shared_ptr<string> const& source)
	{
	    stringstream ss;
	    TokenList res = make_shared<vector<Token_t>>();
		Token_t curTok;
		TokenType& ttype = curTok.type;
		string const& file = *source;
		
		for (size_t pos = 0; pos <= file.size(); pos++)
		{
			if (isalnum(file[pos]) || file[pos] == '_')
			{
				ss << file[pos];
			}
			else
			{
				if (file[pos] == '.' && pos != 0 && isdigit(file[pos - 1])) // Float literals
				{
					ss << file[pos];
					continue;
				}

				if (file[pos] == '{')
				{
					for (; pos <= file.size(); pos++)
					{
						if (file[pos] == '}')
						{
							pos++;
							break;
						}
					}
				}
				
				if (ss.tellp() != std::streampos(0))
				{
					string work = ss.str();
					transform(work.begin(), work.end(), work.begin(), ::tolower);
					ss.str("");
					ss.clear();
					
					if (work.rfind("//", 0) == 0)
					{
						for (; pos <= file.size(); pos++)
						{
							if (file[pos] == '\n')
								break;
						}
					}

					curTok.str = work;
					curTok.pos = pos - work.size();
					pos--;
					
					if (isdigit(work[0]))
						ttype = TokenType::NUMBER_LITERAL;
					else if (work == "begin")
						ttype = TokenType::BEGIN;
					else if (work == "end")
						ttype = TokenType::END;
					else if (work == "var")
						ttype = TokenType::VAR;
					else if (work == "true" || work == "false")
						ttype = TokenType::BOOL_LITERAL;
					else if (work == "program")
						ttype = TokenType::PROGRAM;
					else if (work == "procedure")
						ttype = TokenType::PROCEDURE;
					else
						ttype = TokenType::IDENTIFIER;

					res->push_back(curTok);
				}
				else
				{
					if (isspace(file[pos]))
						continue;
						
					curTok.str = std::string(1, file[pos]);
					curTok.pos = pos;
				
					switch(file[pos])
					{
					case '\0':
						continue;
					case ';':
						ttype = TokenType::SEMICOLON;
						break;
					case '.':
						ttype = TokenType::DOT;
						break;
					case ',':
						ttype = TokenType::COMMA;
						break;
					case ':':
						if (file[pos+1] == '=')
						{
							pos++;
							curTok.str = ":=";
							ttype = TokenType::ASSIGNMENT;
						}
						else
						{
							ttype = TokenType::COLON;
						}
						break;
					case '=':
						ttype = TokenType::EQUAL;
						break;
					case '+':
						ttype = TokenType::PLUS;
						break;
					case '-':
						ttype = TokenType::MINUS;
						break;
					case '/':
						ttype = TokenType::DIVISION;
						break;
					case '%':
						ttype = TokenType::MOD;
						break;
					case '*':
						ttype = TokenType::PRODUCT;
						break;
					case '(':
						ttype = TokenType::OPEN_PAREN;
						break;
					case ')':
						ttype = TokenType::CLOSE_PAREN;
						break;
					default:
						// TODO: Implement logical operators
						ReportsManager::ReportError(pos, ErrorType::ILLEGAL_LETTER);
					}

					res->push_back(curTok);
				}
			}
		}

		res->push_back({ TokenType::FILE_END, "", file.size() - 2 });
		
		return res;
	}

	std::string tokenTypeToString(TokenType type)
	{
		switch (type)
		{
		case TokenType::NONE:
			return "NONE";
		case TokenType::PROGRAM:
			return "PROGRAM";
		case TokenType::BEGIN:
			return "BEGIN";
		case TokenType::END:
			return "END";
		case TokenType::VAR:
			return "VAR";
		case TokenType::PROCEDURE:
			return "PROCEDURE";
		case TokenType::IDENTIFIER:
			return "IDENTIFIER";
		case TokenType::DOT:
			return "DOT";
		case TokenType::COMMA:
			return "COMMA";
		case TokenType::COLON:
			return "COLON";
		case TokenType::SEMICOLON:
			return "SEMICOLON";
		case TokenType::ASSIGNMENT:
			return "ASSIGNMENT";
		case TokenType::EQUAL:
			return "EQUAL";
		case TokenType::GREATER:
			return "GREATER";
		case TokenType::LESS:
			return "LESS";
		case TokenType::GREATER_EQUAL:
			return "GREATER_EQUAL";
		case TokenType::LESS_EQUAL:
			return "LESS_EQUAL";
		case TokenType::PLUS:
			return "PLUS";
		case TokenType::MINUS:
			return "MINUS";
		case TokenType::PRODUCT:
			return "PRODUCT";
		case TokenType::DIVISION:
			return "DIVISION";
		case TokenType::MOD:
			return "MOD";
		case TokenType::OPEN_PAREN:
			return "OPEN_PAREN";
		case TokenType::CLOSE_PAREN:
			return "CLOSE_PAREN";
		case TokenType::OPEN_BRACKET:
			return "OPEN_BRACKET";
		case TokenType::CLOSE_BRACKET:
			return "CLOSE_BRACKET";
		case TokenType::NUMBER_LITERAL:
			return "NUMBER_LITERAL";
		case TokenType::STRING_LITERAL:
			return "STRING_LITERAL";
		case TokenType::BOOL_LITERAL:
			return "BOOL_LITERAL";
		case TokenType::FILE_END:
			return "FILE_END";
		}
	}
}
