#pragma once
#ifndef _H_AST_H_
#define _H_AST_H_
#include "Atom.h"
#include "Common.h"
#include "Runtime.h"


class ASTNode
{
public:
	ASTNode() = default;
	virtual ~ASTNode() = default;

public:
	virtual Atom Eval(Runtime* runtime) = 0;
	virtual EType Type() = 0;
};

class ASTConstNode final : public ASTNode
{
public:
	ASTConstNode(Atom&& value)
	{
		_value = std::move(value);
	}

	~ASTConstNode() = default;

public:
	Atom Eval(Runtime* runtime) override
	{
		return _value;
	}

	EType Type() override
	{
		return TYPE_CONST;
	}

private:
	Atom _value;
};

class ASTSymbolNode final : public ASTNode
{
public:
	ASTSymbolNode(Atom&& value)
	{
		_value = std::move(value);
	}

	~ASTSymbolNode() = default;

public:
	Atom Eval(Runtime* runtime) override
	{
		if (runtime != nullptr)
		{
			return runtime->GetSymbol(_value);
		}

		return _value;
	}

	EType Type() override
	{
		return TYPE_SYMBOL;
	}

private:
	Atom _value;
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

	ASTNodeVector& GetArguments()
	{
		return _node_arguments;
	}

	Atom Eval(Runtime* runtime) override
	{
		if (_node_arguments.empty())
		{
			return Atom::EmptyValue();
		}

		auto* functor_node = _node_arguments.front();
		if (functor_node == nullptr)
		{
			return Atom::EmptyValue();
		}

		const auto& functor_any = functor_node->Eval(runtime);
		const auto& functor = functor_any.As<ScriptFunction>();

		if (functor == nullptr)
		{
			return Atom::EmptyValue();
		}

		return functor(runtime, _node_arguments);
	}

	EType Type() override
	{
		return TYPE_LIST;
	}

private:
	ASTNodeVector _node_arguments;
};

#endif
