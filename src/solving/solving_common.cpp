#include "solving_common.hpp"

bool check_surrounding_operations(pElem elements, std::vector<Element*>::iterator it,
	std::vector<OperationType>& allowed_operations) {
	// Left side
	if (it != elements->begin()) {
		Element* left = *(it - 1);
		if (left->type != ElementType::OPERATION) {
			return false;
		}

		OperationType left_op = (OperationType) reinterpret_cast<int>(left->pointer);
		if (std::find(allowed_operations.cbegin(), allowed_operations.cend(), left_op)
			== allowed_operations.cend()) {
			return false;
		}
	}

	// Right side
	if ((it + 1) != elements->end()) {
		Element* right = *(it + 1);
		if (right->type != ElementType::OPERATION) {
			return false;
		}

		OperationType right_op = (OperationType) reinterpret_cast<int>(right->pointer);
		if (std::find(allowed_operations.cbegin(), allowed_operations.cend(), right_op)
			== allowed_operations.cend()) {
			return false;
		}
	}

	return true;
}

int get_sign(pElem elements, pElem_iterator it) {
	Element* element = *it;
	if (element->type != ElementType::COMPONENT) {
		// Given iterator doesn't point to a valid component
		return 1;
	}

	int prev_sign = 1;
	if (it != elements->begin()) {
		// Get the sign before the element
		auto prev = *(it - 1);
		if (prev->type == ElementType::OPERATION) {
			OperationType operation = (OperationType) reinterpret_cast<int>(prev->pointer);
			if (operation == OperationType::SUBTRACT)
				prev_sign = -1;
		}
	}

	Component* component = static_cast<Component*>(element->pointer);
	int sign = component->getValue() >= 0 ? 1 : -1;

	return sign * prev_sign;
}