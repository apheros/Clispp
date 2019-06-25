#pragma once
#include <vector>
#include "Atom.h"


class SymbolTree;
class SymbolTreeNode
{
	friend SymbolTree;

private:
	SymbolTreeNode()
		:_parent(nullptr)
		,_left(nullptr)
		,_right(nullptr)
	{
		_symbol_value_map.clear();
		_children.clear();
	}

	~SymbolTreeNode()
	{
		_parent = nullptr;
		_left = nullptr;
		_right = nullptr;
		_symbol_value_map.clear();
		_children.clear();
	}

private:
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
		_children.push_back(child);

		return child;
	}

private:
	AtomAtomMap _symbol_value_map;
	std::vector<SymbolTreeNode*> _children;
	SymbolTreeNode* _parent;
	SymbolTreeNode* _left;
	SymbolTreeNode* _right;
};

class SymbolTree
{
public:
	SymbolTree()
		:_root(nullptr)
		,_is_enter_scope(false)
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
		if (_is_enter_scope)
		{
			if (_current->_children.empty())
			{
				auto* child = _current->CreateChild();
				_current = child;
			}
			else
			{
				auto* child = _current->_children.front();
				if (child == nullptr)
				{
					child = _current->CreateChild();
					_current->_children[0] = child;
				}

				_current = child;
			}
		}
		else
		{
			if (_current->_right == nullptr)
			{
				_current->_right = new SymbolTreeNode;
				_current->_right->_parent = _current->_parent;
				_current->_right->_left = _current;
			}

			_current = _current->_right;
		}

		_is_enter_scope = true;
	}

	void LeaveScope()
	{
		if (_current == nullptr || _current->_parent == nullptr)
		{
			throw value_is_null();
		}

		if (_is_enter_scope)
		{
			return;
		}

		_current = _current->_parent;

		_is_enter_scope = false;
	}

	void AddValue(const Atom& name, Atom& value) const
	{
		if (_current == nullptr)
		{
			throw value_is_null();
		}

		_current->AddValue(name, value);
	}

	void AddGlobalValue(const Atom& name, Atom& value) const
	{
		if (_root == nullptr)
		{
			throw value_is_null();
		}

		_root->AddValue(name, value);
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
		if (result.IsNull() && root->_parent != nullptr)
		{
			return _GetValue(_current->_parent, name);
		}

		return result;
	}

private:
	SymbolTreeNode* _root;
	SymbolTreeNode* _current;
	bool _is_enter_scope;
};