#pragma once
#include <typeinfo>


class bad_any_cast : public std::bad_cast
{
	char const* what() const override
	{
		return "bad_any_cast: failed conversion from any";
	}
};

class wrong_type : public std::bad_typeid
{
	char const* what() const override
	{
		return "wrong_type: wrong accept type";
	}
};

class wrong_syntax : public exception
{
	char const* what() const override
	{
		return "wrong_syntax: syntax isnot correct";
	}
};