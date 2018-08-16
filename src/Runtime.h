#pragma once
#include "Common.h"


class Runtime
{
public:
	Runtime();
	~Runtime();

public:
	void AddLocalSymbol(const Any& symbol_name, Any&& symbol);
	void AddGlobalymbol(Any&& symbol_name, Any&& symbol);
	void RemoveSymbol(const Any& symbol_name);
	Any& GetSymbol(Any& symbol_name);

private:
	static AnyAnyMap _static_symbol_stack;
	AnyAnyMap _global_symbol_stack;
	AnyAnyMap _local_symbol_stack;
};
