#include <iostream>
#include <sstream>
#include <string>

#include "CalculatorDriver.h"
#include "Helpers.h"

using namespace std;

CalculatorDriver::~CalculatorDriver()
{
	delete _calculator;
}

void CalculatorDriver::run()
{
	string str;
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
		else throw exception("The expression is not finished.");
	}
}
