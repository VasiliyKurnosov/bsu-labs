#pragma once

#include <vector>
#include <ostream>

namespace matrix
{

class Matrix
{
public:
    Matrix(int m, int n);
    double& operator()(int i, int j);
    double operator()(int i, int j) const;
    int GetRows() const;
    int GetColumns() const;
    bool IsSquare() const;

private:
    std::vector<std::vector<double>> elements_;
};

std::ostream& operator<<(std::ostream& out, const Matrix& matrix);

}  // matrix