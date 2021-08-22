#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "CalculatorDriver.h"
#include "Helpers.h"

using namespace calculator;

CalculatorDriver::CalculatorDriver(std::unique_ptr<Calculator> newCalculator): calculator(std::move(newCalculator))
{
}

void CalculatorDriver::run() const noexcept
{
	if (!calculator) {
		std::cerr << "The calculator has not been set." << std::endl;
		return;
	}

	try {
		std::string str;
		while (std::getline(std::cin, str)) {
			std::stringstream data(str);
			std::string tempExpr;

			if (helpers::isCinValid()) {
				while (std::getline(data, tempExpr, ';')) {
					try {
						double result = calculator->calculate(tempExpr);
						std::cout << tempExpr << " = " << result << "\n" << std::endl;
					}
					catch (const std::exception& error) {
						std::cerr << tempExpr << "\n" << error.what() << "\n" << std::endl;
					}
				}
			}
			else std::cerr << "The expression is not finished." << std::endl;
		}
	}
	catch (const std::exception& error) {
		std::cerr << error.what() << std::endl;
	}
}
