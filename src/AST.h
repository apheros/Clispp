#pragma once
#ifndef _H_AST_H_
#define _H_AST_H_
#include "Any.h"
#include "common.h"


class ASTNode
{
public:
	ASTNode() = default;
	virtual ~ASTNode() = default;

public:
	virtual Any Eval() = 0;
};

class ASTSymbolNode final : public ASTNode
{
public:
	ASTSymbolNode(Any&& value)
	{
		_value = value;
	}

	~ASTSymbolNode() = default;

public:
	Any Eval() override
	{
		return _value;
	}

private:
	Any _value;
};

class ASTListNode final : public ASTNode
{
public:
	ASTListNode() = default;
	~ASTListNode()
	{
		for (ASTNode* node : _node_arguments)
		{
			if (node == nullptr)
			{
				continue;
			}

			delete node;
		}

		_node_arguments.clear();
	}

public:
	void AddNode(ASTNode* node)
	{
		_node_arguments.push_back(node);
	}

	Any Eval() override
	{
		if (_node_arguments.empty())
		{
			return Any::EmptyValue();
		}

		auto iter = _node_arguments.begin();
		auto functor_node = *iter;

		if (functor_node == nullptr)
		{
			return Any::EmptyValue();
		}

		const auto& functor_name = functor_node->Eval().Value<string>();
		const auto functor = FunctionRegister::GetInstance()->GetFunction(functor_name);

		if (functor == nullptr)
		{
			return Any::EmptyValue();
		}

		AnyVector arguments;
		for (; iter != _node_arguments.end(); ++iter)
		{
			auto* node = *iter;
			if (node == nullptr)
			{
				continue;
			}

			arguments.push_back(node->Eval());
		}

		return functor(arguments);
	}

private:
	ASTNodeVector _node_arguments;
};

#endif
