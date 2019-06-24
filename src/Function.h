#pragma once
#ifndef _H_SYSTEM_FUNCTION_H_
#define _H_SYSTEM_FUNCTION_H_
#include "TypeDefine.h"
#include "Atom.h"
#include "Runtime.h"
#include "AST.h"


namespace System
{
	static SystemFunction Define = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Atom
	{
		AtomVector arguments;
		EvalASTVevtor(runtime, node_arguments, arguments);

		if (arguments.size() < 2)
		{
			throw wrong_argument_size();
		}

		runtime->AddGlobalymbol(std::move(arguments[0]), std::move(arguments[1]));

		return Atom::NONE();
	};

	static SystemFunction Lambda = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Atom
	{
		if (node_arguments.size() != 3)
		{
			throw wrong_argument_size();
		}

		return static_cast<SystemFunction>([lambda_node_list = node_arguments](Runtime* runtime, ASTNodeVector& symbol_value_node_list) -> Atom
		{
			auto* symbol_name_list_node = dynamic_cast<ASTListNode*>(lambda_node_list[1]);
			if (symbol_name_list_node == nullptr)
			{
				throw wrong_type();
			}

			AtomVector symbol_name_list;

			auto& symbol_name_node_list = symbol_name_list_node->GetArguments();
			for (auto* symbol_name_node : symbol_name_node_list)
			{
				if (symbol_name_node == nullptr)
				{
					continue;
				}

				symbol_name_list.push_back(symbol_name_node->Eval(runtime));
			}

			AtomVector symbol_value_list;
			EvalASTVevtor(runtime, symbol_value_node_list, symbol_value_list);

			if (symbol_value_list.size() != symbol_name_list.size())
			{
				throw wrong_argument_size();
			}

			for (size_t i = 0; i < symbol_name_list.size(); i++)
			{
				runtime->AddLocalSymbol(symbol_name_list[i], std::move(symbol_value_list[i]));
			}

			auto&& result = lambda_node_list[2]->Eval(runtime);

			for (const auto& symbol_name : symbol_name_list)
			{
				runtime->RemoveSymbol(symbol_name);
			}

			return result;
		});
	};

	static SystemFunction Cond = [](Runtime* runtime, ASTNodeVector& node_arguments) -> Atom
	{
		if (node_arguments.size() < 3)
		{
			throw wrong_argument_size();
		}

		const auto size = node_arguments.size();
		for (unsigned int i = 1; i < size - 1; i++)
		{
			ASTNode* node = node_arguments[i];
			if (node == nullptr || node->Type() != TYPE_LIST)
			{
				throw wrong_type();
			}

			auto* list_node = dynamic_cast<ASTListNode*>(node);
			if (list_node == nullptr)
			{
				throw wrong_type();
			}

			ASTNodeVector& list_node_arguments = list_node->GetArguments();
			if (list_node_arguments.size() != 2)
			{
				throw wrong_argument_size();
			}

			ASTNode* node_condition = list_node_arguments[0];
			if (node_condition == nullptr)
			{
				throw value_is_null();
			}

			Atom&& result = node_condition->Eval(runtime);
			if (result.IsBool() && result.As<Boolean>())
			{
				ASTNode* node_expression = list_node_arguments[1];
				if (node_expression == nullptr)
				{
					throw value_is_null();
				}

				return node_expression->Eval(runtime);
			}
		}

		//else
		ASTNode* node_else = node_arguments[size - 1];
		if (node_else == nullptr || node_else->Type() != TYPE_LIST)
		{
			throw wrong_type();
		}

		return node_else->Eval(runtime);
	};

	static SystemFunction Else = [](Runtime* runtime, ASTNodeVector& node_arguments)->Atom
	{
		if (node_arguments.size() != 2)
		{
			throw wrong_argument_size();
		}

		ASTNode* node_expression = node_arguments[1];
		if (node_expression == nullptr)
		{
			throw value_is_null();
		}

		return node_expression->Eval(runtime);
	};

	static SystemFunction If = [](Runtime* runtime, ASTNodeVector& node_arguments)->Atom
	{
		if (node_arguments.size() != 4)
		{
			throw wrong_argument_size();
		}

		ASTNode* node_condition = node_arguments[1];
		if (node_condition == nullptr)
		{
			throw value_is_null();
		}

		auto&& result = node_condition->Eval(runtime);

		ASTNode* node_expression;
		if (result.IsBool() && result.As<Boolean>())
		{
			node_expression = node_arguments[2];
		}
		else
		{
			node_expression = node_arguments[3];
		}

		if (node_expression == nullptr)
		{
			throw value_is_null();
		}

		return node_expression->Eval(runtime);
	};

	static SystemFunction IsNull = [](Runtime* runtime, ASTNodeVector& node_arguments)->Atom
	{
		if (node_arguments.size() < 2)
		{
			throw wrong_argument_size();
		}

		ASTNode* node = node_arguments[1];
		if (node == nullptr)
		{
			throw value_is_null();
		}

		return node->Type() == TYPE_NONE;
	};

	static SystemFunction IsList = [](Runtime* runtime, ASTNodeVector& node_arguments)->Atom
	{
		if (node_arguments.size() < 2)
		{
			throw wrong_argument_size();
		}

		ASTNode* node = node_arguments[1];
		if (node == nullptr)
		{
			throw value_is_null();
		}

		return node->Type() == TYPE_LIST;
	};

	static SystemFunction IsAtom = [](Runtime* runtime, ASTNodeVector& node_arguments)->Atom
	{
		if (node_arguments.size() < 2)
		{
			throw wrong_argument_size();
		}

		ASTNode* node = node_arguments[1];
		if (node == nullptr)
		{
			throw value_is_null();
		}

		return node->Type() == TYPE_CONST || node->Type() == TYPE_SYMBOL;
	};

	static SystemFunction IsSymbol = [](Runtime* runtime, ASTNodeVector& node_arguments)->Atom
	{
		if (node_arguments.size() < 2)
		{
			throw wrong_argument_size();
		}

		ASTNode* node = node_arguments[1];
		if (node == nullptr)
		{
			throw value_is_null();
		}

		return node->Type() == TYPE_SYMBOL;
	};

	static SystemFunction IsNumber = [](Runtime* runtime, ASTNodeVector& node_arguments)->Atom
	{
		if (node_arguments.size() < 2)
		{
			throw wrong_argument_size();
		}

		ASTNode* node = node_arguments[1];
		if (node == nullptr)
		{
			throw value_is_null();
		}

		auto&& result = node->Eval(runtime);

		return result.IsNumber();
	};

	static SystemFunction IsBoolen = [](Runtime* runtime, ASTNodeVector& node_arguments)->Atom
	{
		if (node_arguments.size() < 2)
		{
			throw wrong_argument_size();
		}

		ASTNode* node = node_arguments[1];
		if (node == nullptr)
		{
			throw value_is_null();
		}

		auto&& result = node->Eval(runtime);

		return result.IsBool();
	};

	static SystemFunction IsString = [](Runtime* runtime, ASTNodeVector& node_arguments)->Atom
	{
		if (node_arguments.size() < 2)
		{
			throw wrong_argument_size();
		}

		ASTNode* node = node_arguments[1];
		if (node == nullptr)
		{
			throw value_is_null();
		}

		auto&& result = node->Eval(runtime);

		return result.IsString();
	};
}

#endif
