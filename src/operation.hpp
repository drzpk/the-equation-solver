#pragma once

#include "operation_types.hpp"
#include "components\component.hpp"

#include <stdexcept>

class Operation;

// This union defines operand types that an operation can accept.
union Operand {
	Operation* operation;
	Component* component;
};

// This class is a base for all types of operations
class Operation {
private:
	Operand left;
	Operand right;

public:
	// Specifies whether left operand is suboperation or a component.
	bool hasLeftSubOperation;
	// Specifies whether right operand is suboperation or a component.
	bool hasRightSubOperation;

	~Operation();

	// Returns if the operation can be performed.
	bool canPerform();

	// Performs the operation and returns its result.
	virtual float perform();

	// Returns left operand.
	Operand getLeft();

	// Returns right operand.
	Operand getRight();

	// Returns type of operation.
	virtual OperationType getType() = 0;
	
	//Returns visual representation of operation.
	virtual char getVisual() = 0;

	// Specifies priority that an operation will have.
	// Operation with higher priority will be performed
	// before operation with lower priority.
	virtual int getPriority() = 0;
};

class Addition : public Operation {
public:
	float perform();
	OperationType getType();
	char getVisual();
	int getPriority();
};

class Subtraction : public Operation {
	float perform();
	OperationType getType();
	char getVisual();
	int getPriority();
};

class Multiplication : public Operation {
	float perform();
	OperationType getType();
	char getVisual();
	int getPriority();
};

class Division : public Operation {
	float perform();
	OperationType getType();
	char getVisual();
	int getPriority();
};

// Equality is special kind of operation.
// It doesn't really perform anything, it's just
// a container for the left and the right side
// of the equation.
class Equality : public Operation {
	bool canPerform();
	float perform();
	OperationType getType();
	char getVisual();
	int getPriority();
};