#pragma once
#include <typeinfo>


class bad_any_cast : public std::bad_cast
{
public:
	bad_any_cast(string type_name)
		:type_name(std::move(type_name))
	{
	}

	char const* what() const override
	{
		return ("bad_any_cast: failed conversion from any to " + type_name).c_str();
	}

private:
	string type_name;
};

class wrong_type : public exception
{
	char const* what() const override
	{
		return "wrong_type: wrong accept type";
	}
};

class wrong_syntax : public exception
{
public:
	wrong_syntax(int line_number, int column_number)
		:line_number(line_number)
		,column_number(column_number)
	{
	}

	char const* what() const override
	{
		return ("wrong_syntax: syntax isnot correct in line: " + to_string(line_number) + " column: " + to_string(column_number)).c_str();
	}

private:
	int line_number;
	int column_number;
};

class no_operator : public exception
{
public:
	no_operator(string operator_name)
		:operator_name(std::move(operator_name))
	{
	}

	char const* what() const override
	{
		return ("type has no operator " + operator_name + "defind").c_str();
	}

private:
	string operator_name;
};

class cannot_find_symbol : public exception
{
	char const* what() const override
	{
		return "Cannot Find Symbol In Scope";
	}
};

class wrong_argument_size : public exception
{
	char const* what() const override
	{
		return "Wrong Argument Size";
	}
};