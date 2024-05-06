#include "MyUtils.hpp"

float evaluateReversePolishFunction(std::vector<std::string> tokens)
{
    std::stack<float> stack;

    std::set<std::string> binaryOperators = {"*", "/", "+", "-", "pow"};
    std::set<std::string> unaryOperators = {"sin", "cos", "sqrt", "abs"};
    std::set<std::string> constants = {"e", "pi"};

    bool isImaginary = false;

    for (std::vector<std::string>::iterator t = tokens.begin(); t != tokens.end(); ++t)
    {
        if (binaryOperators.find(*t) != binaryOperators.end())
        {
            float b = stack.top();
            stack.pop();
            float a = stack.top();
            stack.pop();
            if (!t->compare("*"))
            {
                stack.push(a * b);
            }
            else if (!t->compare("/"))
            {
                stack.push(a / b);
            }
            else if (!t->compare("+"))
            {
                stack.push(a + b);
            }
            else if (!t->compare("-"))
            {
                stack.push(a - b);
            }
            else if (!t->compare("pow"))
            {
                stack.push(std::pow(a, b));
            }
        }
        else if (unaryOperators.find(*t) != unaryOperators.end())
        {
            float a = stack.top();
            stack.pop();
            if (!t->compare("sin"))
            {
                stack.push(std::sin(a));
            }
            else if (!t->compare("cos"))
            {
                stack.push(std::cos(a));
            }
            else if (!t->compare("sqrt"))
            {
                if (a < 0)
                {
                    isImaginary = true;
                }
                stack.push(std::sqrt(a));
            }
            else if (!t->compare("abs"))
            {
                stack.push(std::abs(a));
            }
        }
        else if (constants.find(*t) != constants.end())
        {
            if (!t->compare("e"))
            {
                stack.push(2.71828182845904523536f);
            }
            else if (!t->compare("pi"))
            {
                stack.push(3.14159265358979323846f);
            }
        }
        else
        {
            stack.push(std::stof(*t));
        }
    }

    if (isImaginary)
        return 0;

    return stack.top();
}
