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

		for (auto snd = fst + 2; snd != elements->end(); snd++) {
			// check operation on the left side
			Element* opElement = *(snd - 1);
			if (opElement->type != ElementType::OPERATION) {
				continue;
			}

			OperationType lType = (OperationType) reinterpret_cast<int>(opElement->pointer);
			if (lType != OperationType::ADD && lType != OperationType::SUBTRACT) {
				// no add/subtract operation on the left
				continue;
			}

			bool valid = true;
			if (snd + 1 != elements->end()) {
				Element* rElement = *(snd + 1);
				OperationType rType = (OperationType) reinterpret_cast<int>(rElement->pointer);
				if (rType != OperationType::ADD && rType != OperationType::SUBTRACT) {
					valid = false;
				}
			}

			if (!valid) {
				continue;
			}

			Component* leftComp = static_cast<Component*>(left->pointer);
			Component* rightComp = static_cast<Component*>((*snd)->pointer);
			bool success = false;

			if (lType == OperationType::ADD
				&& leftComp->isCompatible(rightComp, OperationType::ADD)) {
				leftComp->operator+=(*rightComp);
				delete rightComp;
				success = true;
			}
			else if (lType == OperationType::SUBTRACT
				&& leftComp->isCompatible(rightComp, OperationType::SUBTRACT)) {
				leftComp->operator-=(*rightComp);
				delete rightComp;
				success = true;
			}

			if (success) {
				auto eraseIt = snd - 1;
				elements->erase(eraseIt + 1);
				elements->erase(eraseIt);
				return true;
			}
		}
	}

	return false;
}
