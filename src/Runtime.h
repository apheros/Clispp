#pragma once
#include "Common.h"


class Runtime
{
public:
	Runtime();
	~Runtime();

public:
	void AddSymbol(const string& symbol_name, Any&& symbol);
	Any& GetSymbol(const string& symbol_name);

private:
	StringAnyMap _global_symbol_stack;
};

