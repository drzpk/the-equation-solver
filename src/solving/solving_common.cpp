#include "solving_common.hpp"

Element::Element(Element &other) {
	type = other.type;
	switch (type) {
	case COMPONENT: {
		Component* comp = static_cast<Component*>(pointer);
		pointer = comp->clone();
		break;
	}
	case OPERATION: {
		pointer = other.pointer;
	}
	}
}

Element::~Element() {
	// Remove contents, depending on their type
	if (type == ElementType::COMPONENT)
		delete getComponent();
}

Component* Element::getComponent() const {
	if (type != ElementType::COMPONENT)
		throw std::runtime_error("this element doesn't contain acomponent");

	Component* component = reinterpret_cast<Component*>(pointer);
	return component;
}

OperationType Element::getOperation() const {
	if (type != ElementType::OPERATION)
		throw std::runtime_error("this element doesn't contain an operation type");

#pragma warning(suppress : 4311 4302)
	OperationType operation = (OperationType) reinterpret_cast<int>(pointer);
	return operation;
}

bool check_surrounding_operations(pElem elements, std::vector<Element*>::iterator it,
	std::vector<OperationType>& allowed_operations) {
	// Left side
	if (it != elements->begin()) {
		Element* left = *(it - 1);
		if (left->type != ElementType::OPERATION) {
			return false;
		}

		OperationType left_op = left->getOperation();
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

		OperationType right_op = right->getOperation();
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
			OperationType operation = prev->getOperation();
			if (operation == OperationType::SUBTRACT)
				prev_sign = -1;
		}
	}

	Component* component = static_cast<Component*>(element->pointer);
	int sign = component->getValue() >= 0 ? 1 : -1;

	return sign * prev_sign;
}

void for_each_component(pElem elements, std::function<void(Component*)> fun) {
	for (auto elem : *elements) {
		if (elem->type != ElementType::COMPONENT)
			continue;

		Component* comp = elem->getComponent();
		fun(comp);
	}
}