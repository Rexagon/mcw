#ifndef TOKEN_H
#define TOKEN_H

#include <functional>
#include <iostream>
#include <string>

class Token
{
public:
	enum class Type
	{
		Unknown,

		Number,
		Variable,
		Operator,
		Function,

		OpenBracket,
		CloseBracket,
		Comma
	};

	Token(Type type, const std::string& token = std::string()) : 
		m_type(type), m_token(token), m_leftChild(nullptr), m_rightChild(nullptr) {}

	Type GetType() const { return m_type; }

	virtual double GetValue() const { return 0.0; }

	std::string ToString() const { return m_token; }

	void SetLeftChild(Token* token) { m_leftChild = token; }
	Token* GetLeftChild() const { return m_leftChild; }

	void SetRightChild(Token* token) { m_rightChild = token; }
	Token* GetRightChild() const { return m_rightChild; }

	virtual Token* Clone() const { return new Token(*this); }
protected:
	Type m_type;
	std::string m_token;

	Token* m_leftChild;
	Token* m_rightChild;
};


class NumberToken : public Token
{
public:
	NumberToken(double value, const std::string& token = std::string()) : 
		Token(Type::Number, token), m_value(value) {}

	NumberToken(const std::string& value, const std::string& token = std::string()) : Token(Type::Number, token), m_value(std::atof(value.c_str())) {}

	double GetValue() const override { return m_value; }

    Token* Clone() const override { return new NumberToken(*this); }
private:
	double m_value;
};


class VariableToken : public Token
{
public:
	VariableToken(const std::string& token = std::string()) : 
		Token(Type::Variable, token), m_value(0.0) {}

	void SetValue(double value) { m_value = value; }
	double GetValue() const override { return m_value; }

    Token* Clone() const override { return new VariableToken(*this); }
private:
	double m_value;
};


enum class OperatorType
{
	Unary,
	Binary
};

enum class OperatorAssociativity
{
	Left,
	Right
};

class OperatorToken : public Token
{
public:
	OperatorToken(OperatorType operatorType, int priority, OperatorAssociativity associativity, std::function<double(double, double)> function, const std::string& token = std::string()) :
		Token(Token::Type::Operator, token), m_operatorType(operatorType), m_priority(priority), m_associativity(associativity), m_function(function) {}

	double GetValue() const override {
		if (m_rightChild == nullptr) {
			throw std::runtime_error("operator must have at least one operand");
		}

		if (m_operatorType == OperatorType::Binary) {
			if (!m_leftChild) throw std::runtime_error("binary operator must have two operands");

			return m_function(m_leftChild->GetValue(), m_rightChild->GetValue());
		}
		else {
			return m_function(m_rightChild->GetValue(), 0.0);
		}
	}

	OperatorType GetOperatorType() const { return m_operatorType; }
	int GetPriority() const { return m_priority; }
	OperatorAssociativity GetAssociativity() const { return m_associativity; }

    Token* Clone() const override { return new OperatorToken(*this); }
private:
	OperatorType m_operatorType;
	int m_priority;
	OperatorAssociativity m_associativity;
	std::function<double(double, double)> m_function;
};


enum class FunctionType
{
	Unary,
	Binary
};

class FunctionToken : public Token
{
public:
	FunctionToken(FunctionType functionType, std::function<double(double, double)> function, const std::string& token = std::string()) :
		Token(Token::Type::Function, token), m_functionType(functionType), m_function(function) {}

	double GetValue() const override {
		if (m_rightChild == nullptr) {
			throw std::runtime_error("function must have at least one operand");
		}

		if (m_functionType == FunctionType::Binary) {
			if (!m_leftChild) throw std::runtime_error("binary function must have two operands");

			return m_function(m_leftChild->GetValue(), m_rightChild->GetValue());
		}
		else {
			return m_function(m_rightChild->GetValue(), 0.0);
		}
	}

	FunctionType GetFunctionType() const { return m_functionType; }

    Token* Clone() const override { return new FunctionToken(*this); }
private:
	FunctionType m_functionType;
	std::function<double(double, double)> m_function;
};

#endif TOKEN_H
