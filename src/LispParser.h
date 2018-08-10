#pragma once
#ifndef _H_LISPPARSER_H_
#define _H_LISPPARSER_H_
#include "Common.h"
#include "Tokenizer.h"


class Runtime;
class ParseArguments
{
public:
	ParseArguments();
	~ParseArguments();

	void OnBeforeLexer();
	void OnAfterLexer();
	void AddLine(String&& line);
	void GetNextLine();
	char GetNextChar();
	char GetBeforeChar();
	void UnDoGetNextChar();
	String GetValue();
	Boolean IsEnd();
	void Clear();

	String GetErrorString();

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

	void ParserFromFile(const String& file_name);
	void ParserFromString(String&& file_content);
	void ParserContext();
	Boolean Lexer();
	Boolean Parser();
	Any Eval(Runtime* runtime);

private:
	static EToken _CheckToken(const char& value);
	void _PrintLexerError();
	void _PrintParseError();
	ASTNode* _CreatNode();
	ASTNode* _CreatSExpression();
	ASTNode* _CreatSymbol();


private:
	ParseArguments _argument;
	TokenStream _token_stream;
	ASTNodeVector _ast_vector;
};

#endif