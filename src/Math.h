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
	inline void EvalASTVector(Runtime* runtime, ASTNodeVector& node_arguments, AtomVector& result_vector)
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

	inline void EvalASTVector(Runtime* runtime, ASTNodeVector::iterator& begin, ASTNodeVector::iterator&& end, AtomVector& result_vector)
	{
		for (; begin != end; ++begin)
		{
			auto* node = *begin;
			if (node == nullptr)
			{
				continue;
			}

			result_vector.push_back(node->Eval(runtime));
		}
	}

	static SystemFunction Addition = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Atom
	{
		AtomVector arguments;
		EvalASTVector(runtime, node_arguments, arguments);

		auto result = 0.0;
		for (const Atom& item : arguments)
		{
			result += item.As<Number>();
		}

		return result;
	};

	static SystemFunction Subtraction = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Atom
	{
		AtomVector arguments;
		EvalASTVector(runtime, node_arguments, arguments);

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

	static SystemFunction Multiplication = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Atom
	{
		AtomVector arguments;
		EvalASTVector(runtime, node_arguments, arguments);

		auto result = 1.0;
		for (const Atom& item : arguments)
		{
			result *= item.As<Number>();
		}

		return result;
	};

	static SystemFunction Division = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Atom
	{
		AtomVector arguments;
		EvalASTVector(runtime, node_arguments, arguments);

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

	static SystemFunction And = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Atom
	{
		if (node_arguments.empty())
		{
			return true;
		}

		for (unsigned int i = 0; i < node_arguments.size() - 1; i++)
		{
			auto&& argument = node_arguments[i]->Eval(runtime);

			if (argument.IsBool() && !argument.As<Boolean>())
			{
				return false;
			}
		}

		return node_arguments[node_arguments.size() - 1]->Eval(runtime);
	};

	static SystemFunction Or = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Atom
	{
		if (node_arguments.empty())
		{
			return false;
		}

		for (unsigned int i = 0; i < node_arguments.size() - 1; i++)
		{
			auto&& argument = node_arguments[i]->Eval(runtime);

			if (argument.IsBool() && !argument.As<Boolean>())
			{
				continue;
			}

			return std::move(argument);
		}

		return node_arguments[node_arguments.size() - 1]->Eval(runtime);
	};

	static SystemFunction Not = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Atom
	{
		if (node_arguments.empty())
		{
			throw wrong_argument_size();
		}

		auto&& argument = node_arguments[0]->Eval(runtime);

		if (argument.IsBool() && !argument.As<Boolean>())
		{
			return true;
		}

		return false;
	};

	static SystemFunction Equal = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Atom
	{
		AtomVector arguments;
		EvalASTVector(runtime, node_arguments, arguments);

		if (arguments.size() != 2)
		{
			throw wrong_argument_size();
		}

		return arguments[0] == arguments[1];
	};

	static SystemFunction NotEqual = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Atom
	{
		AtomVector arguments;
		EvalASTVector(runtime, node_arguments, arguments);

		if (arguments.size() != 2)
		{
			throw wrong_argument_size();
		}

		return arguments[0] != arguments[1];
	};

	static SystemFunction More = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Atom
	{
		AtomVector arguments;
		EvalASTVector(runtime, node_arguments, arguments);

		if (arguments.size() != 2)
		{
			throw wrong_argument_size();
		}

		return arguments[0] > arguments[1];
	};

	static SystemFunction MoreEqual = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Atom
	{
		AtomVector arguments;
		EvalASTVector(runtime, node_arguments, arguments);

		if (arguments.size() != 2)
		{
			throw wrong_argument_size();
		}

		return arguments[0] >= arguments[1];
	};

	static SystemFunction Less = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Atom
	{
		AtomVector arguments;
		EvalASTVector(runtime, node_arguments, arguments);

		if (arguments.size() != 2)
		{
			throw wrong_argument_size();
		}

		return arguments[0] < arguments[1];
	};

	static SystemFunction LessEqual = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Atom
	{
		AtomVector arguments;
		EvalASTVector(runtime, node_arguments, arguments);

		if (arguments.size() != 2)
		{
			throw wrong_argument_size();
		}

		return arguments[0] <= arguments[1];
	};
}

#endif
