#pragma once
#ifndef _H_TOKENIZER_H_
#define _H_TOKENIZER_H_

#include "common.h"


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
	Token(EToken token, string&& value);
	~Token() = default;

public:
	static const Token* EmptyToken();

public:
	EToken token;
	string value;

private:
	
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
	bool IsEmpty() const;

private:
	void _Clear();

private:
	TokenVector _token_vector;

	unsigned int _token_number;
	unsigned int _max_token_number;
};

#endif
