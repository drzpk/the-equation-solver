#pragma once

#include "..\equation.hpp"
#include "..\components\component.hpp"
#include "solving_common.hpp"

#include "reducer.hpp"
#include "flipper.hpp"

// DEBUG
#include "..\printer.hpp"
#include <iostream>

#include <queue>
#include <vector>


// SOLVING ALGORITHM
// Solving algorithm consists of several steps (or trials)
// applied in the specific order.
// This order is described below.
// 1. Reduction: storing the equation in the simples form possible.
// 2. Flipping: swapping left side of the equation with right side,
//    when all variables are on one side.
// 3. Arrangement: performing various operation so as a result
//    there's only one variable on only one side of the equation.
// 4. Skipping: bypassing parentheses in various ways.
//
// If none of the trials described above works, the equation
// is checked whether it's a polymonial equation. 


class Solver {
private:
	Equation* equation;
	bool solved;

	Reducer reducer;
	Flipper flipper;

public:
	Solver(Equation* equation) : equation(equation) {};

	// Tries to solve the given equation. Returns true when
	// a trial was successfull, false otherwise.
	bool solve();

	// Returns an array containing results.
	// The single result is returned as an equation:
	// x = <result>
	// When 'x' is the desired variable.
	Equation** getResult();
};
