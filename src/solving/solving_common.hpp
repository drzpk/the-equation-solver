#pragma once

#include "..\operation_types.hpp"

#include <vector>

// Holds available types of the equation elements.
enum ElementType {
	COMPONENT,
	OPERATION,
	ELEMENTS
};

// Holds elements of the equation
struct Element {
	ElementType type;
	void* pointer;
};

typedef std::vector<Element*>* pElem;


// Returns whether operation on the left and on the right
// is in the list.
extern bool check_surrounding_operations(pElem elements, std::vector<Element*>::iterator it,
	std::vector<OperationType>& allowed_operations);