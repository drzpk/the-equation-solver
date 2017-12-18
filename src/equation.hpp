#pragma once

#include "components\component.hpp"
#include "components\monomial.hpp"
#include "operation_types.hpp"
#include "solving\solving_common.hpp"

#include <algorithm>
#include <regex>
#include <stdexcept>
#include <string>
#include <string.h>
#include <vector>

#define IS_OPERATION(c) (c == '+' || c == '-' || c == '*' || c == '/' || c == '=')

#pragma warning (disable : 4302 4311)

class Solver;


// This class is used for storing equations.
class Equation {
private:
	std::vector<Element*>* left;
	std::vector<Element*>* right;

	char variableName = 0;

	static std::regex monomialRegex;
public:
	// Creates new equation from string source.
	// If an error occurs, constructor will throw
	// an invalid_argument exception.
	Equation(const char* source);
	~Equation();

	// Creates a new copy of the given equation.
	Equation(Equation* other);

	pElem getLeft() const;
	pElem getRight() const;

private:
	pElem parse(const std::string& source, size_t start, size_t end);
	void processComponent(pElem elements, const std::string& buffer, size_t pos);
	Component* parseComponent(const std::string& source, size_t pos);

	pElem cloneElements(pElem elements);
	void deleteElements(pElem elements);

	// Allow this class change internal pointers.
	friend class Flipper;
};