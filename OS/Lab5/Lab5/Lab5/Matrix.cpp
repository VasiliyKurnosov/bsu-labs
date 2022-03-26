#include "Matrix.h"

namespace matrix
{

Matrix::Matrix(int m, int n) : elements_(m, std::vector<double>(n))
{
}

double Matrix::operator()(int i, int j) const
{
	if (i < 0 || i >= GetRows() || j < 0 || j >= GetColumns())
	{
		throw std::exception("Index out of range");
	}
	return elements_[i][j];
}

double& Matrix::operator()(int i, int j)
{
	if (i < 0 || i >= GetRows() || j < 0 || j >= GetColumns())
	{
		throw std::exception("Index out of range");
	}
	return elements_[i][j];
}

int Matrix::GetRows() const
{
	return elements_.size();
}

int Matrix::GetColumns() const
{
	return elements_[0].size();
}

bool Matrix::IsSquare() const
{
	return GetRows() == GetColumns();
}

std::ostream& operator<<(std::ostream& out, const Matrix& matrix)
{
	for (int i = 0; i < matrix.GetRows(); i++)
	{
		for (int j = 0; j < matrix.GetColumns(); j++)
		{
			out << matrix(i, j) << " ";
		}
		out << std::endl;
	}
	return out;
}

}  // matrix
