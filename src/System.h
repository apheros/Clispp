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
		{ string("=="), Equal },
		{ string("!="), NotEqual },
		{ string(">"), More },
		{ string("<"), Less },
		{ string(">="), MoreEqual },
		{ string("<="), LessEqual },
		{ string("and"), And },
		{ string("or"), Or },
		{ string("not"), Not },
		{ string("null?"), IsNumber },
		{ string("atom?"), IsNumber },
		{ string("list?"), IsList },
		{ string("number?"), IsNumber },
		{ string("bool?"), IsNumber },
		{ string("string?"), IsNumber },
		{ string("symbol?"), IsSymbol},
		{ string("car"), Car },
		{ string("cdr"), Cdr },
		{ string("quote"), Quote },
		{ string("define"), Define },
		{ string("Let"), Let },
		{ string("lambda"), Lambda },
		{ string("cond"), Cond },
		{ string("else"), Else },
		{ string("if"), If },

	};
}

