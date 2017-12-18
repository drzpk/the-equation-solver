#include "flipper.hpp"

bool Flipper::flip(Equation* equation) {
	if (!equation->getLeft() || !equation->getRight()) {
		// This equation doesn't have either left or right side.
		// Nothing to do here.
		return false;
	}

	bool left = hasVariables(equation->getLeft());
	bool right = hasVariables(equation->getRight());

	if (right && !left) {
		// Swap equations only if variables are on the right side.
		pElem tmp = equation->left;
		equation->left = equation->right;
		equation->right = tmp;
		return true;
	}

	return false;
}

bool Flipper::hasVariables(pElem elements) {
	for (auto it = elements->cbegin(); it != elements->cend(); it++) {
		Element* element = *it;
		if (element->type != ElementType::COMPONENT) {
			continue;
		}

		// All components must be checked 'manually',
		// wthout benefits of polymorphism.
		Component* component = static_cast<Component*>(element->pointer);
		if (strcmp(component->getName(), "monomial")) {
			// Only monomials are needed.
			continue;
		}

		Monomial* monomial = dynamic_cast<Monomial*>(component);
		if (monomial->pow && monomial->letter) {
			// This component has variable.
			return true;
		}
	}

	return false;
}