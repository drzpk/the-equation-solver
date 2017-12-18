#include "arranger.hpp"

bool Arranger::arrange(Equation* equation) {
	if (!equation->getLeft() || !equation->getRight()) {
		// Equation must have two sides in order for this
		// class to work.
		return false;
	}

	if (getOptimalSide(equation) == -1) {
		optimal = equation->getLeft();
		another = equation->getRight();
	}
	else {
		optimal = equation->getRight();
		another = equation->getLeft();
	}

	return false;
}

short Arranger::getOptimalSide(Equation* equation) {
	// Optimal side is returned based on a ranking.
	// Every monomial with variable 
	// (currently only monomials are supported)
	// gets a range based on the following formula:
	// rank = number * (power + 1)
	// Total rank is a sum of the single ranks.
	// Side with higher rank is the optimal one.

	int leftRank = getRank(equation->getLeft());
	int rightRank = getRank(equation->getRight());

	return leftRank >= rightRank ? -1 : 1;
}

int Arranger::getRank(pElem elements) {
	int rank = 0;
	for (auto it = elements->cbegin(); it != elements->cend(); it++) {
		Element* element = *it;
		if (element->type == ElementType::ELEMENTS) {
			pElem subElements = static_cast<pElem>(element->pointer);
			rank += getRank(subElements);
		}

		if (element->type != ElementType::COMPONENT) {
			continue;
		}

		Component* component = static_cast<Component*>(element->pointer);
		if (strcmp(component->getName(), "monomial")) {
			continue;
		}

		Monomial* monomial = dynamic_cast<Monomial*>(component);
		if (!monomial->letter) {
			// Only monomials with letters can be ranked.
			continue;
		}

		rank += (monomial->getValue() * (monomial->pow + 1));
	}

	return rank;
}

bool Arranger::tryMove() {
	// Currently everything's moved based only on the optimal side.
	// This behavior should be changed in the future, so the other
	// side will be also taken into account when moving monomials around.

	std::vector<OperationType> validSurroundings = {
		OperationType::ADD,
		OperationType::SUBTRACT
	};

	for (auto it = optimal->begin(); it != optimal->end(); it++) {
		Element* element = *it;
		if (element->type != ElementType::COMPONENT) {
			// Only components can be checked.
			continue;
		}

		if (!check_surrounding_operations(optimal, it, validSurroundings)) {
			// No moving can be performed when elements aren't
			// multiplied with each other yet.
			continue;
		}
		
		Component* component = static_cast<Component*>(element->pointer);
	}

	return false;
}

bool Arranger::tryMulDiv() {
	return false;
}
