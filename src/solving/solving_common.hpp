#pragma once

#include "..\operation_types.hpp"
#include "..\components\component.hpp"

#include <vector>
#include <stdexcept>
#include <functional>

// Holds available types of the equation elements.
enum ElementType {
	// Element is component
	COMPONENT,
	// Element is operation
	OPERATION
};

// Holds elements of the equation
struct Element {
	Element() = default;
	Element(Element &other);
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
};

typedef std::vector<Element*>* pElem;
typedef std::vector<Element*>::iterator pElem_iterator;


// Returns whether operation on the left and on the right
// is in the list.
bool check_surrounding_operations(pElem elements, std::vector<Element*>::iterator it,
	std::vector<OperationType>& allowed_operations);

// Returns a sign (plus or minus) of the given element.
// Note: this function only checks given element and optionally
// a sign before it, so a reduction must be performed first!
// Example:
// -(-a) returns 1
// +(-a) returns -1
int get_sign(pElem elements, pElem_iterator it);

// Executes given function on each element.
void for_each_component(pElem elements, std::function<void(Component*)> fun);