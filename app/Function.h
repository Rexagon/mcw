#ifndef FUNCTION_H
#define FUNCTION_H

#include "Token.h"

#include <vector>

class Function
{
public:
	Function(Token* expression, const std::vector<Token*>& tokens) :
		m_expression(expression), m_tokens(tokens) {}

	~Function() { for (auto& it : m_tokens) delete it; }

	double operator()(double x) { return Evaluate(x); }

    double Evaluate(double x);

    double Derivative(double x, double epsilon = 0.00001);

    double FindRootIterations(double minX, double maxX, double epsilon);

    double FindRootDihotomy(double minX, double maxX, double epsilon);

    double FindRootNewton(double minX, double maxX, double epsilon);
private:
	Token* m_expression;
	std::vector<Token*> m_tokens;
};

#endif
