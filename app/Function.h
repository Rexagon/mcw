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

	double operator()(double x)
	{
		if (!m_expression) return 0.0;
		for (auto& it : m_tokens) {
			if (it->GetType() == Token::Type::Variable) {
				reinterpret_cast<VariableToken*>(it)->SetValue(x);
			}
		}
		return m_expression->GetValue();
	}

    double FindRootIterations(double minX, double maxX, double epsilon);

    double FindRootDihotomy(double minX, double maxX, double epsilon);

    double FindRootNewton(double minX, double maxX, double epsilon);
private:
	Token* m_expression;
	std::vector<Token*> m_tokens;
};

#endif
