#pragma once
#include "Common.h"


class Runtime
{
public:
	Runtime();
	~Runtime();

public:
	void AddSymbol(const String& symbol_name, Any&& symbol);
	Any& GetSymbol(const String& symbol_name);

private:
	StringAnyMap _global_symbol_stack;
};

