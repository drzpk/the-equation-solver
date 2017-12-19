#pragma once

#include "..\operation_types.hpp"
#include "..\components\component.hpp"

#include <vector>
#include <stdexcept>

// Holds available types of the equation elements.
enum ElementType {
	// Element is component
	COMPONENT,
	// Element is operation
	OPERATION,
	// Element is parentheses
	ELEMENTS
};

// Holds elements of the equation
struct Element {
	~Element();

	// Type of the data this element contains
	ElementType type;
	// Pointer to the data this element contains (see the ElementType enum)
	void* pointer;

	// Returns a component associated with this element.
	// Throws std::runtime_error if element doesn't contain Component.
	Component* getComponent() const;

	// Returns an operation associated with this element.
	// Throws std::runtime_error if element doesn't contain OperationType.
	OperationType getOperation() const;

	// Returns a contents of parentheses associated with this element.
	// Throws std::runtime_error if element doesn't contain parentheses.
	std::vector<Element*>* getParentheses() const;
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
