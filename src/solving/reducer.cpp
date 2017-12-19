#include "reducer.hpp"

bool Reducer::reduce(Equation* equation) {
	if (tryMulDiv(equation->getLeft())) {
		return true;
	}
	if (tryMulDiv(equation->getRight())) {
		return true;
	}
	if (tryAddSub(equation->getLeft())) {
		return true;
	}
	if (tryAddSub(equation->getRight())) {
		return true;
	}

	return false;
}

bool Reducer::tryMulDiv(pElem elements) {
	for (auto it = elements->begin(); it != elements->end(); it++) {
		if ((*it)->type == ElementType::ELEMENTS) {
			pElem subelements = static_cast<pElem>((*it)->pointer);

			// found parentheses
			if (tryMulDiv(subelements)) {
				return true;
			}

			// remove parenthesis, if needed
			if (subelements->size() == 1) {
				(*it) = subelements->at(0);
				delete subelements;
			}
		}
		else if ((*it)->type == ElementType::OPERATION) {
			OperationType opType = (OperationType) reinterpret_cast<int>((*it)->pointer);
			if (opType != OperationType::MULTIPLY && opType != OperationType::DIVIDE) {
				continue;
			}

			Element* prev = *(it - 1);
			Element* next = *(it + 1);
			int comp = ElementType::COMPONENT;

			// this action can be performed only on two components
			if (prev->type != comp || next->type != comp) {
				continue;
			}

			Component* leftComp = static_cast<Component*>(prev->pointer);
			Component* rightComp = static_cast<Component*>(next->pointer);
			bool success = false;

			if (opType == OperationType::MULTIPLY
				&& leftComp->isCompatible(rightComp, OperationType::MULTIPLY)) {
				leftComp->operator*=(*rightComp);
				delete rightComp;
				success = true;
			}
			else if (opType == OperationType::DIVIDE
				&& leftComp->isCompatible(rightComp, OperationType::DIVIDE)) {
				leftComp->operator/=(*rightComp);
				delete rightComp;
				success = true;
			}

			if (success) {
				auto eraseIt = it - 1;
				elements->erase(eraseIt + 2);
				elements->erase(eraseIt + 1);
				return true;
			}
		}
	}

	return false;
}

bool Reducer::tryAddSub(pElem elements) {
	if (elements->size() < 3) {
		// in order to add or subtract something at least
		// two components and one operator is needed
		return false;
	}

	// Search for the left operand
	for (auto fst = elements->begin(); (fst + 2) != elements->end(); fst++) {
		Element* left = *fst;

		if (left->type == ElementType::OPERATION) {
			continue;
		}

		if (left->type == ElementType::ELEMENTS) {
			// found parentheses
			pElem subelements = static_cast<pElem>((*fst)->pointer);

			// found parentheses
			if (tryAddSub(subelements)) {
				return true;
			}

			// remove parenthesis, if needed
			if (subelements->size() == 1) {
				(*fst) = subelements->at(0);
				delete subelements;
			}

			continue;
		}

		// Operation before left operand (+ or -)
		OperationType leftOp;
		Element* leftOpElement = nullptr;

		// Check if left operand is reduced (only + or - on its left side)
		if (fst != elements->begin()) {
			leftOpElement = *(fst - 1);
			if (leftOpElement->type != ElementType::OPERATION)
				continue;

			leftOp = (OperationType) reinterpret_cast<int>(leftOpElement->pointer);
			if (leftOp != OperationType::ADD && leftOp != OperationType::SUBTRACT)
				continue;
		}
		else
			leftOp = OperationType::ADD;

		// Search for the right operand
		for (auto snd = fst + 2; snd != elements->end(); snd++) {
			// check operation on the left side
			Element* rightOpElement = *(snd - 1);
			if (rightOpElement->type != ElementType::OPERATION)
				continue;

			OperationType rightOp = (OperationType) reinterpret_cast<int>(rightOpElement->pointer);
			if (rightOp != OperationType::ADD && rightOp != OperationType::SUBTRACT) {
				// no add/subtract operation on the left
				continue;
			}

			// Check if right operand is reduced (only + or - on its right side)
			if (snd + 1 != elements->end()) {
				Element* rElement = *(snd + 1);
				OperationType rType = (OperationType) reinterpret_cast<int>(rElement->pointer);
				if (rType != OperationType::ADD && rType != OperationType::SUBTRACT)
					continue;
			}

			Component* leftComp = static_cast<Component*>(left->pointer);
			Component* rightComp = static_cast<Component*>((*snd)->pointer);

			// Normalize signs
			if (leftOpElement)
				normalizeSigns(leftOp, leftOpElement, leftComp);
			normalizeSigns(rightOp, rightOpElement, rightComp);

			// Check if components can be added and perform operation
			// Addition and subtraction are parallel operations, so it's enough to check just one
			bool success = false;
			if (leftComp->isCompatible(rightComp, OperationType::ADD)) {
				int leftSign = get_sign(elements, fst);
				int rightSign = get_sign(elements, snd);

				float value = (leftSign * leftComp->value) + (rightSign * rightComp->value);
				leftComp->value = value;
				success = true;
			}

			if (success) {
				// Delete objects
				delete rightComp;
				delete rightOpElement;

				// Remove redundant elements from list
				auto eraseIt = snd - 1;
				elements->erase(eraseIt + 1);
				elements->erase(eraseIt);

				// Normalize signs
				if (leftOpElement)
					normalizeSigns(leftOp, leftOpElement, leftComp);

				// Remove unnecesarry expression: +0/-0
				// This transformation can empty vector. In that case, presentation is left up to printer.
				if (!leftComp->value) {
					if (fst != elements->begin()) {
						// Remove component and its sign
						auto eraseIt = fst - 1;
						elements->erase(eraseIt + 1);
						elements->erase(eraseIt);
						delete leftComp;
						if (leftOpElement)
							delete leftOpElement;
					}
					else {
						// Remove only component
						elements->erase(fst);
						delete leftComp;
					}
				}

				return true;
			}
		}
	}

	return false;
}

void Reducer::normalizeSigns(OperationType& type, Element* operationElement, Component* operand) {
	if (type == OperationType::SUBTRACT && operand->value < 0) {
		// -(-a) => +(a)
		operationElement->pointer = reinterpret_cast<void*>(OperationType::ADD);
		operand->value *= -1.f;
		type = OperationType::ADD;
	}
	else if (type == OperationType::ADD && operand->value < 0) {
		// +(-a) => -(a)
		operationElement->pointer = reinterpret_cast<void*>(OperationType::SUBTRACT);
		operand->value *= -1.f;
		type = OperationType::SUBTRACT;
	}
}
