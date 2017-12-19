#pragma once

#include "..\operation_types.hpp"
#include "..\components\component.hpp"

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
typedef std::vector<Element*>::iterator pElem_iterator;


// Returns whether operation on the left and on the right
// is in the list.
extern bool check_surrounding_operations(pElem elements, std::vector<Element*>::iterator it,
	std::vector<OperationType>& allowed_operations);

// Returns a sign (plus or minus) of the given element.
// Note: this function only checks given element and optionally
// a sign before it, so a reduction must be performed first!
extern int get_sign(pElem elements, pElem_iterator it);
