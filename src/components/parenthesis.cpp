#include "parenthesis.hpp"

Parenthesis::~Parenthesis() {
	for (auto it = contents->begin(); it != contents->end(); it++) {
		auto c = *it;
		if (c->type == ElementType::COMPONENT)
			delete c->getComponent();
	}

	delete contents;
}

bool Parenthesis::isCompatible(Component* other, OperationType type) const {
	// Parenthesis expression can only be multiplied or divided by other
	// component. Addition or subtration is omitting the parentheses.
	return type == OperationType::MULTIPLY || type == OperationType::DIVIDE;
}

const char* Parenthesis::getName() const {
	return "parenthesis";
}

Component& Parenthesis::operator+=(const Component& other) {
	// disallowed
	return *this;
}

Component& Parenthesis::operator-=(const Component& other) {
	// disallowed
	return *this;
}

Component& Parenthesis::operator*=(const Component& other) {
	handleOperations(other, true);
	return *this;
}

Component& Parenthesis::operator/=(const Component& other) {
	handleOperations(other, false);
	return *this;
}

Component* Parenthesis::clone() const {
	pElem new_contents = new std::vector<Element*>;
	for (auto it = contents->begin(); it != contents->end(); it++) {
		auto c = *it;
		if (c->type == ElementType::COMPONENT) {
			Component* comp = c->getComponent();
			Component* new_comp = comp->clone();
			
			Element* e = new Element();
			e->type = ElementType::COMPONENT;
			e->pointer = static_cast<void*>(new_comp);
		}
		else if (c->type == ElementType::OPERATION) {
			OperationType op = c->getOperation();

			Element* e = new Element();
			e->type = ElementType::OPERATION;
			e->pointer = reinterpret_cast<void*>(op);
		}
	}

	return new Parenthesis(new_contents);
}

void Parenthesis::handleOperations(const Component& other, bool mul) {
	pElem new_contents = nullptr;
	if (!strcmp(other.getName(), "monomial"))
		new_contents = dmOneParenthesis(static_cast<const Monomial*>(&other), mul);
	else if (!strcmp(other.getName(), this->getName()))
		new_contents = dmTwoParenthesis(static_cast<const Parenthesis*>(&other), mul);

	if (new_contents != nullptr) {
		for (auto it = contents->begin(); it != contents->end(); it++)
			delete *it;

		contents = new_contents;
	}
}

pElem Parenthesis::dmOneParenthesis(const Monomial* monomial, bool mul) {
	pElem new_contents = new std::vector<Element*>;
	for (auto it = contents->begin(); it != contents->end(); it++) {
		auto e = *it;
		if (e->type == ElementType::COMPONENT) {
			auto c = e->getComponent();
			auto copy = c->clone();
			mul ? copy->operator*=(*monomial) : copy->operator/=(*monomial);
			
			Element* e = new Element;
			e->type = ElementType::COMPONENT;
			e->pointer = static_cast<void*>(copy);
			new_contents->push_back(e);
		}
		else if (e->type == ElementType::OPERATION) {
			auto o = e->getOperation();
			Element* e = new Element;
			e->type = ElementType::OPERATION;
			e->pointer = reinterpret_cast<void*>(o);
			new_contents->push_back(e);
		}
	}

	return new_contents;
}

pElem Parenthesis::dmTwoParenthesis(const Parenthesis* other, bool mul) {
	pElem new_contents = new std::vector<Element*>;
	for (auto first = contents->begin(); first != contents->end(); first++) {
		for (auto second = other->contents->begin(); second != other->contents->end(); second++) {
			// todo
		}
	}

	return nullptr;
}
