#include "Number.h"

namespace math
{
	Number::Number(double value) : value_(value)
	{
	}

	double Number::GetValue() const
	{
	return value_;
	}

	Number operator+(const Number& number1, const Number& number2)
	{
	return Number(number1.GetValue() + number2.GetValue());
	}

	Number operator-(const Number& number1, const Number& number2)
	{
	return Number(number1.GetValue() - number2.GetValue());
	}

	Number operator*(const Number& number1, const Number& number2)
	{
	return Number(number1.GetValue() * number2.GetValue());
	}

	Number operator/(const Number& number1, const Number& number2)
	{
	return Number(number1.GetValue() / number2.GetValue());
	}

	std::ostream& operator<<(std::ostream& out, const Number& number)
	{
	out << number.GetValue();
	return out;
	}

	Number CreateNumber(double value)
	{
	return Number(value);
	}
}  // namespace math
