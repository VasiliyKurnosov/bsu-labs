#include <ctime>
#include "MatrixCreator.h"

namespace matrix
{

Matrix MatrixCreator::Read(std::istream& in, int rows, int columns)
{
    Matrix result_matrix(rows, columns);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            in >> result_matrix(i, j);
        }
    }
    return result_matrix;
}

Matrix MatrixCreator::GenerateRandom(int rows, int columns, double max_value)
{
    Matrix result_matrix(rows, columns);
    srand(time(0));
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            result_matrix(i, j) = rand() % (int)max_value;
        }
    }
    return result_matrix;
}

}  // matrix