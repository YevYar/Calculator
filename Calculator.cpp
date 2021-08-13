#include <algorithm>
#include <fmt/core.h>
#include <iostream>
#include <iterator>

#include "Calculator.h"
#include "ExpressionParser.h"

using namespace calculator;
namespace TokenValue = calculator::tokenValues;

Calculator::Calculator(): _parser(new ExpressionParser())
{
}

Calculator::Calculator(ExpressionParser* parser): _parser(parser == nullptr ? new ExpressionParser() : parser)
{
}

Calculator::Calculator(const Calculator& obj):
	_variables(obj._variables), _parser(obj._parser->clone())
{
}

Calculator::Calculator(Calculator&& obj) noexcept:
	_variables(std::move(obj._variables)), _parser(obj._parser)
{
	obj._parser = nullptr;
}

Calculator& Calculator::operator=(const Calculator& obj)
{
	if (this != &obj) {
		ExpressionParser* const tempPointer = obj._parser->clone();
		delete _parser;
		_parser = tempPointer;
		_variables = obj._variables;
	}

	return *this;
}

Calculator& Calculator::operator=(Calculator&& obj) noexcept
{
	if (this != &obj) {
		_variables = std::move(obj._variables);
		delete _parser;
		_parser = obj._parser;
		obj._parser = nullptr;
	}

	return *this;
}

Calculator::~Calculator()
{
	delete _parser;
}

double Calculator::calculate(const std::string& expression)
{
	std::string tempExpr;
	tempExpr.resize(expression.length());

	std::replace_copy(expression.begin(), expression.end(), tempExpr.begin(), ',', '.');
	_parser->setNewExpression(std::move(tempExpr));
	
	try {
		return expr();
	}
	catch (const std::exception& error) {
		std::string errorMes = fmt::format("{errorMes}: position: {pos}",
			fmt::arg("errorMes", error.what()), 
			fmt::arg("pos", _parser->getRestOfExpression()));
		throw std::exception(errorMes.c_str());
	}
}

// E -> T + E | T - E | T
double Calculator::expr()
{
	double first = term();

	while (_parser->parseNextToken()) {
		switch (_parser->getCurrentToken()) {
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
			return first;
		default:
			throw std::exception("Invalid expression");
		}
	}	

	return first;
}

// T -> P * T | P / T | P
double Calculator::term()
{
	double first = prim();

	while (_parser->parseNextToken()) {
		switch (_parser->getCurrentToken()) {
		case TokenValue::MUL: {
			double second = prim();
			first *= second;
			break;
		}
		case TokenValue::DIV: {
			double second = prim();
			if (second == 0) {
				throw std::exception("Division by zero");
			}
			first /= second;
			break;
		}
		default:
			_parser->comeBackToPreviosToken();
			return first;
		}
	}

	return first;
}

// P -> N | (E) | Var = E
double Calculator::prim()
{
	_parser->parseNextToken();

	switch (_parser->getCurrentToken()) {
	case TokenValue::LP: {
		double num = expr();
		if (_parser->getCurrentToken() != TokenValue::RP) {
			throw std::exception("The ) is needed");
		}
		return num;
	}
	case TokenValue::NAME: {
		std::string name = _parser->parseName();
		auto checkVar = [&name](const std::pair<std::string, double>& var) { return var.first == name; };
		auto var = std::find_if(begin(_variables), end(_variables), checkVar);

		_parser->parseNextToken();

		if (_parser->getCurrentToken() == TokenValue::ASSIGN) {
			_parser->setNewExprFromCurIndex();
			double num = expr();
			auto newVar = std::make_pair(name, num);

			if (var != std::end(_variables)) {
				_variables.erase(var);
			}

			_variables.insert(std::move(newVar));
			return num;		
		}
		
		if (var != std::end(_variables)) {
			_parser->comeBackToPreviosToken();
			return var->second;
		}
		else {	
			std::string errorMes = fmt::format("Undeclared variable {name}", fmt::arg("name", name));
			throw std::exception(errorMes.c_str());
		}

		break;
	}
	case TokenValue::ASSIGN: 
		throw std::exception("The assign operator (=) can be used only after var name");
	case TokenValue::NUMBER:
		return _parser->parseNumber();
	case TokenValue::MINUS: 
		_parser->parseNextToken();

		if (_parser->getCurrentToken() == TokenValue::NUMBER) {
			_parser->comeBackToPreviosToken();
			return _parser->parseNumber();
		}
		else throw std::exception("Invalid expression");
	default:
		_parser->comeBackToPreviosToken();
		return 0;
	}

	return 0;
}
