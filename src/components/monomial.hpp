#pragma once

#include "component.hpp"

#include <string>

#define CAST(other) dynamic_cast<const Monomial&>(other)


// This class represents a monomial, which can be described
// as the following expression: ax^n, where:
//   * 'a' is a required value
//   * 'x' is an indeterminate
//   * 'n' is a power of 'x' (default 1)
class Monomial : public Component {
public:
	int pow;
	char letter;

	Monomial(float a) : Monomial(a, 0, 0) {};
	Monomial(float a, int pow) : Monomial(a, pow, 'x') {};
	Monomial(float a, int pow, char letter);

	bool isCompatible(Component* other, OperationType type) const;
	const char* getName() const;

	Component& operator+=(const Component& other);
	Component& operator-=(const Component& other);
	Component& operator*=(const Component& other);
	Component& operator/=(const Component& other);

	Component* clone() const;

protected:
	std::string toString() const;
};