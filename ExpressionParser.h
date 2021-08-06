#ifndef EXPRESSION_PARSER_H
#define EXPRESSION_PARSER_H

#include <string>

namespace calculator {
	class ExpressionParser 
	{
	public:
		enum class TokenValue {
			NAME, NUMBER, SPACE, NO_OPERAND, UNKNOWN,
			PLUS = '+', MINUS = '-', MUL = '*', DIV = '/',
			ASSIGN = '=', LP = '(', RP = ')'
		};

		bool parseNextToken(bool spaceSensitive = false) noexcept;
		double parseNumber();
		std::string parseName() noexcept;
		TokenValue getCurrentToken() noexcept;
		void setNewExpression(const std::string& newExpr) noexcept;
		void setNewExprFromCurIndex() noexcept;
		std::string getRestOfExpression() noexcept;
		void comeBackToPreviosToken() noexcept;

	private:				
		void markThatEndIsReached() noexcept;

		TokenValue _currTok = TokenValue::NO_OPERAND;
		std::string _currExpr = "";
		size_t _index = 0;
		bool _isEndReached = false;

	};
}

#endif
