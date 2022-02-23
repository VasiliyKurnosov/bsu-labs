#pragma once

#include <ostream>

namespace math
{
	class Number
	{
	public:
		explicit Number(double value = 0);
		double GetValue() const;

	private:
		double value_;
	};

	const Number ZERO(0);
	const Number ONE(1);

	Number operator+(const Number& number1, const Number& number2);
	Number operator-(const Number& number1, const Number& number2);
	Number operator*(const Number& number1, const Number& number2);
	Number operator/(const Number& number1, const Number& number2);

	std::ostream& operator<<(std::ostream& out, const Number& number);

	Number CreateNumber(double value);
}  // namespace math