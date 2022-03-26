#pragma once

#include <istream>
#include "Matrix.h"

namespace matrix
{

class MatrixCreator
{
public:
	Matrix Read(std::istream& in, int rows, int columns);
	Matrix GenerateRandom(int rows, int columns, double max_value);
};

}  // matrix
