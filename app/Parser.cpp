#include "Parser.h"

#include <iostream>
#include <stack>

#define M_PI 3.14159265358979323846
#define M_E 2.71828182845904523536

Parser::Parser()
{
    m_tokens["x"] = new VariableToken("x");

	m_tokens["("] = new Token(Token::Type::OpenBracket, "(");
	m_tokens[")"] = new Token(Token::Type::CloseBracket, ")");

	m_tokens[","] = new Token(Token::Type::Comma, ",");
	m_tokens[";"] = new Token(Token::Type::Comma, ",");

    m_tokens["$"] = new OperatorToken(OperatorType::Unary, 2, OperatorAssociativity::Left, [](double a, double b) { return +a; }, "+");
    m_tokens["#"] = new OperatorToken(OperatorType::Unary, 2, OperatorAssociativity::Left, [](double a, double b) { return -a; }, "-");

	AddOperator("+", OperatorType::Binary, 0, OperatorAssociativity::Left, [](double a, double b) { return a + b; });
	AddOperator("-", OperatorType::Binary, 0, OperatorAssociativity::Left, [](double a, double b) { return a - b; });

	AddOperator("*", OperatorType::Binary, 1, OperatorAssociativity::Left, [](double a, double b) { return a * b; });
	AddOperator("/", OperatorType::Binary, 1, OperatorAssociativity::Left, [](double a, double b) { return a / b; });
	AddOperator("\\", OperatorType::Binary, 1, OperatorAssociativity::Left, [](double a, double b) { return a / b; });
	AddOperator("%", OperatorType::Binary, 1, OperatorAssociativity::Left, [](double a, double b) { return std::fmod(a, b); });

	AddOperator("^", OperatorType::Binary, 3, OperatorAssociativity::Right, [](double a, double b) { return std::pow(a, b); });

    AddConstant("pi", M_PI);
    AddConstant("e", M_E);

	AddFunction("abs", FunctionType::Unary, [](double a, double b) { return std::abs(a); });
	AddFunction("sqrt", FunctionType::Unary, [](double a, double b) { return std::sqrt(a); });
	AddFunction("cbrt", FunctionType::Unary, [](double a, double b) { return std::cbrt(a); });
    AddFunction("exp", FunctionType::Unary, [](double a, double b) { return std::pow(M_E, a); });

	AddFunction("max", FunctionType::Binary, [](double a, double b) { return std::fmax(a, b); });

	AddFunction("ln", FunctionType::Unary, [](double a, double b) { return std::log(a); });
	AddFunction("lg", FunctionType::Unary, [](double a, double b) { return std::log10(a); });
	AddFunction("log", FunctionType::Binary, [](double a, double b) { return std::log(b) / std::log(a); });

	AddFunction("sin", FunctionType::Unary, [](double a, double b) { return std::sin(a); });
	AddFunction("cos", FunctionType::Unary, [](double a, double b) { return std::cos(a); });
	AddFunction("tg", FunctionType::Unary, [](double a, double b) { return std::tan(a); });
	AddFunction("ctg", FunctionType::Unary, [](double a, double b) { return 1.0 / std::tan(a); });

	AddFunction("arcsin", FunctionType::Unary, [](double a, double b) { return std::asin(a); });
	AddFunction("arccos", FunctionType::Unary, [](double a, double b) { return std::acos(a); });
	AddFunction("arctg", FunctionType::Unary, [](double a, double b) { return std::atan(a); });
    AddFunction("arcctg", FunctionType::Unary, [](double a, double b) { return M_PI / 2.0 - std::atan(a); });
}

Parser::~Parser()
{
	for (auto& it : m_tokens) {
		delete it.second;
	}
}

void Parser::AddConstant(const std::string & token, double value)
{
	auto it = m_tokens.find(token);
	if (it != m_tokens.end()) {
		delete it->second;
	}

	m_tokens[token] = new NumberToken(value, token);
}

