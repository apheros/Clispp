#pragma once
#include "Common.h"


class Runtime
{
public:
	Runtime();
	~Runtime();

public:
	void AddSymbol(Any&& symbol_name, Any&& symbol);
	Any& GetSymbol(Any& symbol_name);

private:
	AnyAnyMap _global_symbol_stack;
};

