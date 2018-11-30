#include <iostream>
#include "TypeDefine.h"
#include "LispParser.h"
#include "Runtime.h"
#include "Atom.h"


using namespace std;


int main(int argc, char* argv[])
{
	const String file_name = "E:/Workshop/Cpp/LispParser/res/test.lisp";

	cout << "Start Parsing Lisp file: " << file_name << endl;

	try
	{
		LispParser parser;

		parser.ParserFromFile(file_name);

		auto* runtime = new Runtime();

		const Atom& result = parser.Eval(runtime);

		cout << "Result: " << result.ToString() << endl;
	}
	catch (exception& exception)
	{
		cout << "Error: " << exception.what() << endl;
	}

	return 0;
}
