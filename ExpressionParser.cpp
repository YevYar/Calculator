#include <cctype>

#include "ExpressionParser.h"

using namespace calculator;

ExpressionParser* ExpressionParser::clone() const {
	return new ExpressionParser(*this);
}

bool ExpressionParser::parseNextToken(bool spaceSensitive)
{
	if (_isEndReached) {
		return false;
	}

	if (_index >= _currExpr.length()) {
		markThatEndIsReached();
		return false;
	}

	char ch = _currExpr.at(_index++);

	if (!spaceSensitive) {
		while (isspace(ch) && _index < _currExpr.length()) {
			ch = _currExpr.at(_index++);
		}

		if (_index == _currExpr.length() && isspace(ch)) {
			markThatEndIsReached();
			return false;
		}

	}
	else if (isspace(ch)) {
		_currTok = tokenValues::SPACE;
		return true;
	}

	if (isalpha(ch)) {
		_currTok = tokenValues::NAME;
		return true;
	}
	if (isdigit(ch)) {
		_currTok = tokenValues::NUMBER;
		return true;
	}

	switch (ch) {
	case '+':
		_currTok = tokenValues::PLUS;
		break;
	case '-':
		_currTok = tokenValues::MINUS;
		break;
	case '*':
		_currTok = tokenValues::MUL;
		break;
	case '/':
		_currTok = tokenValues::DIV;
		break;
	case '=':
		_currTok = tokenValues::ASSIGN;
		break;
	case '(':
		_currTok = tokenValues::LP;
		break;
	case ')':
		_currTok = tokenValues::RP;
		break;
	default:
		_currTok = tokenValues::UNKNOWN;
		break;
	}

	return true;
}

double ExpressionParser::parseNumber()
{
	try {
		size_t pos;
		double num = std::stod(_currExpr.substr(--_index), &pos);
		_index += pos;
		return num;
	}
	catch (const std::exception&) {
		throw std::exception("Invalid number");
	}
}

std::string ExpressionParser::parseName()
{
	std::string variableName;

	while (_index <= _currExpr.length() && _currTok == tokenValues::NAME) {
		variableName.push_back(_currExpr.at(_index - 1));
		parseNextToken(true);
	}

	--_index;
	return variableName;
}

void ExpressionParser::setNewExpression(const std::string& newExpr)
{
	_currExpr = newExpr;
	_index = 0;
	_isEndReached = false;
	_currTok = tokenValues::NO_OPERAND;
}

ExpressionParser::TokenValue ExpressionParser::getCurrentToken() const noexcept
{
	return _currTok;
}

std::string ExpressionParser::getRestOfExpression() const
{
	return _currExpr.substr(_index > 0 ? _index - 1 : 0);
}

void ExpressionParser::setNewExprFromCurIndex()
{
	setNewExpression(_currExpr.substr(_index));
}

void ExpressionParser::comeBackToPreviosToken() noexcept
{
	--_index;
}

void ExpressionParser::markThatEndIsReached()
{
	_isEndReached = true;
	_currTok = tokenValues::NO_OPERAND;
}
