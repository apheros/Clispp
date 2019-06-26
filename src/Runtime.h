#pragma once
#include "SymbolTree.h"


class Runtime
{
public:
	Runtime();
	~Runtime();

public:
	void AddLocalSymbol(const Atom& symbol_name, Atom&& symbol) const;
	void AddGlobalSymbol(Atom&& symbol_name, Atom&& symbol) const;
	void RemoveSymbol(const Atom& symbol_name) const;
	Atom& GetSymbol(Atom& symbol_name) const;
	void EnterScope();
	void LeaveScope();

private:
	SymbolTree _symbol_tree;
};
