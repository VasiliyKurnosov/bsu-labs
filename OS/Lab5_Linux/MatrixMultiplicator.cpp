#include <vector>
#include <string>
#include <exception>
#include "MatrixMultiplicator.h"

namespace matrix
{

Matrix MatrixMultiplicator::Multiply(const Matrix& matrix1, const Matrix& matrix2) const
{
    if (!CheckCompatibility(matrix1, matrix2))
    {
        throw std::runtime_error("Matrices are incompatible");
    }
    Matrix product = Matrix(matrix1.GetRows(), matrix2.GetColumns());
    for (int i = 0; i < product.GetRows(); ++i)
    {
        for (int j = 0; j < product.GetColumns(); ++j)
        {
            for (int k = 0; k < product.GetColumns(); ++k)
            {
                product(i, j) += matrix1(i, k) * matrix2(k, j);
            }
        }
    }
    return product;
}

Matrix MatrixMultiplicator::MultiplyBlockMatrices(const Matrix& matrix1, const Matrix& matrix2, int blocks_number)
{
    if (!CheckCompatibility(matrix1, matrix2))
    {
        throw std::runtime_error("Matrices are incompatible");
    }
    if (!matrix1.IsSquare() || !matrix2.IsSquare())
    {
        throw std::runtime_error("Matrices must be square");
    }
    if (matrix1.GetRows() % blocks_number != 0)
    {
        throw std::runtime_error("Matrices can't be divided on blocks of the same size");
    }
    int block_size = matrix1.GetRows() / blocks_number;
    Matrix product = Matrix(matrix1.GetRows(), matrix2.GetColumns());
    std::vector<pthread_t> threads;
    std::vector<ThreadFunctionArgument*> args;
    for (int i = 0; i < product.GetRows(); i += block_size)
    {
        for (int j = 0; j < product.GetColumns(); j += block_size)
        {
            for (int k = 0; k < product.GetColumns(); k += block_size)
            {
                auto* arg = new ThreadFunctionArgument(lock_, product, matrix1, matrix2, i, k, k, j, block_size);
                pthread_t thread;
                pthread_create(&thread, nullptr, &MatrixMultiplicator::MultiplyBlocks, arg);
                threads.push_back(thread);
                args.push_back(arg);
            }
        }
    }
    for (auto& thread : threads)
    {
        pthread_join(thread, nullptr);
    }
    for (auto& arg : args)
    {
        delete arg;
    }
    return product;
}

bool MatrixMultiplicator::CheckCompatibility(const Matrix& matrix1, const Matrix& matrix2) const
{
    return matrix1.GetColumns() == matrix2.GetRows();
}

MatrixMultiplicator::ThreadFunctionArgument::ThreadFunctionArgument(pthread_mutex_t& lock, Matrix& result,
        const Matrix& matrix1, const Matrix& matrix2, int row1, int column1, int row2, int column2, int block_size) :
        Lock(lock), Result(result), Matrix1(matrix1), Matrix2(matrix2), Row1(row1), Column1(column1), Row2(row2), Column2(column2), BlockSize(block_size)
{
}

void* MatrixMultiplicator::MultiplyBlocks(void* argument)
{
    auto* arg = (MatrixMultiplicator::ThreadFunctionArgument*)argument;
    for (int i = arg->Row1; i < arg->Row1 + arg->BlockSize; ++i)
    {
        for (int j = arg->Column2; j < arg->Column2 + arg->BlockSize; ++j)
        {
            double temp = 0;
            for (int k = 0; k < arg->BlockSize; ++k)
            {
                temp += arg->Matrix1(i, arg->Column1 + k) * arg->Matrix2(arg->Row2 + k, j);
            }
            pthread_mutex_lock(&arg->Lock);
            arg->Result(i, j) += temp;
            pthread_mutex_unlock(&arg->Lock);
        }
    }
    pthread_exit(nullptr);
}

}  // matrix