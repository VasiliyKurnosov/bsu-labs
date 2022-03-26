#include <vector>
#include <thread>
#include <algorithm>
#include <string>
#include <exception>
#include "MatrixMultiplicator.h"

namespace matrix
{

Matrix MatrixMultiplicator::Multiply(const Matrix& matrix1, const Matrix& matrix2)
{
	if (!CheckCompatibility(matrix1, matrix2))
	{
		throw std::exception("Matrices are incompatible");
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
		throw std::exception("Matrices are incompatible");
	}
	if (!matrix1.IsSquare() || !matrix2.IsSquare())
	{
		throw std::exception("Matrices must be square");
	}
	if (matrix1.GetRows() % blocks_number != 0)
	{
		throw std::exception("Matrices can't be divided on blocks of the same size");
	}
	int block_size = matrix1.GetRows() / blocks_number;
	Matrix product = Matrix(matrix1.GetRows(), matrix2.GetColumns());
	std::vector<std::thread> threads;
	for (int i = 0; i < product.GetRows(); i += block_size)
	{
		for (int j = 0; j < product.GetColumns(); j += block_size)
		{
			for (int k = 0; k < product.GetColumns(); k += block_size)
			{
				std::thread thread(&MatrixMultiplicator::MultiplyBlocks, this, std::ref(product),
								   std::ref(matrix1), std::ref(matrix2), i, k, k, j, block_size);
				threads.push_back(std::move(thread));
			}
		}
	}
	for (auto& thread : threads)
	{
		thread.join();
	}
	return product;
}

bool MatrixMultiplicator::CheckCompatibility(const Matrix& matrix1, const Matrix& matrix2)
{
	return matrix1.GetColumns() == matrix2.GetRows();
}

void MatrixMultiplicator::MultiplyBlocks(Matrix& result, const Matrix& matrix1, const Matrix& matrix2,
										 int row1, int column1, int row2, int column2, int block_size)
{
	for (int i = row1; i < row1 + block_size; ++i)
	{
		for (int j = column2; j < column2 + block_size; ++j)
		{
			double temp = 0;
			for (int k = 0; k < block_size; ++k)
			{
				temp += matrix1(i, column1 + k) * matrix2(row2 + k, j);
			}
			std::lock_guard<std::mutex> locker(lock_);
			result(i, j) += temp;
		}
	}
}

}  // matrix
