#ifndef EXPRESSION_PARSER_H
#define EXPRESSION_PARSER_H

#include <string>

namespace calculator {
	namespace tokenValues {
		inline constexpr int NAME = 0;
		inline constexpr int NUMBER = 1;
		inline constexpr int SPACE = 2;
		inline constexpr int NO_OPERAND = 3;
		inline constexpr int UNKNOWN = 4;
		inline constexpr int PLUS = '+';
		inline constexpr int MINUS = '-';
		inline constexpr int MUL = '*';
		inline constexpr int DIV = '/';
		inline constexpr int ASSIGN = '=';
		inline constexpr int LP = '(';
		inline constexpr int RP = ')';
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

		TokenValue currTok = tokenValues::NO_OPERAND;
		std::string currExpr;
		size_t currIndex = 0;
		bool isEndReached = false;

	};
}

#endif
