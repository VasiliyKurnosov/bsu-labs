#pragma once

#include <windows.h>
#include "Matrix.h"

namespace matrix
{

class MatrixMultiplicator
{
public:
	MatrixMultiplicator();
	~MatrixMultiplicator();
	Matrix Multiply(const Matrix& matrix1, const Matrix& matrix2);
	Matrix MultiplyBlockMatrices(const Matrix& matrix1, const Matrix& matrix2, int k);
	bool CheckCompatibility(const Matrix& matrix1, const Matrix& matrix2);

private:
	HANDLE h_lock_;

	struct ThreadFunctionArgument
	{
		HANDLE HLock;
		Matrix& Result;
		const Matrix& Matrix1;
		const Matrix& Matrix2;
		int Row1;
		int Column1;
		int Row2;
		int Column2;
		int BlockSize;

		ThreadFunctionArgument(HANDLE h_lock, Matrix& result, const Matrix& matrix1, const Matrix& matrix2,
							   int row1, int column1, int row2, int column2, int block_size);
	};

	static DWORD WINAPI MultiplyBlocks(LPVOID argument);
};

}  // matrix
