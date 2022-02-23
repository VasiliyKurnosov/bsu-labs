#include "pch.h"
#include "Vector.h"

namespace math
{
	Vector::Vector(Number x, Number y) : x_(x), y_(y)
	{
	}

	void Vector::Add(Vector other)
	{
		x_ = x_ + other.GetX();
		y_ = y_ + other.GetY();
	}

	Number Vector::GetPolarRadius() const
	{
		return Number(sqrt((x_ * x_ + y_ * y_).GetValue()));
	}

	Number Vector::GetPolarArgument() const
	{
		double angle = acos((x_ / GetPolarRadius()).GetValue());
		if (asin((y_ / GetPolarRadius()).GetValue()) < 0)
		{
			double pi = 3.14159265358979323846;
			angle = 2 * pi - angle;
		}
		return Number(angle);
	}

	Number Vector::GetX() const
	{
		return x_;
	}

	Number Vector::GetY() const
	{
		return y_;
	}

	std::ostream& operator<<(std::ostream& out, const Vector& vector)
	{
		out << "(" << vector.GetX() << ", " << vector.GetY() << ")";
		return out;
	}
}  // namespace math
