#pragma once
#include <typeinfo>
#include <string>


class bad_any_cast : public std::bad_cast
{
public:
	bad_any_cast(std::string type_name)
		:type_name(std::move(type_name))
	{
	}

	char const* what() const override
	{
		return ("bad_any_cast: failed conversion from any to " + type_name).c_str();
	}

private:
	std::string type_name;
};

class value_is_null : public std::exception
{
	char const* what() const override
	{
		return "value is Null";
	}
};

class wrong_type : public std::exception
{
	char const* what() const override
	{
		return "wrong_type: wrong accept type";
	}
};

class wrong_syntax : public std::exception
{
public:
	wrong_syntax(int line_number, int column_number)
		:line_number(line_number)
		,column_number(column_number)
	{
	}

	char const* what() const override
	{
		return ("wrong_syntax: syntax isnot correct in line: " + std::to_string(line_number) + " column: " + std::to_string(column_number)).c_str();
	}

private:
	int line_number;
	int column_number;
};

class no_operator : public std::exception
{
public:
	no_operator(std::string operator_name)
		:operator_name(std::move(operator_name))
	{
	}

	char const* what() const override
	{
		return ("type has no operator " + operator_name + "defind").c_str();
	}

private:
	std::string operator_name;
};

class cannot_find_symbol : public std::exception
{
	char const* what() const override
	{
		return "Cannot Find Symbol In Scope";
	}
};

class wrong_argument_size : public std::exception
{
	char const* what() const override
	{
		return "Wrong Argument Size";
	}
};