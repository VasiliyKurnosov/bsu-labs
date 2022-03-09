#include <iostream>
#include <Number.h>
#include <Vector.h>

int main()
{
    math::Number n1(5.7), n2(1.5);
    std::cout << "5.7 + 1.5 = "<< n1 + n2  << std::endl;
    std::cout << "5.7 - 1.5 = "<< n1 - n2  << std::endl;
    std::cout << "5.7 * 1.5 = "<< n1 * n2  << std::endl;
    std::cout << "5.7 / 1.5 = "<< n1 / n2  << std::endl;

    math::Vector vector1(math::CreateNumber(1), math::CreateNumber(1.73));
    std::cout << "Argument(" << vector1 << ") = " << vector1.GetPolarArgument() << std::endl;
    std::cout << vector1 << " + " << math::ONE_VECTOR << " = ";
    vector1.Add(math::ONE_VECTOR);
    std::cout << vector1 << std::endl;
    std::cout << "Radius(" << vector1 << ") = " << vector1.GetPolarRadius() << std::endl;
}