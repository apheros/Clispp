#pragma once
#include "Common.h"

class IFuncton;
class FunctionRegister
{
private:
	FunctionRegister();
	~FunctionRegister();

public:
	static FunctionRegister* GetInstance()
	{
		static auto* instance = new FunctionRegister;

		return instance;
	}

	void Init();
	void AddFunction(const string& function_name, ScriptFunction&& function);
	ScriptFunction GetFunction(const string& function_name);
	bool IsFunctor(const string& symbol);
	void Clear();

private:
	NameFunctionMap _name_function_map;
};

