#pragma once

#include "equation.hpp"
#include "solving\solving_common.hpp"
#include "components\component.hpp"
#include "components\monomial.hpp"

#include <string>
#include <sstream>

#pragma warning(disable : 4302 4311)


// This class generates a string representation of an equation.
// Note that the equation isn't validated here, it can be incorrect,
// but then proper formatting isn't guaranteed.
class Printer {
private:
	Equation* equation;
	bool useSpaces = false;

public:
	Printer(Equation* equation) : equation(equation) {};

	// Sets whether a string representation should
	// contain spaces between operations and components.
	// Default state is 'false'.
	void setHasSpaces(bool hasSpaces);

	// Generates a string representation of the given equation.
	std::string toString() const;

private:
	std::ostringstream format(pElem elements) const;
};