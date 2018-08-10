#include "Runtime.h"
#include "Any.h"
#include "ScriptFunction.h"


Runtime::Runtime()
{
	AddSymbol("+", Any(SystemFunction::Addition));
	AddSymbol("-", Any(SystemFunction::Subtraction));
	AddSymbol("*", Any(SystemFunction::Multiplication));
	AddSymbol("/", Any(SystemFunction::Division));
}

Runtime::~Runtime()
{
	_global_symbol_stack.clear();
}

void Runtime::AddSymbol(const String& symbol_name, Any&& symbol)
{
	_global_symbol_stack[symbol_name] = std::move(symbol);
}

Any& Runtime::GetSymbol(const String& symbol_name)
{
	auto iter = _global_symbol_stack.find(symbol_name);
	if (iter != _global_symbol_stack.end())
	{
		return iter->second;
	}

	throw std::bad_typeid();
}
