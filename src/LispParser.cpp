#include "LispParser.h"
#include "fstream"
#include <iostream>
#include "AST.h"


ParseArguments::ParseArguments()
	:_column_number(0)
	, _line_number(0)
	, _max_column_number(0)
	, _max_line_number(0)
{
	_file_lines.clear();
}

ParseArguments::~ParseArguments()
{
	Clear();
}

void ParseArguments::OnBeforeLexer()
{
	const string& line = _file_lines[_line_number];
	_max_column_number = static_cast<unsigned int>(line.size());
}

void ParseArguments::OnAfterLexer()
{
	
}

bool ParseArguments::IsEnd()
{
	return _line_number >= _max_line_number;
}

void ParseArguments::AddLine(string&& line)
{
	if (line.empty())
	{
		return;
	}

	_file_lines.push_back(std::move(line));
	_max_line_number++;
}

void ParseArguments::GetNextLine()
{
	if (_line_number < _max_line_number)
	{
		_column_number = 0;

		const string& line = _file_lines[_line_number++];
		_max_column_number = static_cast<unsigned int>(line.size());
	}
}

char ParseArguments::GetNextChar()
{
	if (_line_number >= _max_line_number)
	{
		return 0;
	}
	if (_column_number >= _max_column_number)
	{
		GetNextLine();
		return GetNextChar();
	}

	return _file_lines[_line_number][_column_number++];
}

void ParseArguments::UnDoGetNextChar()
{
	if (_column_number <= 0)
	{
		if (_line_number > 0)
		{
			_line_number--;
			_column_number = static_cast<unsigned int>(_file_lines[_line_number].size());
		}
	}
	else
	{
		_column_number--;
	}
}

char ParseArguments::GetBeforeChar()
{
	if (_column_number <= 0)
	{
		if (_line_number > 0)
		{
			return *_file_lines[_line_number - 1].rbegin();
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return _file_lines[_line_number][_column_number - 1];
	}
}

string ParseArguments::GetValue()
{
	const string& line = _file_lines[_line_number];

	const auto begin_position = _column_number;
	while ((_max_column_number > _column_number))
	{
		const char& temp_item = line[_column_number];

		if ((temp_item == ' ')
			|| (temp_item == ')')
			|| (temp_item == '\n'))
		{
			break;
		}

		_column_number++;
	}

	return line.substr(begin_position, _column_number-begin_position);
}

void ParseArguments::Clear()
{
	_file_lines.clear();
	_column_number = 0;
	_line_number = 0;
	_max_column_number = 0;
	_max_line_number = 0;
}

string ParseArguments::GetErrorString()
{
	return "Synax Error In Line: "  + std::to_string(_line_number) + " Colume: " + std::to_string(_column_number);
}


LispParser::LispParser()
{
}

LispParser::~LispParser()
{
}

void LispParser::ParserFromFile(const string& file_name)
{
	ifstream ifstream;
	ifstream.open(file_name, ios::in);
	if (ifstream.is_open())
	{
		while (!ifstream.eof())
		{
			string temp_line;
			getline(ifstream, temp_line);
			_argument.AddLine(std::move(temp_line));
		}

		//const string file_content((std::istreambuf_iterator<char>(ifstream)), std::istreambuf_iterator<char>());
		//ParserFromString(file_content);
		
		ifstream.close();

		ParserContext();
	}
}

void LispParser::ParserFromString(string&& file_content)
{
	_argument.AddLine(std::move(file_content));

	ParserContext();
}

void LispParser::ParserContext()
{
	if (!Lexer())
	{
		return;
	}

	if (!Parser())
	{
		return;
	}
}

bool LispParser::Lexer()
{
	_argument.OnBeforeLexer();

	while (!_argument.IsEnd())
	{
		const char& token_char = _argument.GetNextChar();
		if (token_char == 0)
		{
			break;
		}

		const EToken token = _CheckToken(token_char);

		switch (token)
		{
		case LPAREN:
		case RPAREN:
			{
			_token_stream.Push(new Token(token, string(1, token_char)));
			break;
			}
		case BOOLEN:
		case NUMBER:
		case STRING:
		case SYMBOL:
			{
			_argument.UnDoGetNextChar();
			_token_stream.Push(new Token(token, _argument.GetValue()));
			break;
			}
		case SPACE:
			{
			break;
			}
		default:
			{
			_PrintLexerError();
			return false;
			}
		}
	}

	_argument.OnAfterLexer();

	return true;
}

EToken LispParser::_CheckToken(const char& value)
{
	if (value == '(')
	{
		return LPAREN;
	}
	if (value == ')')
	{
		return RPAREN;
	}
	if (value == '#')
	{
		return BOOLEN;
	}
	if (value == '"')
	{
		return STRING;
	}
	if (value >= '0' && value <= '9')
	{
		return NUMBER;
	}
	if (value == ' ')
	{
		return SPACE;
	}
	else if (value != 0)
	{
		return SYMBOL;
	}

	return NONE;
}

void LispParser::_PrintLexerError()
{
	cout << _argument.GetErrorString() << endl;
}

bool LispParser::Parser()
{
	while (!_token_stream.IsEmpty())
	{
		_ast_vector.push_back(_CreatNode());
	}

	return true;
}

ASTNode* LispParser::_CreatNode()
{
	const Token* token = _token_stream.Pop();
	if (token->token == LPAREN)
	{
		return _CreatSExpression();
	}
	else if ((token->token == BOOLEN)
		|| (token->token == NUMBER)
		|| (token->token == STRING)
		|| (token->token == SYMBOL))
	{
		_token_stream.UndoPop();
		return _CreatSymbol();
	}

	return nullptr;
}

ASTNode* LispParser::_CreatSExpression()
{
	auto node = new ASTListNode;

	const auto* token = _token_stream.Pop();
	while (token->token != RPAREN)
	{
		if (_token_stream.IsEmpty())
		{
			_PrintParseError();
			
			delete node;

			return nullptr;
		}

		_token_stream.UndoPop();

		ASTNode* argument_node = _CreatNode();
		if (argument_node == nullptr)
		{
			_PrintParseError();
			
			delete node;

			return nullptr;
		}

		node->AddNode(argument_node);

		token = _token_stream.Pop();
	}

	return node;
}

ASTNode* LispParser::_CreatSymbol()
{
	const Token* token = _token_stream.Pop();
	if (token->token == BOOLEN)
	{
		bool value = token->value == "#t";

		return new ASTConstNode(Any(value));
	}
	else if (token->token == NUMBER)
	{
		double value = strtod(token->value.c_str(), nullptr);

		return new ASTConstNode(Any(value));
	}
	else if (token->token == STRING)
	{
		return new ASTConstNode(Any(token->value));
	}
	else if (token->token == SYMBOL)
	{
		return new ASTSymbolNode(Any(token->value));
	}

	return nullptr;
}

void LispParser::_PrintParseError()
{
}

void LispParser::ClearNodeVector(ASTNodeVector& node_vector)
{
	for (ASTNode* node : node_vector)
	{
		if (node == nullptr)
		{
			continue;
		}

		delete node;
	}

	node_vector.clear();
}

Any LispParser::Eval(Runtime* runtime)
{
	Any result = nullptr;

	for (auto* ast_node : _ast_vector)
	{
		if (ast_node == nullptr)
		{
			continue;
		}

		result = ast_node->Eval(runtime);
	}

	return result;
}
