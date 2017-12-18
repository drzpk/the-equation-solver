#include "solver.hpp"

bool Solver::solve() {
	int debug = 0;
	Printer printer(equation);

	// Algorithm applies only one action at a time
	// in order to save a history of steps.
	bool stepCompleted = true;
	std::string lastOperation;
	while (stepCompleted) {
		stepCompleted = false;
		// DEBUG
		// TODO: create a decent testing instruments
		std::string printed = printer.toString();
		std::cout << "attempt " << (debug++) << ": " << printed << std::endl;
		std::cout << "lastOperation: " << lastOperation << std::endl;

		// Step #1: reduction
		if (reducer.reduce(equation)) {
			stepCompleted = true;
			lastOperation = "reduction";
		}

		// Step #2: flipping
		else if (flipper.flip(equation)) {
			stepCompleted = true;
			lastOperation = "flipping";
		}

		solved |= stepCompleted;
		if (!stepCompleted)
			break;
	}


	// return <check if solved>; <<<--- TODO
	return solved;
}

Equation** Solver::getResult() {
	return 0;
}
