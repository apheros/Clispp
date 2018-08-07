#include "Tokenizer.h"


Token::Token(EToken token, string&& value)
	:token(token)
	,value(std::move(value))
{
}

const Token* Token::EmptyToken()
{
	static const Token* empty_token = new Token(NONE, "");
	return empty_token;
}

TokenStream::TokenStream()
	:_token_number(0)
	,_max_token_number(0)
{
}

TokenStream::~TokenStream()
{
	_Clear();
}

void TokenStream::_Clear()
{
	for (const Token* token : _token_vector)
	{
		if (token == nullptr)
		{
			continue;
		}

		delete token;
	}
}

void TokenStream::Push(Token* token)
{
	_token_vector.push_back(token);
	_max_token_number++;
}

const Token* TokenStream::Pop()
{
	if (IsEmpty())
	{
		return Token::EmptyToken();
	}

	return _token_vector[_token_number++];
}

void TokenStream::UndoPop()
{
	if (_token_number > 0)
	{
		_token_number--;
	}
}

bool TokenStream::IsEmpty() const
{
	return _token_number >= _max_token_number;
}