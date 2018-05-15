#pragma once

#include "..\components\component.hpp"
#include "..\equation.hpp"
#include "solving_common.hpp"


// Purpose of this class is to omit 
class Skipper {
public:
	bool skip(Equation* equation);

private:
	// Search for suitable components
	bool search(pElem elements);
	// Try to multiply, divide parenthesis by number
	pElem mdByNumber(pElem elements, Monomial* factor, bool mul);
	bool skipParenthesis(pElem elements);
};