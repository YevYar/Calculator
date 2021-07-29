#include <iostream>

#include "Calculator.h"

int main() {
	Calculator calculator;

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
