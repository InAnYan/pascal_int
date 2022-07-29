#ifndef PASCAL_LEXER_HPP
#define PASCAL_LEXER_HPP

#include <string>
#include <memory>
#include <vector>

#include <stdexcept>

namespace Pascal
{
	enum class TokenType
	{
		NONE = 0,
		
		// Keywords
		PROGRAM,
		BEGIN,
		END,
		VAR,
		PROCEDURE,

		IDENTIFIER,

		// Punctuation
		DOT,
		COMMA,
		COLON,
		SEMICOLON,
		ASSIGNMENT,

		// Logical operators
		// TODO: Implement logical operators
		EQUAL,
		GREATER,
		LESS,
		GREATER_EQUAL,
		LESS_EQUAL,
		
		// Ariphmetic operations
		PLUS,
		MINUS,
		PRODUCT,
		DIVISION,
		MOD,
		
		// Pairs
		OPEN_PAREN,
		CLOSE_PAREN,
		OPEN_BRACKET,
		CLOSE_BRACKET,

		// Literals
		NUMBER_LITERAL,
		STRING_LITERAL,
		BOOL_LITERAL,

		// Special
		FILE_END
	};

	typedef struct
	{
		TokenType type;
		std::string str;
		size_t pos;
	} Token_t;

	std::string tokenTypeToString(TokenType type);
	
	typedef Token_t const& Token;

	typedef std::shared_ptr<std::vector<Token_t>> TokenList;
	
    TokenList Tokenize(std::shared_ptr<std::string> const& file);
}

#endif
