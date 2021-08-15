#include <algorithm>
#include <fmt/core.h>
#include <iostream>
#include <iterator>

#include "Calculator.h"
#include "ExpressionParser.h"

using namespace calculator;
namespace TokenValue = calculator::tokenValues;

Calculator::Calculator(): parser(new ExpressionParser())
{
}

Calculator::Calculator(ExpressionParser* newParser): parser(newParser == nullptr ? new ExpressionParser() : newParser)
{
}

Calculator::Calculator(const Calculator& obj):
	variables(obj.variables), parser(obj.parser->clone())
{
}

Calculator::Calculator(Calculator&& obj) noexcept:
	variables(std::move(obj.variables)), parser(obj.parser)
{
	obj.parser = nullptr;
}

Calculator& Calculator::operator=(const Calculator& obj)
{
	if (this != &obj) {
		ExpressionParser* const tempPointer = obj.parser->clone();
		delete parser;
		parser = tempPointer;
		variables = obj.variables;
	}

	return *this;
}

Calculator& Calculator::operator=(Calculator&& obj) noexcept
{
	if (this != &obj) {
		variables = std::move(obj.variables);
		delete parser;
		parser = obj.parser;
		obj.parser = nullptr;
	}

	return *this;
}

Calculator::~Calculator()
{
	delete parser;
}

// it is faster pass by value than pass by const ref because the next commented code slowlier
double Calculator::calculate(std::string expression)
{
	/*
	std::string tempExpr;
	tempExpr.resize(expression.length());
	std::replace_copy(expression.begin(), expression.end(), tempExpr.begin(), ',', '.');
	*/
	std::replace(expression.begin(), expression.end(), ',', '.');
	parser->setNewExpression(expression);
	
	try {
		return expr();
	}
	catch (const std::exception& error) {
		std::string errorMes = fmt::format("{errorMes}: position: {pos}",
			fmt::arg("errorMes", error.what()), 
			fmt::arg("pos", parser->getRestOfExpression()));
		throw std::exception(errorMes.c_str());
	}
}

// E -> T + E | T - E | T
double Calculator::expr()
{
	double first = term();

	while (parser->parseNextToken()) {
		switch (parser->getCurrentToken()) {
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

	while (parser->parseNextToken()) {
		switch (parser->getCurrentToken()) {
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
			parser->comeBackToPreviosToken();
			return first;
		}
	}

	return first;
}

// P -> N | (E) | Var = E
double Calculator::prim()
{
	parser->parseNextToken();

	switch (parser->getCurrentToken()) {
	case TokenValue::LP: {
		double num = expr();
		if (parser->getCurrentToken() != TokenValue::RP) {
			throw std::exception("The ) is needed");
		}
		return num;
	}
	case TokenValue::NAME: {
		std::string name = parser->parseName();
		auto checkVar = [&name](const std::pair<std::string, double>& var) { return var.first == name; };
		auto var = std::find_if(begin(variables), end(variables), checkVar);

		parser->parseNextToken();

		if (parser->getCurrentToken() == TokenValue::ASSIGN) {
			parser->setNewExprFromCurIndex();
			double num = expr();
			auto newVar = std::make_pair(name, num);

			if (var != std::end(variables)) {
				variables.erase(var);
			}

			variables.insert(std::move(newVar));
			return num;		
		}
		
		if (var != std::end(variables)) {
			parser->comeBackToPreviosToken();
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
		return parser->parseNumber();
	case TokenValue::MINUS: 
		parser->parseNextToken();

		if (parser->getCurrentToken() == TokenValue::NUMBER) {
			parser->comeBackToPreviosToken();
			return parser->parseNumber();
		}
		else throw std::exception("Invalid expression");
	default:
		parser->comeBackToPreviosToken();
		return 0;
	}

	return 0;
}
