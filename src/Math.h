#pragma once
#pragma once
#ifndef _H_SYSTEM_MATH_H_
#define _H_SYSTEM_MATH_H_
#include "TypeDefine.h"
#include "Atom.h"
#include "Runtime.h"
#include "AST.h"


namespace System
{
	inline void EvalASTVevtor(Runtime* runtime, ASTNodeVector& node_arguments, AtomVector& result_vector)
	{
		auto iter = node_arguments.begin();

		for (++iter; iter != node_arguments.end(); ++iter)
		{
			auto* node = *iter;
			if (node == nullptr)
			{
				continue;
			}

			result_vector.push_back(node->Eval(runtime));
		}
	}

	static ScriptFunction Addition = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Atom
	{
		AtomVector arguments;
		EvalASTVevtor(runtime, node_arguments, arguments);

		auto result = 0.0;
		for (const Atom& item : arguments)
		{
			result += item.As<Number>();
		}

		return result;
	};

	static ScriptFunction Subtraction = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Atom
	{
		AtomVector arguments;
		EvalASTVevtor(runtime, node_arguments, arguments);

		if (arguments.empty())
		{
			throw wrong_argument_size();
		}

		auto result = arguments[0].As<Number>();

		for (unsigned int i = 1; i < arguments.size(); i++)
		{
			result -= arguments[i].As<Number>();
		}

		return result;
	};

	static ScriptFunction Multiplication = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Atom
	{
		AtomVector arguments;
		EvalASTVevtor(runtime, node_arguments, arguments);

		auto result = 1.0;
		for (const Atom& item : arguments)
		{
			result *= item.As<Number>();
		}

		return result;
	};

	static ScriptFunction Division = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Atom
	{
		AtomVector arguments;
		EvalASTVevtor(runtime, node_arguments, arguments);

		if (arguments.empty())
		{
			throw wrong_argument_size();
		}

		auto result = arguments[0].As<Number>();

		for (unsigned int i = 1; i < arguments.size(); i++)
		{
			result /= arguments[i].As<Number>();
		}

		return result;
	};
}

#endif
