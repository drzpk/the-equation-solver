#include "equation.hpp"

std::regex Equation::monomialRegex("^(-)?(\\d+[,.]?\\d*)*\\*?([A-Za-z])?(\\^\\d)*$");

Equation::Equation(const char* source) {
	// pre-validate source
	std::string str(source);
	size_t eq_count = std::count(str.cbegin(), str.cend(), '=');
	if (!eq_count) {
		throw std::invalid_argument("equality sign not found");
	}
	else if (eq_count > 1) {
		throw std::invalid_argument("too many equality signs");
	}

	std::vector<Element*>* elements = parse(str, 0, strlen(source));

	// sort equation to left and right side
	// this will make solving easier
	left = new std::vector<Element*>;
	right = new std::vector<Element*>;
	bool leftMode = true;
	for (auto it = elements->begin(); it != elements->end(); it++) {
		Element* element = *it;
		if (element->type == ElementType::OPERATION
			&& reinterpret_cast<int>(element->pointer) == OperationType::EQUALS) {
			// equals sign
			leftMode = false;
			continue;
		}

		if (leftMode) {
			left->push_back(element);
		}
		else {
			right->push_back(element);
		}
	}
	delete elements;
}

Equation::~Equation() {
	if (left) {
		deleteElements(left);
	}
	if (right) {
		deleteElements(right);
	}
}

Equation::Equation(Equation* other) {
	variableName = other->variableName;

	if (other->left) {
		left = cloneElements(other->left);
	}
	if (other->right) {
		right = cloneElements(other->right);
	}
}

pElem Equation::getLeft() const {
	return left;
}

pElem Equation::getRight() const {
	return right;
}

pElem Equation::parse(const std::string& source, size_t start, size_t end) {
	std::vector<Element*>* tmpElements = new std::vector<Element*>;
	std::string buffer;
	bool expect_operator = false;

	for (size_t i = start; i < end; i++) {
		char c = source[i];

		// ignore spaces
		if (c == ' ')
			continue;

		if (IS_OPERATION(c)) {
			if (buffer.size()) {
				// add component
				processComponent(tmpElements, buffer, i);
				buffer.clear();
			}

			// check if operator expected
			// (minus sign can be either an operator or be part of a number)
			if (!expect_operator && c != '-') {
				char buffer[30] = { 0 };
				sprintf_s(buffer, 29, "unexpected operator '%c' at %zi", c, i + 1);
				delete tmpElements;
				throw std::invalid_argument(buffer);
			}

			OperationType type;
			switch (c) {
			case '+':
				type = OperationType::ADD;
				break;
			case '-':
				type = OperationType::SUBTRACT;
				break;
			case '*':
				type = OperationType::MULTIPLY;
				break;
			case '/':
				type = OperationType::DIVIDE;
				break;
			case '=':
				type = OperationType::EQUALS;
				break;
			}
			expect_operator = false;

			Element* element = new Element;
			element->type = ElementType::OPERATION;
			element->pointer = reinterpret_cast<void*>(type);
			tmpElements->push_back(element);

		}
		else if (c == ')') {
			// closing parenthesis isn't expected at this point
			char buffer[38] = { 0 };
			sprintf_s(buffer, 37, "unexpected closing parenthesis at %zi", i + 1);
			delete tmpElements;
			throw std::invalid_argument(buffer);
		}
		// ensure that found parentheses don't belong to a component
		else if (c == '(' && (i > 0 || IS_OPERATION(source[i - 1]))) {
			size_t subend = 0;
			bool found = false;
			int open = 0;
			for (size_t j = i; j < end; j++) {
				if (source[j] == ')') {
					open--;
					if (!open) {
						// found desired parenthesis
						subend = j;
						found = true;
						break;
					}
					else if (open < 0) {
						char buffer[38] = { 0 };
						sprintf_s(buffer, 37, "unexpected closing parenthesis at %zi", j + 1);
						delete tmpElements;
						throw std::invalid_argument(buffer);
					}
				}
				else if (source[j] == '(') {
					open++;
				}
			}

			if (!found) {
				char buffer[40] = { 0 };
				sprintf_s(buffer, 39, "expected closing parenthesis after %zi", i + 1);
				delete tmpElements;
				throw std::invalid_argument(buffer);
			}
			
			auto subElement = parse(source, i + 1, subend);
			Element* element = new Element;
			element->type = ElementType::COMPONENT;
			element->pointer = new Parenthesis(subElement);
			tmpElements->push_back(element);

			i = subend;
			expect_operator = true;
		}
		else {
			buffer += c;
			expect_operator = true;
		}
	}

	if (!buffer.size()) {
		// Formula must end with a component.
		// Calculator mode isn't supported yet.
		char buffer[37] = { 0 };
		sprintf_s(buffer, 36, "unexpected end of the formula at %zi", source.size());
		delete tmpElements;
		throw std::invalid_argument(buffer);
	}
	processComponent(tmpElements, buffer, source.size() - buffer.size());

	return tmpElements;
}

void Equation::processComponent(pElem elements, const std::string& buffer, size_t pos) {
	Component* component = parseComponent(buffer, pos);
	Element* element = new Element;
	element->type = ElementType::COMPONENT;
	element->pointer = component;
	elements->push_back(element);
}

Component* Equation::parseComponent(const std::string& source, size_t pos) {
	// check for monomial
	std::cmatch cmatch;
	if (std::regex_match(source.c_str(), cmatch, monomialRegex)) {
		bool minus = cmatch[1].length();
		float number = 1.f;

		if (cmatch[1].length()) {
			number *= -1;
		}

		if (cmatch[2].length()) {
			// TODO: check numbers with dot and comma
			number = std::stof(cmatch[2].str());
		}

		Monomial* monomial = new Monomial(number);
		if (cmatch[3].length()) {
			monomial->letter = cmatch[3].str()[0];
		}

		if (cmatch[4].length()) {
			std::string powstr(cmatch[4].str().begin() + 1, cmatch[4].str().end());
			monomial->pow = std::stoi(powstr);
		}

		return monomial;
	}

	// component not recognized
	char buffer[30] = { 0 };
	sprintf_s(buffer, 29, "unrecognized component at %zi", pos + 1);
	throw std::invalid_argument(buffer);

	return 0;
}

pElem Equation::cloneElements(pElem elements) {
	pElem clone = new std::vector<Element*>;
	for (auto it = elements->cbegin(); it != elements->cend(); it++) {
		Element* elem = *it;
		Element* newElement = new Element;
		newElement->type = elem->type;

		switch (elem->type) {
		case OPERATION:
			newElement->pointer = elem->pointer;
			break;
		case COMPONENT: {
			Component* oldComponent = static_cast<Component*>(elem->pointer);
			Component* newComponent = oldComponent->clone();
			newElement->pointer = newComponent;
			break;
		}
		}

		clone->push_back(newElement);
	}

	return clone;
}

void Equation::deleteElements(pElem elements) {
	for (auto it = elements->cbegin(); it != elements->cend(); it++) {
		Element* element = *it;
		delete element;
	}

	delete elements;
}
