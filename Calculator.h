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

class Calculator
{
public:
	double calculate(const std::string& expression);

private:
	enum class TokenValue {
		NAME, NUMBER, SPACE, NO_OPERAND, UNKNOWN,
		PLUS = '+', MINUS = '-', MUL = '*', DIV = '/',
		PRINT = ';', DOT = '.', ASSIGN = '=', LP = '(', RP = ')'
	};	

	double expr();
	double term();
	double prim();
	void parseNextToken(bool spaceSensitive = false) noexcept;
	double parseNumber();
	std::string parseName() noexcept;	
	void markThatEndIsReached() noexcept;
	void resetCurrentExpression(const std::string& newExpr) noexcept;

	TokenValue _currTok = TokenValue::NO_OPERAND;
	std::string _currExpr = "";
	size_t _index = 0;
	std::set<std::pair<std::string, double>> _variables;
	bool _isEndReached = false;
};

#endif
