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
	}

	~SymbolTree()
	{

	}

public:
	void EnterScope()
	{
		if (_is_enter_scope)
		{
			if (_root->_children.empty())
			{
				auto* child = _root->CreateChild();
				_root = child;
			}
			else
			{
				auto* child = _root->_children.front();
				if (child == nullptr)
				{
					child = _root->CreateChild();
					_root->_children[0] = child;
				}

				_root = child;
			}
		}
		else
		{
			if (_root->_right == nullptr)
			{
				_root->_right = new SymbolTreeNode;
				_root->_right->_parent = _root->_parent;
				_root->_right->_left = _root;
			}

			_root = _root->_right;
		}

		_is_enter_scope = true;
	}

	void LeaveScope()
	{
		if (_root == nullptr)
		{
			return;
		}

		if (_is_enter_scope)
		{
			return;
		}

		_root = _root->_parent;

		_is_enter_scope = false;
	}

	void AddValue(const Atom& name, Atom& value) const
	{
		if (_root == nullptr)
		{
			throw value_is_null();
		}

		_root->AddValue(name, value);
	}

	Atom GetValue(const Atom& name) const
	{
		return _GetValue(_root, name);
	}

	void RemoveValue(const Atom& name) const
	{
		if (_root == nullptr)
		{
			throw value_is_null();
		}

		_root->RemoveValue(name);
	}

private:
	Atom _GetValue(SymbolTreeNode* root, const Atom& name) const
	{
		if (root == nullptr)
		{
			throw value_is_null();
		}

		auto& result = root->GetValue(name);
		if (result.IsNull() && root->_parent != nullptr)
		{
			_GetValue(_root->_parent, name);
		}

		return result;
	}

private:
	SymbolTreeNode* _root;
	bool _is_enter_scope;
};