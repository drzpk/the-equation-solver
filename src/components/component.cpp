#include "component.hpp"

float Component::getValue() const {
	return value;
}

bool Component::isCompatible(Component* other, OperationType type) const {
	return !strcmp(getName(), other->getName());
}

Component& Component::operator+=(const Component& other) {
	return *this;
}

Component& Component::operator-=( const Component& other) {
	return *this;
}

Component& Component::operator*=(const Component& other) {
	return *this;
}

Component& Component::operator/=(const Component& other) {
	return *this;
}

std::string Component::toString() const {
	std::ostringstream output;
	output << value;
	return output.str();
}

std::ostream& operator<<(std::ostream& ostream, const Component& component) {
	ostream << component.toString();
	return ostream;
}

std::ostream& operator<<(std::ostream& ostream, Component* component) {
	ostream << component->toString();
	return ostream;
}