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
	string str;
	try {
		while (getline(cin, str)) {
			stringstream data(str);
			string tempExpr;

			if (helpers::isCinValid()) {
				while (getline(data, tempExpr, ';')) {
					try {
						cout << tempExpr << " = " << _calculator->calculate(tempExpr) << endl << endl;
					}
					catch (const exception& error) {
						cerr << tempExpr << endl << error.what() << endl << endl;
					}
				}
			}
			else cerr << "The expression is not finished." << endl;
		}
	}
	catch (const exception& error) {
		cerr << error.what() << endl;
	}
}
