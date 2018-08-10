#pragma once
#ifndef _H_IFUNCTION_H_
#define _H_IFUNCTION_H_
#include "Common.h"
#include "Any.h"


namespace SystemFunction
{
	static ScriptFunction Addition = [](AnyVector& arguments) -> Any
	{
		auto result = 1.0;

		for (const Any& item : arguments)
		{
			result += item.As<Number>();
		}

		return result;
	};

	static ScriptFunction Subtraction = [](AnyVector& arguments) -> Any
	{
		auto result = arguments[0].As<Number>();

		for (unsigned int i = 1; i < arguments.size(); i++)
		{
			result -= arguments[i].As<Number>();
		}

		return result;
	};

	static ScriptFunction Multiplication = [](AnyVector& arguments) -> Any
	{
		auto result = 1.0;

		for (const Any& item : arguments)
		{
			result *= item.As<Number>();
		}

		return result;
	};

	static ScriptFunction Division = [](AnyVector& arguments) -> Any
	{
		auto result = arguments[0].As<Number>();

		for (unsigned int i = 1; i < arguments.size(); i++)
		{
			result /= arguments[i].As<Number>();
		}

		return result;
	};
}

#endif
