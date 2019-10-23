#include <string>
#include <iostream>
#include "../include/matrix.h"
#include "../include/random_engine.h"

Matrix::Matrix(int numRows, int numCols, bool isRandom): 
    numRows(numRows), 
    numCols(numCols)
{
    for(int i = 0; i < numRows; ++i){
        std::vector<double> colValues;
        for(int j = 0; j < numCols; ++j){
            if(isRandom) colValues.push_back(uniformRandom(0.0, 1.0));
            else colValues.push_back(0.0);
        }
        values.push_back(colValues);
    }
}

Matrix Matrix::transpose(){
    Matrix aux(numCols, numRows);
    
    for(int i = 0; i < numRows; ++i){
        for(int j = 0; j < numCols; ++j){
            aux.setValue(j, i, getValue(i, j));
        }
    }

    return aux;
}

Matrix Matrix::forEach(std::function<double(double)> function){
    Matrix output(numRows, numCols);
    
    for(int i = 0; i < numRows; ++i){
        for(int j = 0; j < numCols; ++j){
            output.setValue(i, j, function.operator()(getValue(i, j)));
        }
    }

    return output;
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix){
    for(int i = 0; i < matrix.numRows; ++i){
        for(int j = 0; j < matrix.numCols; ++j){
            os << matrix.getValue(i, j) << "\t\t";
        }
        os << std::endl;
    }
    
    return os;
}

Matrix operator+(const Matrix& m1, const Matrix& m2){
    
    /*  Returns the elementwise sum of two matrices.
        Inputs: 
            m1: a vector
            m2: a vector
        Output: a vector, sum of the matrices m1 and m2.
    */
    
    Matrix sum(m1.numRows, m1.numCols);
    
    for(int i = 0; i < m1.numRows; ++i){
        for(int j = 0; j < m1.numCols; ++j){
            double v = m1.getValue(i, j) + m2.getValue(i, j);
            sum.setValue(i, j, v);
        }
    }
    
    return sum;
}

Matrix operator-(const Matrix& m1, const Matrix& m2){
    
    /*  Returns the difference between two matrices.
        Inputs:
            m1: vector
            m2: vector
        Output: vector, m1 - m2, difference between two matrices m1 and m2.
    */
    
    Matrix diff(m1.numRows, m1.numCols);
    
    for(int i = 0; i < m1.numRows; ++i){
        for(int j = 0; j < m1.numCols; ++j){
            double v = m1.getValue(i, j) - m2.getValue(i, j);
            diff.setValue(i, j, v);
        }
    }
    
    return diff;
}

Matrix operator*(const Matrix& m1, const Matrix& m2){
    
    /*  Returns the product of two matrices (elementwise multiplication).
        Inputs:
            m1: vector
            m2: vector
        Output: vector, m1 * m2, product of two matrices m1 and m2
    */
    
    Matrix mult(m1.numRows, m1.numCols);
    
    for(int i = 0; i < m1.numRows; ++i){
        for(int j = 0; j < m1.numCols; ++j){
            double v = m1.getValue(i, j) * m2.getValue(i, j);
            mult.setValue(i, j, v);
        }
    }
    
    return mult;
}

Matrix dot(const Matrix& m1, const Matrix& m2){
    
    /*  Returns the product of two matrices: m1 x m2.
        Inputs:
            m1: vector, left matrix of size m1_rows x m1_columns
            m2: vector, right matrix of size m1_columns x m2_columns (the number of rows in the right matrix 
                must be equal to the number of the columns in the left one)
            m1_rows: int, number of rows in the left matrix m1
            m1_columns: int, number of columns in the left matrix m1
            m2_columns: int, number of columns in the right matrix m2
        Output: vector, m1 * m2, product of two matrices m1 and m2, a matrix of size m1_rows x m2_columns
    */
    
    Matrix output(m1.numRows, m2.numCols);
    
    for(int row = 0; row < m1.numRows; ++row) {
        for(int col = 0; col < m2.numCols; ++col) {
            double v = 0.0;
            for( int k = 0; k < m1.numCols; ++k ) {
                v += m1.getValue(row, k) * m2.getValue(k, col);
            }
            output.setValue(row, col, v);
        }
    }
    
    return output;
}