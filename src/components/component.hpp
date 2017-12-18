#pragma once

#include <sstream>
#include <string>
#include <string.h>

#include "..\operation_types.hpp"

// Component is the most basic part of the equation.
// It can be used as the left or right side in one
// of four operations.
class Component {
public:
	float value;

	// Returns component's value.
	virtual float getValue() const;

	// Checks if the other component is compatible
	// with this component. Components are considered
	// compatible, if an operation can be performed on them.
	// By default, only component names are checked, however
	// this behavior can be changed by overriding this method.
	virtual bool isCompatible(Component* other, OperationType type) const;

	// Returns component's name.
	virtual const char* getName() const = 0;

	// Below are defined four types of basic operations.

	virtual Component& operator+=(const Component& other);
	virtual Component& operator-=(const Component& other);
	virtual Component& operator*=(const Component& other);
	virtual Component& operator/=(const Component& other);

	// Clones component.
	virtual Component* clone() const = 0;

protected:
	virtual std::string toString() const;

	friend std::ostream& operator<<(std::ostream& ostream, const Component& component);
	friend std::ostream& operator<<(std::ostream& ostream, Component* component);
};