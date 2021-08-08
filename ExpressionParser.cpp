#include <cctype>

#include "ExpressionParser.h"

using namespace std;
using namespace calculator;

bool ExpressionParser::parseNextToken(bool spaceSensitive) noexcept
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
		_currTok = TokenValue::SPACE;
		return true;
	}

	if (isalpha(ch)) {
		_currTok = TokenValue::NAME;
		return true;
	}
	if (isdigit(ch)) {
		_currTok = TokenValue::NUMBER;
		return true;
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
	default:
		_currTok = TokenValue::UNKNOWN;
		break;
	}

	return true;
}

double ExpressionParser::parseNumber()
{
	try {
		size_t pos;
		double num = stod(_currExpr.substr(--_index), &pos);
		_index += pos;
		return num;
	}
	catch (const exception&) {
		throw exception("Invalid number");
	}
}

string ExpressionParser::parseName()
{
	string variableName;

	while (_index <= _currExpr.length() && _currTok == TokenValue::NAME) {
		variableName.push_back(_currExpr.at(_index - 1));
		parseNextToken(true);
	}

	--_index;
	return variableName;
}

ExpressionParser::TokenValue ExpressionParser::getCurrentToken() noexcept
{
	return _currTok;
}

void ExpressionParser::setNewExpression(const string& newExpr)
{
	_currExpr = newExpr;
	_index = 0;
	_isEndReached = false;
	_currTok = TokenValue::NO_OPERAND;
}

void ExpressionParser::setNewExprFromCurIndex()
{
	setNewExpression(_currExpr.substr(_index));
}

string ExpressionParser::getRestOfExpression()
{
	return _currExpr.substr(_index > 0 ? --_index : 0);
}

void ExpressionParser::comeBackToPreviosToken() noexcept
{
	--_index;
}

void ExpressionParser::markThatEndIsReached() noexcept
{
	_isEndReached = true;
	_currTok = TokenValue::NO_OPERAND;
}
