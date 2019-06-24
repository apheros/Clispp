#pragma once
#include "Math.h"
#include "Function.h"


namespace System
{
	static AtomAtomMap STATIC_SYMBOL_STACK = {
		{ string("+"), Addition },
		{ string("-"), Subtraction },
		{ string("*"), Multiplication },
		{ string("/"), Division },
		{ string("define"), Define },
		{ string("lambda"), Lambda },
		{ string("cond"), Cond },
		{ string("else"), Else },
		{ string("if"), If },
		{ string("null?"), IsNumber },
		{ string("atom?"), IsNumber },
		{ string("list?"), IsNumber },
		{ string("number?"), IsNumber },
		{ string("bool?"), IsNumber },
		{ string("string?"), IsNumber },
	};
}

