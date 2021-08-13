#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "CalculatorDriver.h"
#include "Helpers.h"

using namespace calculator;

CalculatorDriver::CalculatorDriver(std::unique_ptr<Calculator> newCalculator): _calculator(std::move(newCalculator))
{
}

void CalculatorDriver::run() const noexcept
{
	if (!_calculator) {
		std::cerr << "The calculator has not been set.\n";
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
						std::cout << tempExpr << " = " << _calculator->calculate(tempExpr) << "\n\n";
					}
					catch (const std::exception& error) {
						std::cerr << tempExpr << "\n" << error.what() << "\n\n";
					}
				}
			}
			else std::cerr << "The expression is not finished.\n";
		}
	}
	catch (const std::exception& error) {
		std::cerr << error.what() << "\n";
	}
}
