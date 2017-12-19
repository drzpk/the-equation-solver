#include "arranger.hpp"

std::vector<OperationType> Arranger::validSurroundings = {
	OperationType::ADD,
	OperationType::SUBTRACT
};

bool Arranger::arrange(Equation* equation) {
	this->equation = equation;

	if (!equation->getLeft() || !equation->getRight()) {
		// Equation must have two sides in order for this
		// class to work.
		return false;
	}

	// Step #1
	if (getOptimalSide(equation) == -1) {
		optimal = equation->getLeft();
		another = equation->getRight();
	}
	else {
		optimal = equation->getRight();
		another = equation->getLeft();
	}

	// Step #2 and #3
	return tryMove() || tryMulDiv();
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

	Monomial* appendix = nullptr;

	// Moving elements: optimal -> another
	for (auto it = optimal->begin(); it != optimal->end(); it++) {
		Monomial* monomial = canBeMoved(optimal, it);
		if (monomial == nullptr)
			continue;

		if (!monomial->letter) {
			// Move monomial without indeterminates to the opposite side
			int sign = get_sign(optimal, it) * -1;
			appendix = new Monomial(sign * abs(monomial->getValue()));
			break;
		}
	}

	// Moving elements: another -> optimal
	for (auto it = another->begin(); appendix == nullptr && it != another->end(); it++) {
		Monomial* monomial = canBeMoved(another, it);
		if (monomial == nullptr)
			continue;

		if (monomial->letter && monomial->pow > 0) {
			// Move monomial with indeterminate the the optimal side
			int sign = get_sign(another, it) * -1;
			appendix = new Monomial(sign * abs(monomial->getValue()), monomial->pow, monomial->letter);
			break;
		}
	}

	// Add appendix to the sides
	if (appendix != nullptr) {
		Element* addElement = new Element();
		addElement->type = ElementType::OPERATION;
		addElement->pointer = reinterpret_cast<void*>(OperationType::ADD);
		
		Element* optimalElement = new Element();
		optimalElement->type = ElementType::COMPONENT;
		optimalElement->pointer = appendix;
		Element* anotherElement = new Element();
		anotherElement->type = ElementType::COMPONENT;
		anotherElement->pointer = new Monomial(*appendix);

		// Only add new elements to equation and let the reducer do the rest
		optimal->push_back(addElement);
		optimal->push_back(optimalElement);
		another->push_back(new Element(*addElement));
		another->push_back(anotherElement);

		// Assume that before arranger equation was fully reduced
		while (reducer.reduce(equation));

		return true;
	}

	return false;
}

Monomial* Arranger::canBeMoved(pElem side, pElem_iterator it) {
	Element* element = *it;
	if (element->type != ElementType::COMPONENT) {
		// Only components can be checked.
		return nullptr;
	}

	if (!check_surrounding_operations(side, it, validSurroundings)) {
		// No moving can be performed when elements aren't
		// multiplied with each other yet.
		return nullptr;
	}

	Component* component = static_cast<Component*>(element->pointer);
	Monomial* monomial = dynamic_cast<Monomial*>(component);
	return monomial;
}

bool Arranger::tryMulDiv() {
	return false;
}
