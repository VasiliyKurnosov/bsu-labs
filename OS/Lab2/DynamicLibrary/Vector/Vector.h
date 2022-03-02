#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H


#include <cmath>
#include <Number.h>

namespace math
{
    class Vector
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

    std::ostream& operator<<(std::ostream & out, const Vector & vector);

    const Vector ZERO_VECTOR(ZERO, ZERO);
    const Vector ONE_VECTOR(ONE, ONE);
}  // namespace math


#endif //VECTOR_VECTOR_H
