#include "skipper.hpp"

static std::vector<OperationType> allowed_operations = { OperationType::ADD, OperationType::SUBTRACT };

bool Skipper::skip(Equation* equation) {
	if (search(equation->getLeft())
		|| search(equation->getRight()))
		return true;

	return false;
}

bool Skipper::search(pElem elements) {
	for (auto it = elements->begin(); it != elements->end(); it++) {
		if ((*it)->type != ElementType::COMPONENT)
			continue;
		
		Component* comp = static_cast<Component*>((*it)->pointer);
		if (strcmp(comp->getName(), "parenthesis"))
			continue;
		Parenthesis* parenthesis = static_cast<Parenthesis*>((*it)->pointer);

		// Check if parentheses can be omitted (whether they are surrounded
		// only by addition or subtraction signs).
		if (!check_surrounding_operations(elements, it, allowed_operations))
			continue;

		int sign = 1;
		if (it != elements->begin()) {
			Element* prev = *(it - 1);
			if (prev->type != ElementType::OPERATION) {
				char buffer[34] = { 0 };
				sprintf_s(buffer, "expected operation sign at %zi", std::distance(elements->begin(), it) + 1);
				throw std::runtime_error(buffer);
			}

			OperationType type = (OperationType) reinterpret_cast<int>(prev->pointer);
			if (type != OperationType::ADD && type != OperationType::SUBTRACT) {
				char buffer[55] = { 0 };
				sprintf_s(buffer, "program error: expected plus or minus sign at %zi", std::distance(elements->begin(), it) + 1);
				throw std::runtime_error(buffer);
			}
			
			if (type == OperationType::SUBTRACT)
				sign = -1;
		}

		// Apply the sign
		for_each_component(parenthesis->contents, [sign](Component* comp) { comp->value *= sign; });

		// LOOK FOR MEMORY MANAGEMENT ERROR BELOW

		// Remove parentheses with the sign
		std::vector<Element*>::iterator newPos;
		if (it != elements->begin())
			newPos = elements->erase(it - 1, it + 1);
		else
			newPos = elements->erase(it);

		// Insert contents of the parentheses (copy of pointers)
		elements->insert(newPos, parenthesis->contents->begin(), parenthesis->contents->end());
		
		// Clear the parenthesis' contents so no components will be deleted
		// when the object itself is.
		parenthesis->contents->clear();
		delete parenthesis;
		
		return true;
	}

	return false;
}
