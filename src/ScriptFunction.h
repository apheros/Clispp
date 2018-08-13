#pragma once
#ifndef _H_IFUNCTION_H_
#define _H_IFUNCTION_H_
#include "Common.h"
#include "Any.h"


namespace SystemFunction
{
	static ScriptFunction Addition = [](Runtime*, AnyVector& arguments) -> Any
	{
		auto result = 0.0;

		for (const Any& item : arguments)
		{
			result += item.As<Number>();
		}

		return result;
	};

	static ScriptFunction Subtraction = [](Runtime*, AnyVector& arguments) -> Any
	{
		auto result = arguments[0].As<Number>();

		for (unsigned int i = 1; i < arguments.size(); i++)
		{
			result -= arguments[i].As<Number>();
		}

		return result;
	};

	static ScriptFunction Multiplication = [](Runtime*, AnyVector& arguments) -> Any
	{
		auto result = 1.0;

		for (const Any& item : arguments)
		{
			result *= item.As<Number>();
		}

		return result;
	};

	static ScriptFunction Division = [](Runtime*, AnyVector& arguments) -> Any
	{
		auto result = arguments[0].As<Number>();

		for (unsigned int i = 1; i < arguments.size(); i++)
		{
			result /= arguments[i].As<Number>();
		}

		return result;
	};

	static ScriptFunction Let = [](Runtime* runtime, AnyVector& arguments) -> Any
	{
		if (runtime == nullptr)
		{
			return Any::EmptyValue();
		}

		if (arguments.size() < 2)
		{
			throw wrong_argument_size();
		}

		runtime->AddSymbol(std::move(arguments[0]), std::move(arguments[1]));

		return Any::EmptyValue();
	};
}

#endif
