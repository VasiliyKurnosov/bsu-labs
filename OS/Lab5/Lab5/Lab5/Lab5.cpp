#include <iostream>
#include <fstream>
#include <ctime>
#include "Matrix.h"
#include "MatrixCreator.h"
#include "MatrixMultiplicator.h"

int main()
{
	int n = 500;
	matrix::MatrixCreator matrix_creator;
	matrix::Matrix matrix1 = matrix_creator.GenerateRandom(n, n, 100);
	matrix::Matrix matrix2 = matrix_creator.GenerateRandom(n, n, 100);
	matrix::MatrixMultiplicator matrix_multiplicator;
	int start_time = clock();
	matrix::Matrix matrix3 = matrix_multiplicator.Multiply(matrix1, matrix2);
	int end_time = clock();
	std::cout << "Basic multiplication algorithm: " << end_time - start_time << std::endl;
	for (int k = 1; k <= n; ++k)
	{
		if (n % k == 0)
		{
			int start_time = clock();
			matrix::Matrix matrix4 = matrix_multiplicator.MultiplyBlockMatrices(matrix1, matrix2, k);
			int end_time = clock();
			std::cout << k * k << " blocks: " << end_time - start_time << std::endl;
		}
	}
}
