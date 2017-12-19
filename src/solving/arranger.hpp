#pragma once

#include "..\equation.hpp"
#include "solving_common.hpp"
#include "reducer.hpp"
#include "..\operation_types.hpp"
#include "..\components\component.hpp"
#include "..\components\monomial.hpp"

#include <string.h>


// This class' task is to arrange elements in the equation.
// Arrangement is moving compatible components around,
// so that in the end on one side there's only a variable,
// and on the other - the rest.
// LIST OF STEPS:
// 1. Choose optimal side - choose a side into which
//    variables will be moved.
// 2. Move components around - add or subtract components
//    so in the end on the optimal side there will be
//    only variables.
// 3. Multiply or divide - when are components are in right places,
//    reduce their numeric parts to one.
class Arranger {
private:
	// Sides of the equation
	pElem optimal, another;
	// The equation
	Equation* equation;
	// Valid surroundings for moving components between sides of the equation
	static std::vector<OperationType> validSurroundings;
	// A reducer instance
	Reducer reducer;

public:
	// Tries to arrange the given equation. Returns
	// true or false, depending on the result.
	bool arrange(Equation* equation);

private:
	// Step #1
	short getOptimalSide(Equation* equation);
	int getRank(pElem elements);

	// Step #2
	bool tryMove();
	// Returns pointer if element can be moved and is a monomial.
	Monomial* canBeMoved(pElem side, pElem_iterator it);

	// Step #3
	bool tryMulDiv();
};