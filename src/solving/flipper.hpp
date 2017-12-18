#pragma once

#include "solving_common.hpp"
#include "..\equation.hpp"
#include "..\components\component.hpp"
#include "..\components\monomial.hpp"

#include <string.h>


// This class swaps sides of the equation if variables
// are only on one side. The resulting equation will
// have all variables on the left side. This step
// is aestethic-purpose only.
class Flipper {
public:
	// Tries to flip the equation, as described in
	// the class description. Returns true or false,
	// depending on the result of this operation.
	bool flip(Equation* equation);

private:
	bool hasVariables(pElem elements);
};