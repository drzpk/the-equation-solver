#pragma once

#include "component.hpp"
#include "..\solving\solving_common.hpp"
#include "..\components\monomial.hpp"


// Representation of parenthesis
class Parenthesis : public Component {
public:
	pElem contents;
	
	Parenthesis(pElem contents) : contents(contents) {}
	~Parenthesis();

	bool isCompatible(Component* other, OperationType type) const;
	const char* getName() const;

	Component& operator+=(const Component& other);
	Component& operator-=(const Component& other);
	Component& operator*=(const Component& other);
	Component& operator/=(const Component& other);

	Component* clone() const;

private:
	void handleOperations(const Component& other, bool mul);
	pElem dmOneParenthesis(const Monomial* monomial, bool mul);
	pElem dmTwoParenthesis(const Parenthesis* other, bool mul);
};