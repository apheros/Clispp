#pragma once
#ifndef _H_IFUNCTION_H_
#define _H_IFUNCTION_H_
#include "common.h"
#include "Any.h"

//
//class ScriptFunction
//{
//public:
//	ScriptFunction() = default;
//	virtual ~ScriptFunction() = default;
//
//public:
//	virtual const string& GetFunctionName() = 0;
//	virtual Any operator()(const AnyVector& arguments) = 0;
//};
//
//class Add : public ScriptFunction
//{
//public:
//	Add() = default;
//	~Add() = default;
//
//public:
//	Any operator()(const AnyVector& arguments) override
//	{
//		double sum = 0;
//
//		for (const Any& item : arguments)
//		{
//			sum += item.Value<double>();
//		}
//
//		return sum;
//	}
//};
//
//class Minus : public ScriptFunction
//{
//public:
//	Minus() = default;
//	~Minus() = default;
//
//public:
//	Any operator()(const AnyVector& arguments) override
//	{
//		double diff = 0;
//
//		for (const Any& item : arguments)
//		{
//			sum += item.Value<double>();
//		}
//
//		return sum;
//	}
//};

static auto Add = [](AnyVector& arguments) -> Any
{
	auto sum = 0;

	for (const Any& item : arguments)
	{
		sum += item.Value<double>();
	}

	return sum;
};

static auto Minus = [](AnyVector& arguments) -> Any
{
	auto diff = arguments[0].Value<double>();

	for (unsigned int i = 1; i < arguments.size() - static_cast<unsigned int>(1); i++)
	{
		diff -= arguments[i].Value<double>();
	}

	return diff;
};

#endif