void Parser::AddOperator(const std::string & token, OperatorType operatorType, int priority, OperatorAssociativity associativity, std::function<double(double, double)> function)
{
	auto it = m_tokens.find(token);
	if (it != m_tokens.end()) {
		delete it->second;
	}

	m_tokens[token] = new OperatorToken(operatorType, priority, associativity, function, token);
}

void Parser::AddFunction(const std::string & token, FunctionType functionType, std::function<double(double, double)> function)
{
	auto it = m_tokens.find(token);
	if (it != m_tokens.end()) {
		delete it->second;
	}

	m_tokens[token] = new FunctionToken(functionType, function, token);
}

bool Parser::HasToken(const std::string & token)
{
	return m_tokens.find(token) != m_tokens.end();
}

Token * Parser::GetToken(const std::string & token)
{
	auto it = m_tokens.find(token);
	if (it == m_tokens.end()) {
		return nullptr;
	}
	else {
		return it->second;
	}
}


std::vector<Token*> Parser::Parse(const std::string& expression)
{
	std::vector<Token*> result;

	try {
		std::string currentToken;
		Token::Type lastTokenType = Token::Type::Unknown;
		unsigned int dotsCount = 0;
		unsigned int bracketsCount = 0;
        unsigned int symbolNumber = 0;
		for (unsigned int i = 0; i < expression.size(); ++i) {
            if (::isspace(expression[i])) continue;

			char symbol = ::tolower(expression[i]);
			if (symbolNumber == 0 || lastTokenType == Token::Type::Operator || lastTokenType == Token::Type::OpenBracket) {
				if (symbol == '+') {
					symbol = '$';
				}
				else if (symbol == '-') {
					symbol = '#';
				}
            }

			if (symbol == '.' && dotsCount++ > 0) {
                throw std::runtime_error("Ошибка в записи числа");
			}

			currentToken.append(1, symbol);


			Token* token = nullptr;
			if ((token = GetToken(currentToken)) != nullptr) {
				token = token->Clone();
				currentToken.clear();
			}
			else if (isdigit(symbol) && (i + 1 > expression.size() || !(expression[i + 1] == '.' || expression[i + 1] == 'e' || isdigit(expression[i + 1])))) {
				for (auto& s : currentToken) {
					if (!isdigit(s) && s != '.' && s != 'e') {
                        throw std::runtime_error("Ошибка в записи числа");
					}
				}

				token = new NumberToken(currentToken, currentToken);
				currentToken.clear();
				dotsCount = 0;
			}

			if (token != nullptr) {
				lastTokenType = token->GetType();

				if (lastTokenType == Token::Type::OpenBracket) ++bracketsCount;
				else if (lastTokenType == Token::Type::CloseBracket) --bracketsCount;

				result.push_back(token);
			}

			++symbolNumber;
		}

        if (!currentToken.empty()) {
            throw std::runtime_error("Введён несуществующий оператор/функция");
		}
		else if (bracketsCount != 0) {
            throw std::runtime_error("Неправильно расставлены скобки");
		}

		return result;
	}
	catch (const std::exception& e) {
		for (auto& it : result) {
			delete it;
			it = nullptr;
		}

		throw e;
	}
}

