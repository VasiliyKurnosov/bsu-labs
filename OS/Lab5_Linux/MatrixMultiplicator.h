#pragma once

#include <pthread.h>
#include "Matrix.h"

namespace matrix
{

class MatrixMultiplicator
{
public:
    MatrixMultiplicator() = default;
    Matrix Multiply(const Matrix& matrix1, const Matrix& matrix2) const;
    Matrix MultiplyBlockMatrices(const Matrix& matrix1, const Matrix& matrix2, int k);
    bool CheckCompatibility(const Matrix& matrix1, const Matrix& matrix2) const;

private:
    pthread_mutex_t lock_;

    struct ThreadFunctionArgument
    {
        pthread_mutex_t& Lock;
        Matrix& Result;
        const Matrix& Matrix1;
        const Matrix& Matrix2;
        int Row1;
        int Column1;
        int Row2;
        int Column2;
        int BlockSize;

        ThreadFunctionArgument(pthread_mutex_t& lock, Matrix& result, const Matrix& matrix1, const Matrix& matrix2,
                               int row1, int column1, int row2, int column2, int block_size);
    };

    static void* MultiplyBlocks(void* argument);
};

}  // matrix