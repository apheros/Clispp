#pragma once
#ifndef _H_TOKENIZER_H_
#define _H_TOKENIZER_H_

#include "Common.h"


enum EToken
{
	NONE = 0,
	LPAREN,    //(
	RPAREN,    //)
	NUMBER,
	STRING,
	BOOLEN,    //#t, #f
	LIST,
	SYMBOL,
	SPACE,
	END,
};

class Token
{
public:
	Token(EToken token, String&& value);
	~Token() = default;

public:
	static const Token* EmptyToken();

public:
	EToken token;
	String value;
};

class TokenStream
{
public:
	TokenStream();
	~TokenStream();

public:
	void Push(Token* token);
	const Token* Pop();
	void UndoPop();
	Boolean IsEmpty() const;

private:
	void _Clear();

private:
	TokenVector _token_vector;

	unsigned int _token_number;
	unsigned int _max_token_number;
};

#endif
