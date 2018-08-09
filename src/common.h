#pragma once
#include <vector>
#include <map>
#include <functional>
#include <set>


using namespace std;


static const string EMPTY_STRING = string();


typedef vector<string> StringVector;

class ASTNode;
typedef vector<ASTNode*> ASTNodeVector;

class Any;
typedef vector<Any> AnyVector;
typedef map<string, Any> StringAnyMap;

class Token;
typedef vector<const Token*> TokenVector;

typedef function<Any(AnyVector&)> ScriptFunction;
typedef map<std::string, ScriptFunction> NameFunctionMap;