std::vector<Token*> Parser::MakeRPN(std::vector<Token*> tokens)
{
	std::vector<Token*> result;
	std::stack<Token*> temp;

	try {
		for (auto& token : tokens) {
			switch (token->GetType())
			{
			case Token::Type::Number: case Token::Type::Variable:
				result.push_back(token);
				break;

			case Token::Type::Comma:
				delete token;
				token = nullptr;
				while (!temp.empty() && temp.top()->GetType() != Token::Type::OpenBracket) {
					result.push_back(temp.top());
					temp.pop();

					if (temp.empty()) {
                        throw std::runtime_error("Пропущена открывающая скобка или запятая");
					}
				}
				break;
			case Token::Type::Operator:
			{
				OperatorToken* op1 = reinterpret_cast<OperatorToken*>(token);
				OperatorToken* op2 = nullptr;
				while (!temp.empty() && (temp.top()->GetType() == Token::Type::Operator) && (op2 = reinterpret_cast<OperatorToken*>(temp.top())) &&
					(op1->GetAssociativity() == OperatorAssociativity::Left && op1->GetPriority() <= op2->GetPriority() ||
						op1->GetAssociativity() == OperatorAssociativity::Right && op1->GetPriority() < op2->GetPriority())) {
					result.push_back(temp.top());
					temp.pop();
				}
			}
			case Token::Type::Function:
			case Token::Type::OpenBracket:
				temp.push(token);
				break;
			case Token::Type::CloseBracket:
				delete token;
				token = nullptr;
				while (!temp.empty() && temp.top()->GetType() != Token::Type::OpenBracket) {
					result.push_back(temp.top());
					temp.pop();

					if (temp.empty()) {
                        throw std::runtime_error("Пропущена открывающая скобка");
					}
				}
				delete temp.top();
				temp.pop();

				if (!temp.empty() && temp.top()->GetType() == Token::Type::Function) {
					result.push_back(temp.top());
					temp.pop();
				}
				break;
			}
		}

		while (!temp.empty()) {
			result.push_back(temp.top());
			temp.pop();
		}

		return result;
	}
	catch (const std::exception& e) {
		for (auto& it : tokens) {
			delete it;
			it = nullptr;
		}
		for (auto& it : result) {
			delete it;
			it = nullptr;
		}
		while (!temp.empty()) {
			delete temp.top();
			temp.pop();
		}

		throw e;
	}
}


Function Parser::CreateFunction(const std::string & expression)
{
	auto rpn = MakeRPN(Parse(expression));

    /*std::cout << "----------\n";
	for (auto& it : rpn) {
		std::cout << it->ToString() << ' ';
	}
    std::cout << "\n----------\n";*/

	std::stack<Token*> temp;

	try {
		for (auto& token : rpn) {
			switch (token->GetType()) {
			case Token::Type::Number:
			case Token::Type::Variable:
				temp.push(token);
				break;
			case Token::Type::Operator:
				{
					OperatorToken* op = reinterpret_cast<OperatorToken*>(token);

                    std::string argumentsNum = op->GetOperatorType() == OperatorType::Unary ? "1 аргумент" : "2 аргумента";

                    if (temp.empty()) throw std::runtime_error(op->ToString() + " требует " + argumentsNum);
					auto right = temp.top();
					temp.pop();
					op->SetRightChild(right);

					if (op->GetOperatorType() == OperatorType::Binary) {
                        if (temp.empty()) throw std::runtime_error(op->ToString() + " требует " + argumentsNum);
						auto left = temp.top();
						temp.pop();
						op->SetLeftChild(left);
					}

					temp.push(token);
				}
				break;
			case Token::Type::Function:
				{
					FunctionToken* op = reinterpret_cast<FunctionToken*>(token);

                    std::string argumentsNum = op->GetFunctionType() == FunctionType::Unary ? "1 аргумент" : "2 аргумента";

                    if (temp.empty()) throw std::runtime_error(op->ToString() + " требует " + argumentsNum);
					auto right = temp.top();
					temp.pop();
					op->SetRightChild(right);

					if (op->GetFunctionType() == FunctionType::Binary) {
                        if (temp.empty()) throw std::runtime_error(op->ToString() + " требует " + argumentsNum);
						auto left = temp.top();
						temp.pop();
						op->SetLeftChild(left);
					}

					temp.push(token);
				}
				break;
			}
		}

		if (temp.size() < 2) {
			Token* token = nullptr;
			if (!temp.empty()) {
				token = temp.top();
			}

			return Function(token, rpn);
		}
		else {
            throw std::runtime_error("Пропущен оператор");
		}
	}
	catch (const std::runtime_error& e) {
		for (auto& it : rpn) {
			delete it;
			it = nullptr;
		}

		throw e;
	}
}
