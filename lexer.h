/*
 * lexer.h
 *
 *      Author: connor roche
 */

#ifndef LEXER_H_
#define LEXER_H_

#include <string>
#include <iostream>
using std::string;
using std::istream;
using std::ostream;

enum TokenType {
		// keywords
	T_INT,
	T_STRING,
	T_SET,
	T_PRINT,
	T_PRINTLN,

		// an identifier
	T_ID,

		// an integer and string constant
	T_ICONST,
	T_SCONST,

		// the operators, parens and semicolon
	T_PLUS,
	T_MINUS,
	T_STAR,
	T_SLASH,
	T_LPAREN,
	T_RPAREN,
	T_SC,

		// any error returns this token
	T_ERROR,

		// when completed (EOF), return this token
	T_DONE
};

class Token {
	TokenType	tt;
	string		lexeme;
	int			lnum;

public:
	Token(TokenType tt = T_ERROR, string lexeme = "") : tt(tt), lexeme(lexeme) {
		extern int lineNumber;
		lnum = lineNumber;
	}

	bool operator==(const TokenType tt) const { return this->tt == tt; }
	bool operator!=(const TokenType tt) const { return this->tt != tt; }

	TokenType	GetTokenType() const { return tt; }
	string		GetLexeme() const { return lexeme; }
	int			GetLinenum() const { return lnum; }
};

extern ostream& operator<<(ostream& out, const Token& tok);

extern Token getToken(istream* br);


#endif /* LEXER_H_ */
