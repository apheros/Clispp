#pragma once
#include <vector>
#include <map>
#include <functional>


using namespace std;


typedef vector<string> StringVector;

class ASTNode;
typedef vector<ASTNode*> ASTNodeVector;

class Any;
typedef vector<Any> AnyVector;


class Token;
typedef vector<const Token*> TokenVector;

typedef function<Any(AnyVector&)> ScriptFunction;
typedef map<std::string, ScriptFunction> NameFunctionMap;
