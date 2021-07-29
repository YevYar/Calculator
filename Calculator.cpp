#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <sstream>

#include "Calculator.h"
#include "Helpers.h"

using namespace std;

void Calculator::run()
{
	string str;
	while (getline(cin, str)) {

		std::stringstream data(str);
		string tempExpr;

		if (helpers::isCinValid()) {
			while (getline(data, tempExpr, ';')) {
				replace(tempExpr.begin(), tempExpr.end(), ',', '.');
				resetCurrentExpression(tempExpr);
				cout << tempExpr << " = " << expr() << endl;
			}
		}
		else throw exception("The expression is not finished.");
	}
}

// E -> T + E | T - E | T
double Calculator::expr() noexcept
{
	double first = 0;

	try {
		first = term();

		while (_index < _currExpr.length() && !_isEndReached) {
			parseNextToken();

			switch (_currTok) {
			case TokenValue::PLUS: {
				double second = term();
				first += second;
				break;
			}
			case TokenValue::MINUS: {
				double second = term();
				first -= second;
				break;
			}
			case TokenValue::RP:
			case TokenValue::NO_OPERAND:
			case TokenValue::PRINT:
				return first;
			default:
				throw exception("Invalid expression");
			}
		}
	}
	catch (const exception& error) {
		cerr << error.what() << ": position: " << _currExpr.substr(--_index) << "\nCurrent result: " << first << endl;
		resetCurrentExpression("");
	}

	return first;
}

// T -> P * T | P / T | P
double Calculator::term()
{
	double first = prim();

	while (_index < _currExpr.length() && !_isEndReached) {
		parseNextToken();

		switch (_currTok) {
		case TokenValue::MUL: {
			double second = prim();
			first *= second;
			break;
		}
		case TokenValue::DIV: {
			double second = prim();
			if (second == 0) {
				throw exception("Division by zero");
			}
			first /= second;
			break;
		}
		default:
			--_index;
			return first;
		}
	}

	return first;
}

// P -> N | (E) | Var = E
double Calculator::prim()
{
	TokenValue lastSymbol = _currTok;
	
	parseNextToken();
	if (_currTok == TokenValue::MINUS) {
		return parseNumber();
	}

	--_index;
	_currTok = lastSymbol;

	string varName;

	while (_index < _currExpr.length() && !_isEndReached) {
		lastSymbol = _currTok;
		parseNextToken();

		switch (_currTok) {
		case TokenValue::LP: {
			double num = expr();
			if (_currTok != TokenValue::RP) {
				throw exception("The ) is needed");
			}
			return num;
		}
		case TokenValue::NAME: {
			// variable cannot be used after another variable, number or ) without +-*/=(
			if (lastSymbol == TokenValue::NAME || lastSymbol == TokenValue::NUMBER || lastSymbol == TokenValue::RP) {
				throw exception("Invalid expression");
			}
			// names cannot contain spaces
			if (lastSymbol == TokenValue::SPACE && !varName.empty()) {
				throw exception("Invalid variable name");
			}

			string name = parseName();	
			auto checkVar = [&name](const pair<string, double>& var) { return var.first == name; };
			auto var = find_if(begin(_variables), end(_variables), checkVar);

			if (var == end(_variables)) {
				varName = name;
			}
			else {
				return var->second;
			}

			break;
		}
		case TokenValue::ASSIGN: {
			if (varName.empty()) {
				throw exception("The assign operator (=) can be used only after var name");
			}

			resetCurrentExpression(_currExpr.substr(_index));

			double num = expr();
			auto newVar = make_pair(varName, num);
			_variables.insert(newVar);
			return num;
		}
		case TokenValue::NUMBER:
			return parseNumber();
		default:
			--_index;
			return 0;
		}
	}
}

void Calculator::parseNextToken(bool spaceSensitive) noexcept
{
	if (_index >= _currExpr.length()) {
		markThatEndIsReached();
		return;
	}
	
	char ch = _currExpr.at(_index++);

	if (!spaceSensitive) {
		while (isspace(ch) && _index < _currExpr.length()) {
			ch = _currExpr.at(_index++);
		}

		if (_index == _currExpr.length() && isspace(ch)) {
			markThatEndIsReached();
			return;
		}

	} else if (isspace(ch)) {
		_currTok = TokenValue::SPACE;
		return;
	}

	if (isalpha(ch)) {
		_currTok = TokenValue::NAME;
		return;
	}
	if (isdigit(ch)) {
		_currTok = TokenValue::NUMBER;
		return;
	}
	
	switch (ch) {
	case '+':
		_currTok = TokenValue::PLUS;
		break;
	case '-':
		_currTok = TokenValue::MINUS;
		break;
	case '*':
		_currTok = TokenValue::MUL;
		break;
	case '/':
		_currTok = TokenValue::DIV;
		break;
	case '=':
		_currTok = TokenValue::ASSIGN;
		break;
	case '(':
		_currTok = TokenValue::LP;
		break;
	case ')':
		_currTok = TokenValue::RP;
		break;
	case '.':
		_currTok = TokenValue::DOT;
		break;
	case ';':
		_currTok = TokenValue::PRINT;
		break;
	}
}

double Calculator::parseNumber()
{
	string numberStr;
	bool isDotFound = false;
	
	while (_index <= _currExpr.length() 
		&& (_currTok == TokenValue::NUMBER || _currTok == TokenValue::DOT || _currTok == TokenValue::MINUS)) 
	{
		// cannot be more than one minus in a number
		if (_currTok == TokenValue::MINUS && !numberStr.empty()) {
			// just break because minus can be a part of an expression, not only of a number
			break;
		}
		if (_currTok == TokenValue::DOT && isDotFound) {
			throw exception("Extra dot in the number");
		}
		if (_currTok == TokenValue::DOT && !isDotFound) {
			isDotFound = true;
		}	

		numberStr.push_back(_currExpr.at(_index - 1));
		parseNextToken(true);
	}

	--_index;

	try {
		return stod(numberStr);
	}
	catch (const exception& error) {
		throw exception("Invalid number");
	}
}

std::string Calculator::parseName() noexcept
{
	string variableName;
	
	while (_index <= _currExpr.length() && _currTok == TokenValue::NAME) {
		variableName.push_back(_currExpr.at(_index - 1));
		parseNextToken(true);
	}

	--_index;
	return variableName;
}

void Calculator::markThatEndIsReached() noexcept
{
	_isEndReached = true;
	_currTok = TokenValue::NO_OPERAND;
}

void Calculator::resetCurrentExpression(const std::string& newExpr) noexcept
{
	_currExpr = newExpr;
	_index = 0;
	_isEndReached = false;
	_currTok = TokenValue::NO_OPERAND;
}
