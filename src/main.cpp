#include <iostream>
#include "Common.h"
#include "LispParser.h"
#include "Runtime.h"
#include "Any.h"


using namespace std;


int main(int argc, char* argv[])
{
	const String file_name = "E:/Workshop/Cpp/LispParser/res/test.lsp";

	cout << "Start Parsing Lisp file: " << file_name << endl;

	LispParser parser;

	parser.ParserFromFile(file_name);

	auto* runtime = new Runtime();

	const Any& result = parser.Eval(runtime);

	cout << "Result: " << result.ToString() << endl;

	return 0;
}
