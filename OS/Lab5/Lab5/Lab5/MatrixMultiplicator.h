#pragma once

#include <mutex>
#include "Matrix.h"

namespace matrix
{

class MatrixMultiplicator
{
public:
	Matrix Multiply(const Matrix& matrix1, const Matrix& matrix2);
	Matrix MultiplyBlockMatrices(const Matrix& matrix1, const Matrix& matrix2, int k);
	bool CheckCompatibility(const Matrix& matrix1, const Matrix& matrix2);

private:
	std::mutex lock_;

	void MultiplyBlocks(Matrix& result, const Matrix& matrix1, const Matrix& matrix2, 
						int row1, int column1, int row2, int column2, int block_size);
};

}  // matrix
