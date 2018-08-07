#pragma once
#ifndef _H_LISPPARSER_H_
#define _H_LISPPARSER_H_
#include <string>
#include "common.h"
#include "Tokenizer.h"
#include "Runtime.h"


class ParseArguments
{
public:
	ParseArguments();
	~ParseArguments();

	void OnBeforeLexer();
	void OnAfterLexer();
	void AddLine(string&& line);
	void GetNextLine();
	char GetNextChar();
	char GetBeforeChar();
	void UnDoGetNextChar();
	string GetValue();
	bool IsEnd();
	void Clear();

	string GetErrorString();

private:
	StringVector _file_lines;

	unsigned int _column_number;
	unsigned int _line_number;
	unsigned int _max_column_number;
	unsigned int _max_line_number;
};

class LispParser
{
public:
	LispParser();
	~LispParser();

public:
	static void ClearNodeVector(ASTNodeVector& node_vector);

	void ParserFromFile(const string& file_name);
	void ParserFromString(string&& file_content);
	void ParserContext();
	bool Lexer();
	bool Parser();
	void Eval(Runtime* runtime);

private:
	EToken _CheckToken(const char& value);
	void _PrintLexerError();
	void _PrintParseError();
	ASTNode* _CreatNode();
	ASTNode* _CreatSExpression();
	ASTNode* _CreatSymbol();


private:
	ParseArguments _argument;
	TokenStream _token_stream;
	ASTNode* _ast_root;
};

#endif