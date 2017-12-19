#pragma once

#include "..\components\component.hpp"
#include "..\equation.hpp"
#include "solving_common.hpp"
#include "..\operation_types.hpp"

#include <stdexcept>

#pragma warning (disable : 4302 4311)


// This class' task is to reduce and sort given equation.
// Reducing is storing the equation in the simplest possible form.
// For example, the sentence '5 * 3 * x' can be written as '15x'.
// Only one reduction is done at a time.
class Reducer {
public:
	// Tries to reduce the given equation.
	// Returns true is the equation was reduced,
	// false otherwise.
	bool reduce(Equation* equation);

private:
	// Approach #1
	bool tryMulDiv(pElem elements);

	// Approach #2
	bool tryAddSub(pElem elements);
	// Change signs to more suitable representation for reduction.
	void normalizeSigns(OperationType& type, Element* operationElement, Component* operand);
};