#include "Function.h"

#include <cmath>

double Function::Evaluate(double x)
{
    if (!m_expression) return 0.0;
    for (auto& it : m_tokens) {
        if (it->GetType() == Token::Type::Variable) {
            reinterpret_cast<VariableToken*>(it)->SetValue(x);
        }
    }
    return m_expression->GetValue();
}

double Function::Derivative(double x, double epsilon)
{
	return (Evaluate(x + epsilon) - Evaluate(x)) / epsilon;
}

std::vector<Root> Function::FindRoots(double minX, double maxX, double epsilon, int method)
{
	std::vector<Root> roots;

	double step = 0.01;
	for (double x = minX; x < maxX; x += step) {
		if (Evaluate(x) == 0) {
			roots.push_back(Root(x, 1));
		}
		else if (Evaluate(x) * Evaluate(x + step) < 0) {
			switch (method) {
			case 0:
				roots.push_back(Iterations(x, x + step, epsilon));
				break;
			case 1:
				roots.push_back(Dihotomy(x, x + step, epsilon));
				break;
			case 2:
				roots.push_back(Newton(x, x + step, epsilon));
				break;
			default:
				break;
			}
		}
	}

	return roots;
}

Root Function::Iterations(double minX, double maxX, double epsilon)
{
	int numIterations = 0;

	double lastX, lambda = 1.0 / Derivative(minX);

	do {
		lastX = minX;
		minX = minX - lambda * Evaluate(minX);
		numIterations++;
	}
	while (std::fabs(minX - lastX) > epsilon);

	return Root(minX, numIterations);
}

Root Function::Dihotomy(double minX, double maxX, double epsilon)
{
	int numIterations = 0;

	double lastX, f;

	do {
		lastX = (minX + maxX) / 2.0;
		f = Evaluate(lastX);

		if (f * Evaluate(minX) < 0) {
			maxX = lastX;
		}
		else {
			minX = lastX;
		}
		numIterations++;
	} while (std::fabs(f) > epsilon);

	return Root(lastX, numIterations);
}

Root Function::Newton(double minX, double maxX, double epsilon)
{
	int numIterations = 0;

	double lastX;

	do {
		lastX = minX;
		minX = lastX - Evaluate(lastX) / Derivative(lastX);
		numIterations++;
	}
	while(std::fabs(minX - lastX) > epsilon);

	return Root(minX, numIterations);
}
