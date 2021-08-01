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
				_calculator->startCalculation(tempExpr);
			}
		}
		else throw exception("The expression is not finished.");
	}
}
