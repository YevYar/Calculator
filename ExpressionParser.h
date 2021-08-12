#ifndef EXPRESSION_PARSER_H
#define EXPRESSION_PARSER_H

#include <string>

namespace calculator {
	namespace tokenValues {
		constexpr int NAME = 0;
		constexpr int NUMBER = 1;
		constexpr int SPACE = 2;
		constexpr int NO_OPERAND = 3;
		constexpr int UNKNOWN = 4;
		constexpr int PLUS = '+';
		constexpr int MINUS = '-';
		constexpr int MUL = '*';
		constexpr int DIV = '/';
		constexpr int ASSIGN = '=';
		constexpr int LP = '(';
		constexpr int RP = ')';
	}

	class ExpressionParser 
	{
	public:
		using TokenValue = int;

		virtual ~ExpressionParser() = default;

		virtual ExpressionParser* clone() const;
		virtual bool parseNextToken(bool spaceSensitive = false);
		virtual double parseNumber();
		virtual std::string parseName();
		virtual void setNewExpression(const std::string& newExpr);
		TokenValue getCurrentToken() const noexcept;
		std::string getRestOfExpression() const;
		void setNewExprFromCurIndex();		
		void comeBackToPreviosToken() noexcept;

	protected:				
		virtual void markThatEndIsReached();

		TokenValue _currTok = tokenValues::NO_OPERAND;
		std::string _currExpr = "";
		size_t _index = 0;
		bool _isEndReached = false;

	};
}

#endif
