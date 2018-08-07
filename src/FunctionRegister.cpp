#include "FunctionRegister.h"
#include "ScriptFunction.h"


FunctionRegister::FunctionRegister()
{
}

FunctionRegister::~FunctionRegister()
{
	Clear();
}

void FunctionRegister::Init()
{
	AddFunction("+", Add);
	AddFunction("-", Minus);
}

void FunctionRegister::AddFunction(const string& function_name, ScriptFunction&& function)
{
	if (function == nullptr)
	{
		return;
	}

	_name_function_map[function_name] = function;
}

ScriptFunction FunctionRegister::GetFunction(const std::string& function_name)
{
	const auto iter = _name_function_map.find(function_name);
	if (iter != _name_function_map.end())
	{
		return iter->second;
	}

	return nullptr;
}

void FunctionRegister::Clear()
{
	_name_function_map.clear();
}

bool FunctionRegister::IsFunctor(const string& symbol)
{
	const auto iter = _name_function_map.find(symbol);
	return iter != _name_function_map.end();
}
