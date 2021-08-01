#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <sstream>

#include "Calculator.h"

using namespace std;

void Calculator::startCalculation(const std::string& expression)
{
	string tempExpr;
	tempExpr.resize(expression.length());

	replace_copy(expression.begin(), expression.end(), tempExpr.begin(), ',', '.');
	resetCurrentExpression(tempExpr);

	cout << expression << " = " << expr() << endl;
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
		string name = parseName();	
		auto checkVar = [&name](const pair<string, double>& var) { return var.first == name; };
		auto var = find_if(begin(_variables), end(_variables), checkVar);

		parseNextToken();

		if (_currTok == TokenValue::ASSIGN) {
			resetCurrentExpression(_currExpr.substr(_index));
			double num = expr();
			auto newVar = make_pair(name, num);

			if (var != end(_variables)) {
				_variables.erase(var);
			}

			_variables.insert(newVar);
			return num;		
		}
		
		if (var != end(_variables)) {
			--_index;
			return var->second;
		}
		else {				
			stringstream data;
			data << "Undeclared variable " << name;
			throw exception(data.str().c_str());
		}

		break;
	}
	case TokenValue::ASSIGN: 
		throw exception("The assign operator (=) can be used only after var name");
	case TokenValue::NUMBER:
		return parseNumber();
	case TokenValue::MINUS: 
		parseNextToken();

		if (_currTok == TokenValue::NUMBER) {
			--_index;
			return parseNumber();
		}
		else throw exception("Invalid expression");
	default:
		--_index;
		return 0;
	}

	return 0;
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
	default:
		_currTok = TokenValue::UNKNOWN;
		break;
	}
}

double Calculator::parseNumber()
{
	try {
		stringstream data(_currExpr.substr(--_index));
		double num = 0;
		data >> num;

		auto pos = data.tellg();
		if (pos != -1) {
			_index += pos;
		}
		else {
			_index = _currExpr.length();
		}

		return num;

	}
	catch (const exception&) {
		throw exception("Invalid number");
	}
}

string Calculator::parseName() noexcept
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

void Calculator::resetCurrentExpression(const string& newExpr) noexcept
{
	_currExpr = newExpr;
	_index = 0;
	_isEndReached = false;
	_currTok = TokenValue::NO_OPERAND;
}
