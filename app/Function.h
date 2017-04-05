#ifndef FUNCTION_H
#define FUNCTION_H

#include "Token.h"

#include <vector>

struct Root
{
	Root(double x, int numIterations) :
		x(x), numIterations(numIterations) {}

	double x;
	int numIterations;
};

enum RootsFindingMethod
{
	IterationsMethod,
	DihotomyMethod,
	NewtonsMethod
};

class Function
{
public:
	Function(Token* expression, const std::vector<Token*>& tokens) :
		m_expression(expression), m_tokens(tokens) {}

	~Function() { for (auto& it : m_tokens) delete it; }

    double Evaluate(double x);
    double Derivative(double x, double epsilon = 0.00001);

	std::vector<Root> FindRoots(double minX, double maxX, double epsilon, int method);

	double operator()(double x) { return Evaluate(x); }
private:
	Root Iterations(double minX, double maxX, double epsilon);
	Root Dihotomy(double minX, double maxX, double epsilon);
	Root Newton(double minX, double maxX, double epsilon);

	Token* m_expression;
	std::vector<Token*> m_tokens;
};

#endif
