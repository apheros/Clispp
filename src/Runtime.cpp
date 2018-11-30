#include "Runtime.h"
#include "Atom.h"
#include "System.h"


using namespace System;


Runtime::Runtime()
{
}

Runtime::~Runtime()
{
}

void Runtime::AddGlobalymbol(Atom&& symbol_name, Atom&& symbol)
{
	_global_symbol_stack[std::move(symbol_name)] = std::move(symbol);
}

void Runtime::AddLocalSymbol(const Atom& symbol_name, Atom&& symbol)
{
	_local_symbol_stack[symbol_name] = std::move(symbol);
}

void Runtime::RemoveSymbol(const Atom& symbol_name)
{
	const auto iter = _local_symbol_stack.find(symbol_name);
	if (iter != _local_symbol_stack.end())
	{
		_local_symbol_stack.erase(iter);
	}
}

Atom& Runtime::GetSymbol(Atom& symbol_name)
{
	auto iter = STATIC_SYMBOL_STACK.find(symbol_name);
	if (iter != System::STATIC_SYMBOL_STACK.end())
	{
		return iter->second;
	}

	iter = _local_symbol_stack.find(symbol_name);
	if (iter != _local_symbol_stack.end())
	{
		return iter->second;
	}

	iter = _global_symbol_stack.find(symbol_name);
	if (iter != _global_symbol_stack.end())
	{
		return iter->second;
	}

	/*throw cannot_find_symbol();*/

	return symbol_name;
}
