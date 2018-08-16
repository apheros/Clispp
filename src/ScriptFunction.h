#pragma once
#ifndef _H_IFUNCTION_H_
#define _H_IFUNCTION_H_
#include "Common.h"
#include "Any.h"
#include "Runtime.h"
#include "AST.h"


namespace SystemFunction
{
	inline void EvalASTVevtor(Runtime* runtime, ASTNodeVector& node_arguments, AnyVector& result_vector)
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

	static ScriptFunction Addition = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Any
	{
		AnyVector arguments;
		EvalASTVevtor(runtime, node_arguments, arguments);

		auto result = 0.0;
		for (const Any& item : arguments)
		{
			result += item.As<Number>();
		}

		return result;
	};

	static ScriptFunction Subtraction = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Any
	{
		AnyVector arguments;
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

	static ScriptFunction Multiplication = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Any
	{
		AnyVector arguments;
		EvalASTVevtor(runtime, node_arguments, arguments);

		auto result = 1.0;
		for (const Any& item : arguments)
		{
			result *= item.As<Number>();
		}

		return result;
	};

	static ScriptFunction Division = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Any
	{
		AnyVector arguments;
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

	static ScriptFunction Define = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Any
	{
		AnyVector arguments;
		EvalASTVevtor(runtime, node_arguments, arguments);

		if (arguments.size() < 2)
		{
			throw wrong_argument_size();
		}

		runtime->AddSymbol(std::move(arguments[0]), std::move(arguments[1]));

		return Any::EmptyValue();
	};

	class LambdaFunction : public ScriptFunction
	{
	public:
		LambdaFunction(AnyVector& arguments, ASTNode* function_root)
			:_argument_list(arguments)
			,_function_node(function_root)
		{
		}

		LambdaFunction(LambdaFunction&& other) noexcept
			:_function_node(nullptr)
		{
			*this = std::move(other);
		}

		LambdaFunction(const LambdaFunction& other)
			:_argument_list(other._argument_list)
			,_function_node(other._function_node)
		{
		}

		~LambdaFunction()
		{
			_argument_list.clear();
			_function_node = nullptr;
		}

		LambdaFunction& operator=(LambdaFunction&& other) noexcept
		{
			_argument_list = other._argument_list;
			_function_node = other._function_node;

			return *this;
		}

		LambdaFunction& operator=(const LambdaFunction& other) noexcept
		{
			_argument_list = other._argument_list;
			_function_node = other._function_node;

			return *this;
		}

		Any operator()(Runtime* runtime, ASTNodeVector& node_arguments)
		{
			AnyVector arguments;
			EvalASTVevtor(runtime, node_arguments, arguments);

			if (_argument_list.size() != arguments.size())
			{
				throw wrong_argument_size();
			}

			for (size_t i = 0; i < _argument_list.size(); i++)
			{
				auto symbol_name = _argument_list[i];
				runtime->AddSymbol(std::move(symbol_name), std::move(arguments[i]));
			}

			return _function_node->Eval(runtime);
		}

	private:
		AnyVector _argument_list;
		ASTNode* _function_node;
	};

	static ScriptFunction Lambda = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Any
	{
		if (node_arguments.size() != 3)
		{
			throw wrong_argument_size();
		}

		auto* node = dynamic_cast<ASTListNode*>(node_arguments[1]);
		if (node == nullptr)
		{
			throw wrong_type();
		}

		AnyVector arguments;

		auto& arguments_list = node->GetArguments();
		for (auto* argument : arguments_list)
		{
			if (argument == nullptr)
			{
				continue;
			}

			arguments.push_back(argument->Eval(runtime));
		}

		return static_cast<ScriptFunction>([=](Runtime* run_time, ASTNodeVector& node_argument_list) -> Any
		{
			AnyVector argument_list;
			EvalASTVevtor(run_time, node_argument_list, argument_list);

			if (argument_list.size() != arguments.size())
			{
				throw wrong_argument_size();
			}

			for (size_t i = 0; i < arguments.size(); i++)
			{
				Any symbol_name = arguments[i];
				run_time->AddSymbol(std::move(symbol_name), std::move(argument_list[i]));
			}

			return node_arguments[2]->Eval(runtime);
		});
	};
}

#endif
