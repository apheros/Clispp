#include "Runtime.h"
#include "Atom.h"
#include "System.h"
#include "SymbolTree.h"


using namespace System;


Runtime::Runtime()
{
}

Runtime::~Runtime()
{
}

void Runtime::AddGlobalSymbol(Atom&& symbol_name, Atom&& symbol) const
{
	_symbol_tree.AddGlobalValue(symbol_name, symbol);
}

void Runtime::AddLocalSymbol(const Atom& symbol_name, Atom&& symbol) const
{
	_symbol_tree.AddValue(symbol_name, symbol);
}

void Runtime::RemoveSymbol(const Atom& symbol_name) const
{
	_symbol_tree.RemoveValue(symbol_name);
}

Atom& Runtime::GetSymbol(Atom& symbol_name) const
{
	auto iter = STATIC_SYMBOL_STACK.find(symbol_name);
	if (iter != System::STATIC_SYMBOL_STACK.end())
	{
		return iter->second;
	}

	return _symbol_tree.GetValue(symbol_name);
}

void Runtime::EnterScope()
{
	_symbol_tree.EnterScope();
}

void Runtime::LeaveScope()
{
	_symbol_tree.LeaveScope();
}
