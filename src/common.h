#pragma once
#include <string>
#include <functional>

#include <vector>
#include <map>
#include <set>
#include <list>
#include <unordered_map>


using namespace std;


#define Number double
#define Boolean bool
#define String string
#define List AtomList


static const String EMPTY_STRING = String();


typedef vector<String> StringVector;

class ASTNode;
typedef vector<ASTNode*> ASTNodeVector;

class Atom;
typedef vector<Atom> AtomVector;
typedef map<Atom, Atom> AtomAtomMap;
typedef unordered_map<Atom, Atom> AtomAtomHashMap;
typedef list<Atom> AtomList;

class Token;
typedef vector<const Token*> TokenVector;

class Runtime;
typedef function<Atom(Runtime*, ASTNodeVector&)> ScriptFunction;
typedef map<String, ScriptFunction> NameFunctionMap;


enum EType
{
	TYPE_NONE,
	TYPE_CONST,
	TYPE_SYMBOL,
	TYPE_LIST,
};
