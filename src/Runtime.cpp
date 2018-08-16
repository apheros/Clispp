#include "Runtime.h"
#include "Any.h"
#include "ScriptFunction.h"


AnyAnyMap Runtime::_static_symbol_stack = {
	{ string("+"), SystemFunction::Addition },
	{ string("-"), SystemFunction::Subtraction },
	{ string("-"), SystemFunction::Subtraction },
	{ string("*"), SystemFunction::Multiplication },
	{ string("/"), SystemFunction::Division },
	{ string("define"), SystemFunction::Define },
	{ string("lambda"), SystemFunction::Lambda },
};

Runtime::Runtime()
{
}

Runtime::~Runtime()
{
}

void Runtime::AddGlobalymbol(Any&& symbol_name, Any&& symbol)
{
	_global_symbol_stack[std::move(symbol_name)] = std::move(symbol);
}

void Runtime::AddLocalSymbol(const Any& symbol_name, Any&& symbol)
{
	_local_symbol_stack[symbol_name] = std::move(symbol);
}

void Runtime::RemoveSymbol(const Any& symbol_name)
{
	const auto iter = _local_symbol_stack.find(symbol_name);
	if (iter != _local_symbol_stack.end())
	{
		_local_symbol_stack.erase(iter);
	}
}

Any& Runtime::GetSymbol(Any& symbol_name)
{
	auto iter = _local_symbol_stack.find(symbol_name);
	if (iter != _local_symbol_stack.end())
	{
		return iter->second;
	}

	iter = _static_symbol_stack.find(symbol_name);
	if (iter != _static_symbol_stack.end())
	{
		return iter->second;
	}

	/*throw cannot_find_symbol();*/
	return symbol_name;
}
