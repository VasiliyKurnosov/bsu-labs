#pragma once

#include <cmath>
#include <Number.h>

#ifdef VECTOR_EXPORTS
#define VECTOR_API __declspec(dllexport)
#else
#define VECTOR_API __declspec(dllimport)
#endif // VECTOR_EXPORTS


namespace math
{
	class VECTOR_API Vector
	{
	public:
		Vector() = default;
		Vector(Number x, Number y);
		void Add(Vector other);
		Number GetPolarRadius() const;
		Number GetPolarArgument() const;
		Number GetX() const;
		Number GetY() const;

	private:
		Number x_, y_;
	};

	extern "C" VECTOR_API std::ostream& operator<<(std::ostream & out, const Vector & vector);

	const Vector ZERO_VECTOR(ZERO, ZERO);
	const Vector ONE_VECTOR(ONE, ONE);
}  // namespace math