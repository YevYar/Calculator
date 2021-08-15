#include <cctype>

#include "ExpressionParser.h"

using namespace calculator;

ExpressionParser* ExpressionParser::clone() const {
	return new ExpressionParser(*this);
}

bool ExpressionParser::parseNextToken(bool spaceSensitive)
{
	if (isEndReached) {
		return false;
	}

	if (currIndex >= currExpr.length()) {
		markThatEndIsReached();
		return false;
	}

	char ch = currExpr.at(currIndex++);

	if (!spaceSensitive) {
		while (isspace(ch) && currIndex < currExpr.length()) {
			ch = currExpr.at(currIndex++);
		}

		if (currIndex == currExpr.length() && isspace(ch)) {
			markThatEndIsReached();
			return false;
		}

	}
	else if (isspace(ch)) {
		currTok = tokenValues::SPACE;
		return true;
	}

	if (isalpha(ch)) {
		currTok = tokenValues::NAME;
		return true;
	}
	if (isdigit(ch)) {
		currTok = tokenValues::NUMBER;
		return true;
	}

	switch (ch) {
	case '+':
		currTok = tokenValues::PLUS;
		break;
	case '-':
		currTok = tokenValues::MINUS;
		break;
	case '*':
		currTok = tokenValues::MUL;
		break;
	case '/':
		currTok = tokenValues::DIV;
		break;
	case '=':
		currTok = tokenValues::ASSIGN;
		break;
	case '(':
		currTok = tokenValues::LP;
		break;
	case ')':
		currTok = tokenValues::RP;
		break;
	default:
		currTok = tokenValues::UNKNOWN;
		break;
	}

	return true;
}

double ExpressionParser::parseNumber()
{
	try {
		size_t pos;
		double num = std::stod(currExpr.substr(--currIndex), &pos);
		currIndex += pos;
		return num;
	}
	catch (const std::exception&) {
		throw std::exception("Invalid number");
	}
}

std::string ExpressionParser::parseName()
{
	std::string variableName;

	while (currIndex <= currExpr.length() && currTok == tokenValues::NAME) {
		variableName.push_back(currExpr.at(currIndex - 1));
		parseNextToken(true);
	}

	--currIndex;
	return variableName;
}

void ExpressionParser::setNewExpression(const std::string& newExpr)
{
	currExpr = newExpr;
	currIndex = 0;
	isEndReached = false;
	currTok = tokenValues::NO_OPERAND;
}

ExpressionParser::TokenValue ExpressionParser::getCurrentToken() const noexcept
{
	return currTok;
}

std::string ExpressionParser::getRestOfExpression() const
{
	return currExpr.substr(currIndex > 0 ? currIndex - 1 : 0);
}

void ExpressionParser::setNewExprFromCurIndex()
{
	setNewExpression(currExpr.substr(currIndex));
}

void ExpressionParser::comeBackToPreviosToken() noexcept
{
	--currIndex;
}

void ExpressionParser::markThatEndIsReached()
{
	isEndReached = true;
	currTok = tokenValues::NO_OPERAND;
}
