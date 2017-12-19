#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <stdexcept>

#include "equation.hpp"
#include "solving\solver.hpp"
#include "printer.hpp"


int main() {
	std::string expression("x + 7=5");
	Equation* eq;
	try {
		eq = new Equation(expression.c_str());
		//int t = 0;
		//t++;
	}
	catch (const std::invalid_argument& e) {
		std::cout << "Error: " << e.what();
		return 1;
	}

	Solver solver(eq);
	solver.solve() ? (std::cout << "equation solved") : (std::cout << "unable to solve equation");
	std::cout << "\n";

	Printer printer(eq);
	std::cout << printer.toString() << std::endl;

	/*std::vector<int> v = { 1,2,3,4,5,6 };
	int i = 0;
	for (auto it = v.begin(); it != v.end(); it++) {
		if ((i++) == 2) {
			v.erase(it - 1);
			v.erase(it + 1);
		}
	}

	for (int ii : v) {
		std::cout << ii << ", ";
	}*/

	return 0;
}
