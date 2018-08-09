#pragma once
#ifndef _H_AST_H_
#define _H_AST_H_
#include "Any.h"
#include "Common.h"
#include "Runtime.h"


class ASTNode
{
public:
	ASTNode() = default;
	virtual ~ASTNode() = default;

public:
	virtual Any Eval(Runtime* runtime) = 0;
};

class ASTConstNode final : public ASTNode
{
public:
	ASTConstNode(Any&& value)
	{
		_value = std::move(value);
	}

	~ASTConstNode() = default;

public:
	Any Eval(Runtime* runtime) override
	{
		return _value;
	}

private:
	Any _value;
};

class ASTSymbolNode final : public ASTNode
{
public:
	ASTSymbolNode(Any&& value)
	{
		_value = std::move(value);
	}

	~ASTSymbolNode() = default;

public:
	Any Eval(Runtime* runtime) override
	{
		if (runtime != nullptr)
		{
			if (_value.Is<string>())
			{
				const string& symbol_name = _value.As<string>();
				return runtime->GetSymbol(symbol_name);
			}
		}

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

	Any Eval(Runtime* runtime) override
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

		const auto& functor_any = functor_node->Eval(runtime);
		const auto& functor = functor_any.As<ScriptFunction>();

		if (functor == nullptr)
		{
			return Any::EmptyValue();
		}

		AnyVector arguments;
		for (++iter; iter != _node_arguments.end(); ++iter)
		{
			auto* node = *iter;
			if (node == nullptr)
			{
				continue;
			}

			arguments.push_back(node->Eval(runtime));
		}

		return functor(arguments);
	}

private:
	ASTNodeVector _node_arguments;
};

#endif
