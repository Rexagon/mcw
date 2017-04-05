#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <map>

#include "Function.h"

class Parser
{
public:
	Parser();
	~Parser();


	void AddConstant(const std::string& token, double value);
	void AddOperator(const std::string& token, OperatorType operatorType, int priority, OperatorAssociativity associativity, std::function<double(double, double)> function);
	void AddFunction(const std::string& token, FunctionType functionType, std::function<double(double, double)> function);

	bool HasToken(const std::string& token);
	Token* GetToken(const std::string& token);

	Function CreateFunction(const std::string& expression);
private:
	std::vector<Token*> Parse(const std::string& expression);
	std::vector<Token*> MakeRPN(std::vector<Token*> tokens);

	std::map<std::string, Token*> m_tokens;
};

#endif
