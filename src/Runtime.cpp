#include "Runtime.h"
#include "Any.h"
#include "ScriptFunction.h"


Runtime::Runtime()
{
	AddSymbol(string("+"), SystemFunction::Addition);
	AddSymbol(string("-"), SystemFunction::Subtraction);
	AddSymbol(string("*"), SystemFunction::Multiplication);
	AddSymbol(string("/"), SystemFunction::Division);
	AddSymbol(string("let"), SystemFunction::Let);
}

Runtime::~Runtime()
{
	_global_symbol_stack.clear();
}

void Runtime::AddSymbol(Any&& symbol_name, Any&& symbol)
{
	_global_symbol_stack[std::move(symbol_name)] = std::move(symbol);
}

Any& Runtime::GetSymbol(Any& symbol_name)
{
	auto iter = _global_symbol_stack.find(symbol_name);
	if (iter != _global_symbol_stack.end())
	{
		return iter->second;
	}

	/*throw cannot_find_symbol();*/
	return symbol_name;
}
