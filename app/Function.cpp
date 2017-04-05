#include "Function.h"

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

double Function::FindRootIterations(double minX, double maxX, double epsilon)
{
    double lambda = 1.0 / Derivative(maxX);

    double x = minX;
    // (fabs())

    return minX;
}

double Function::FindRootDihotomy(double minX, double maxX, double epsilon)
{
    return minX;
}

double Function::FindRootNewton(double minX, double maxX, double epsilon)
{
    return minX;
}
