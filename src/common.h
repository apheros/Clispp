#pragma once
#include <string>
#include <functional>

#include <vector>
#include <map>
#include <set>
#include <list>


using namespace std;


#define Number double
#define Boolean bool
#define String string
#define List AnyList


static const String EMPTY_STRING = String();


typedef vector<String> StringVector;

class ASTNode;
typedef vector<ASTNode*> ASTNodeVector;

class Any;
typedef vector<Any> AnyVector;
typedef map<String, Any> StringAnyMap;
typedef list<Any> AnyList;

class Token;
typedef vector<const Token*> TokenVector;

typedef function<Any(AnyVector&)> ScriptFunction;
typedef map<String, ScriptFunction> NameFunctionMap;
