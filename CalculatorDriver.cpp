#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "CalculatorDriver.h"
#include "Helpers.h"

using namespace std;
using namespace calculator;

CalculatorDriver::CalculatorDriver(unique_ptr<Calculator> newCalculator): _calculator(move(newCalculator))
{
}

void CalculatorDriver::run() noexcept
{
	if (!_calculator) {
		cerr << "The calculator has not been set.\n";
		return;
	}

	try {
		string str;
		while (getline(cin, str)) {
			stringstream data(str);
			string tempExpr;

			if (helpers::isCinValid()) {
				while (getline(data, tempExpr, ';')) {
					try {
						cout << tempExpr << " = " << _calculator->calculate(tempExpr) << "\n\n";
					}
					catch (const exception& error) {
						cerr << tempExpr << endl << error.what() << "\n\n";
					}
				}
			}
			else cerr << "The expression is not finished.\n";
		}
	}
	catch (const exception& error) {
		cerr << error.what() << "\n";
	}
}
