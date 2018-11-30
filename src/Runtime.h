#pragma once
#include "Common.h"


class Runtime
{
public:
	Runtime();
	~Runtime();

public:
	void AddLocalSymbol(const Atom& symbol_name, Atom&& symbol);
	void AddGlobalymbol(Atom&& symbol_name, Atom&& symbol);
	void RemoveSymbol(const Atom& symbol_name);
	Atom& GetSymbol(Atom& symbol_name);

private:
	AtomAtomMap _global_symbol_stack;
	AtomAtomMap _local_symbol_stack;
};
