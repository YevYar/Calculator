// The calculator support:
// 1. standard operations +-*/
// 2. real numbers with dot (.) and comma (,)
// 3. expressions with brackets ()
// 4. variables, which must contain only letters
// 5. assigning of results of expressions, negative numbers and other variables to variable
// 6. variables reassigning 

// Notes:
// 1. The calculator is not space sensitive. Spaces is taken into account only while parsing names and numbers.

// The calculator is implemented as recursive descent parser (https://en.wikipedia.org/wiki/Recursive_descent_parser).
// Simple explanation - https://habr.com/ru/post/489534/

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <set>

namespace calculator {
	class ExpressionParser;

	class Calculator
	{
	public:
		Calculator();
		Calculator(const Calculator& obj);
		Calculator(Calculator&& obj) noexcept;
		Calculator& operator=(const Calculator& obj);
		Calculator& operator=(Calculator&& obj) noexcept;
		virtual ~Calculator();

		virtual double calculate(std::string expression);

	protected:
		Calculator(ExpressionParser* parser);

		virtual double expr();
		virtual double term();
		virtual double prim();
		
		std::set<std::pair<std::string, double>> variables;
		ExpressionParser* parser;

	};
}

#endif
