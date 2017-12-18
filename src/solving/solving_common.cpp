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
