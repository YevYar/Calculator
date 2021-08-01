// Notes:
// 1. A semicolon (;) is used as a delimeter of different expressions.
// 2. Calculating of expressions starts after pressing Enter.
// 3. The last expression in the line can optionaly to not use a semicolon as a delimeter.

#ifndef CALCULATOR_DRIVER_H
#define CALCULATOR_DRIVER_H

#include "Calculator.h"

class CalculatorDriver
{
public:
	~CalculatorDriver();
	void run();

private:
	Calculator* _calculator = new Calculator();

};

#endif