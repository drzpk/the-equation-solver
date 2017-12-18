#include "printer.hpp"

void Printer::setHasSpaces(bool hasSpaces) {
	useSpaces = hasSpaces;
}

std::string Printer::toString() const {
	std::string output;

	// Left side of the equation
	if (equation->getLeft()) {
		output += format(equation->getLeft()).str();
		output += useSpaces ? " " : "";
	}

	output += "=";

	// Right side of the equation
	if (equation->getRight()) {
		output += useSpaces ? " " : "";
		output += format(equation->getRight()).str();
	}

	return output;
}

std::ostringstream Printer::format(pElem elements) const {
	std::ostringstream output;
	for (auto it = elements->cbegin(); it != elements->cend(); it++) {
		Element* element = *it;
		switch (element->type) {
		case ELEMENTS:
			output << format(static_cast<pElem>(element->pointer)).str();
			break;
		case COMPONENT: {
			Component* component = static_cast<Component*>(element->pointer);
			output << component;
			break;
		}
		case OPERATION: {
			char visual = 0;
			OperationType type = (OperationType) reinterpret_cast<int>(element->pointer);
			switch (type) {
			case ADD:
				visual = '+';
				break;
			case SUBTRACT:
				visual = '-';
				break;
			case MULTIPLY:
				visual = '*';
				break;
			case DIVIDE:
				visual = '/';
				break;
			}

			if (useSpaces) output << " ";
			output << visual;
			if (useSpaces) output << " ";

			break;
		}
		}
	}

	return output;
}