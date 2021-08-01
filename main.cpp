#include <iostream>

#include "CalculatorDriver.h"

int main() {
	CalculatorDriver calculator;

	try
	{
		calculator.run();
	}
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;

		return 1;
	}

	return 0;
}
