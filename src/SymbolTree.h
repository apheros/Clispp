#pragma once
#include <vector>
#include "Atom.h"


class SymbolTree;
class SymbolTreeNode
{
public:
	SymbolTreeNode()
		:_parent(nullptr)
		,_child(nullptr)
		,_left(nullptr)
		,_right(nullptr)
	{
		_symbol_value_map.clear();
	}

	~SymbolTreeNode()
	{
		_parent = nullptr;
		_left = nullptr;
		_right = nullptr;
		_symbol_value_map.clear();

		auto* temp_child = _child;
		while (temp_child != nullptr)
		{
			_child = _child->_right;

			delete temp_child;

			temp_child = _child;
		}
	}

public:
	Atom& GetValue(const Atom& name)
	{
		const auto iter = _symbol_value_map.find(name);
		if (iter == _symbol_value_map.end())
		{
			Atom::NONE();
		}

		return iter->second;
	}

	void AddValue(const Atom& name, Atom& value)
	{
		_symbol_value_map[name] = value;
	}

	void RemoveValue(const Atom& name)
	{
		const auto iter = _symbol_value_map.find(name);
		if (iter == _symbol_value_map.end())
		{
			return;
		}

		_symbol_value_map.erase(iter);
	}

	SymbolTreeNode* CreateChild()
	{
		auto* child = new SymbolTreeNode;

		child->_parent = this;

		if (_child == nullptr)
		{
			_child = child;
			return child;
		}

		auto* temp_child = _child;
		while (temp_child->_right != nullptr)
		{
			temp_child = temp_child->_right;
		}

		temp_child->_right = child;
		child->_left = temp_child;

		return child;
	}

	void DeleteChild()
	{
		if (_child == nullptr)
		{
			return;
		}

		auto* temp_child = _child;

		_child = _child->_right;

		delete temp_child;
	}

	SymbolTreeNode* GetParent()
	{
		return _parent;
	}

	SymbolTreeNode* GetChild()
	{
		return _child;
	}

	SymbolTreeNode* GetLeft()
	{
		return _left;
	}

	SymbolTreeNode* GetRight()
	{
		return _right;
	}

private:
	SymbolTreeNode* _child;
	SymbolTreeNode* _parent;
	SymbolTreeNode* _left;
	SymbolTreeNode* _right;

	AtomAtomMap _symbol_value_map;
};

class SymbolTree
{
public:
	SymbolTree()
		:_root(nullptr)
	{
		_root = new SymbolTreeNode;
		_current = _root;
	}

	~SymbolTree()
	{
	}

public:
	void EnterScope()
	{
		if (_current == nullptr)
		{
			throw value_is_null();
		}

		auto* child = _current->GetChild();
		if (child == nullptr)
		{
			child = _current->CreateChild();
		}

		_current = child;
	}

	void LeaveScope()
	{
		if (_current == nullptr || _current->GetParent() == nullptr)
		{
			throw value_is_null();
		}

		_current = _current->GetParent();

		_current->DeleteChild();
	}

	void AddGlobalValue(const Atom& name, Atom& value) const
	{
		if (_root == nullptr)
		{
			throw value_is_null();
		}

		_root->AddValue(name, value);
	}

	void AddValue(const Atom& name, Atom& value) const
	{
		if (_current == nullptr)
		{
			throw value_is_null();
		}

		_current->AddValue(name, value);
	}

	Atom& GetValue(const Atom& name) const
	{
		return _GetValue(_current, name);
	}

	void RemoveValue(const Atom& name) const
	{
		if (_current == nullptr)
		{
			throw value_is_null();
		}

		_current->RemoveValue(name);
	}

private:
	Atom& _GetValue(SymbolTreeNode* root, const Atom& name) const
	{
		if (root == nullptr)
		{
			throw value_is_null();
		}

		auto& result = root->GetValue(name);
		if (result.IsNull() && root->GetParent() != nullptr)
		{
			return _GetValue(_current->GetParent(), name);
		}

		return result;
	}

private:
	SymbolTreeNode* _root;
	SymbolTreeNode* _current;
};