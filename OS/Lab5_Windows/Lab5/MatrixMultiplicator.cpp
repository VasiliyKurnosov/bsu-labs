#include <vector>
#include <algorithm>
#include <string>
#include <exception>
#include "MatrixMultiplicator.h"

namespace matrix
{

MatrixMultiplicator::MatrixMultiplicator()
{
	h_lock_ = CreateMutex(NULL, FALSE, NULL);
}

MatrixMultiplicator::~MatrixMultiplicator()
{
	CloseHandle(h_lock_);
}

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
	std::vector<HANDLE> h_threads;
	std::vector<ThreadFunctionArgument*> args;
	for (int i = 0; i < product.GetRows(); i += block_size)
	{
		for (int j = 0; j < product.GetColumns(); j += block_size)
		{
			for (int k = 0; k < product.GetColumns(); k += block_size)
			{
				ThreadFunctionArgument* arg = new ThreadFunctionArgument(h_lock_, product, matrix1, matrix2, i, k, k, j, block_size);
				HANDLE h_thread = CreateThread(NULL, 0, MatrixMultiplicator::MultiplyBlocks, arg, 0, NULL);
				h_threads.push_back(h_thread);
				args.push_back(arg);
			}
		}
	}
	WaitForMultipleObjects(h_threads.size(), &h_threads[0], TRUE, INFINITE);
	for (int i = 0; i < h_threads.size(); ++i)
	{
		CloseHandle(h_threads[i]);
		delete args[i];
	}
	return product;
}

bool MatrixMultiplicator::CheckCompatibility(const Matrix& matrix1, const Matrix& matrix2)
{
	return matrix1.GetColumns() == matrix2.GetRows();
}

MatrixMultiplicator::ThreadFunctionArgument::ThreadFunctionArgument(HANDLE h_lock, Matrix& result,
	const Matrix& matrix1, const Matrix& matrix2, int row1, int column1, int row2, int column2, int block_size) :
	HLock(h_lock), Result(result), Matrix1(matrix1), Matrix2(matrix2), Row1(row1), Column1(column1), Row2(row2), Column2(column2), BlockSize(block_size)
{
}

DWORD WINAPI MatrixMultiplicator::MultiplyBlocks(LPVOID argument)
{
	MatrixMultiplicator::ThreadFunctionArgument* arg = (MatrixMultiplicator::ThreadFunctionArgument*)argument;
	for (int i = arg->Row1; i < arg->Row1 + arg->BlockSize; ++i)
	{
		for (int j = arg->Column2; j < arg->Column2 + arg->BlockSize; ++j)
		{
			double temp = 0;
			for (int k = 0; k < arg->BlockSize; ++k)
			{
				temp += arg->Matrix1(i, arg->Column1 + k) * arg->Matrix2(arg->Row2 + k, j);
			}
			WaitForSingleObject(arg->HLock, INFINITE);
			arg->Result(i, j) += temp;
			ReleaseMutex(arg->HLock);
		}
	}
	ExitThread(0);
}

}  // matrix
