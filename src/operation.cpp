#include "operation.hpp"


Operation::~Operation() {
	// delete operands
	
	if (hasLeftSubOperation)
		delete left.operation;
	else
		delete left.component;

	if (hasRightSubOperation)
		delete right.operation;
	else
		delete right.component;
}

bool Operation::canPerform() {
	return !hasLeftSubOperation && !hasRightSubOperation;
}

float Operation::perform() {
	if (!canPerform())
		throw std::logic_error("types of the operands don't allow to perform this operation");

	// This method's only task is to check for logic errors.
	// Actual computation should be programmed in a derived class.
	return 0;
}

Operand Operation::getLeft() {
	return left;
}

Operand Operation::getRight() {
	return right;
}


// ADDITION

float Addition::perform() {
	Operation::perform();

	///

	return 0;
}

OperationType Addition::getType() {
	return OperationType::ADD;
}

char Addition::getVisual() {
	return '+';
}

int Addition::getPriority() {
	return 1;
}


// SUBTRACTION

OperationType Subtraction::getType() {
	return OperationType::SUBTRACT;
}

char Subtraction::getVisual() {
	return '-';
}

int Subtraction::getPriority() {
	return 1;
}


// MULTIPLICATION

OperationType Multiplication::getType() {
	return OperationType::MULTIPLY;
}

char Multiplication::getVisual() {
	return '*';
}

int Multiplication::getPriority() {
	return 2;
}


// DIVISION

OperationType Division::getType() {
	return OperationType::DIVIDE;
}

char Division::getVisual() {
	return '/';
}

int Division::getPriority() {
	return 2;
}


// EQUALITY

bool Equality::canPerform() {
	return false;
}

float Equality::perform() {
	throw std::runtime_error("no operation can be performed on equality");
}

OperationType Equality::getType() {
	return OperationType::EQUALS;
}

char Equality::getVisual() {
	return '=';
}

int Equality::getPriority() {
	return 2;
}