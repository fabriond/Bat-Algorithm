#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <functional>

class Matrix{
public:
    Matrix(int numRows, int numCols);
    Matrix transpose();
    Matrix forEach(std::function<double(double)> function);
    void setValue(int r, int c, double v){values.at(r).at(c) = v;}
    double getValue(int r, int c) const {return values.at(r).at(c);}
    int getNumRows(){return numRows;}
    int getNumCols(){return numCols;}
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
    friend Matrix operator+(const Matrix& m1, const Matrix& m2);
    friend Matrix operator-(const Matrix& m1, const Matrix& m2);
    friend Matrix operator*(const Matrix& m1, const Matrix& m2);
    friend Matrix dot(const Matrix& m1, const Matrix& m2);

private:
    int numRows, numCols;
    std::vector<std::vector<double> > values;
};

#endif