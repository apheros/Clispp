#include <iostream>
#include <string>
#include "LispParser.h"
#include "FunctionRegister.h"


using namespace std;


int main(int argc, char* argv[])
{
	FunctionRegister::GetInstance()->Init();

	const string file_name = "E:/Workshop/Cpp/LispParser/res/test.lsp";

	cout << "Start Parsing Lisp file: " << file_name << endl;

	LispParser parser;

	parser.ParserFromFile(file_name);

	return 0;
}
