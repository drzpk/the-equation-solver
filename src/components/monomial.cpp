#include "monomial.hpp"

Monomial::Monomial(float a, int pow, char letter) {
	this->value = a;
	this->pow = pow;
	this->letter = letter;
}

bool Monomial::isCompatible(Component* other, OperationType type) const {
	if (!Component::isCompatible(other, type)) {
		return false;
	}

	Monomial* monomial = dynamic_cast<Monomial*>(other);
	if (type == OperationType::MULTIPLY || type == OperationType::DIVIDE) {
		if (letter && monomial->letter) {
			return letter == monomial->letter;
		}
		else {
			return true;
		}
	}
	else {
		// addition, subtraction
		return pow == monomial->pow && letter == monomial->letter;
	}
}

const char* Monomial::getName() const {
	return "monomial";
}

Component& Monomial::operator+=(const Component& other) {
	value += CAST(other).getValue();
	return *this;
}

Component& Monomial::operator-=(const Component& other) {
	value -= CAST(other).getValue();
	return *this;
}

Component& Monomial::operator*=(const Component& other) {
	auto cast = CAST(other);
	value *= cast.getValue();
	if (letter && cast.letter)
		pow += cast.pow;
	return *this;
}

Component& Monomial::operator/=(const Component& other) {
	auto cast = CAST(other);
	value /= cast.getValue();
	if (letter && cast.letter)
		pow -= cast.pow;
	return *this;
}

Component* Monomial::clone() const {
	Monomial* monomial = new Monomial(value, pow, letter);
	return monomial;
}

std::string Monomial::toString() const {
	std::string output;
	if (!letter || abs(1.f - value) > 0.0001f) {
		// no need to print 1 before variable
		output += Component::toString();
	}
	if (letter) {
		output += letter;
	}
	if (pow > 1) {
		output += "^";
		output += std::to_string(pow);
	}

	return output;
}