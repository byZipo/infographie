#include "Matrix.h"

#include <string>
#include <iostream>
#include <cmath>
#include <vector>
#include <cassert>

using namespace std;

Matrix::Matrix(int x, int y):  matrix(vector<vector<float> >(x, vector<float>(y, 0.f))), rows(x), cols(y) { }
Matrix Matrix::identity(int dimension){
    Matrix E(dimension, dimension);
    for(int i=0;i<dimension;i++){
        for(int j=0;j<dimension;j++){
            E[i][j] = (i==j ? 1.f : 0.f);
        }
    }
    return E;
}

vector<float>& Matrix::operator[](const int i) {
    assert(i>=0 && i<rows);
    return matrix[i];
}


Matrix Matrix::operator*(const Matrix& a) {
    assert(cols == a.rows);
    Matrix result(rows, a.cols);
    for (int i=0; i<rows; i++) {
        for (int j=0; j<a.cols; j++) {
            result.matrix[i][j] = 0.f;
            for (int k=0; k<cols; k++) {
                result.matrix[i][j] += matrix[i][k]*a.matrix[k][j];
            }
        }
    }
    return result;
}
Matrix Matrix::transpose() {
    Matrix result(cols, rows);
    for(int i=0; i<rows; i++)
        for(int j=0; j<cols; j++)
            result[j][i] = matrix[i][j];
    return result;
}

Matrix Matrix::inverse() {
    assert(rows==cols);
    // augmenting the square matrix with the identity matrix of the same dimensions A => [AI]
    Matrix result(rows, cols*2);
    for(int i=0; i<rows; i++)
        for(int j=0; j<cols; j++)
            result[i][j] = matrix[i][j];
    for(int i=0; i<rows; i++)
        result[i][i+cols] = 1;
    // first pass
    for (int i=0; i<rows-1; i++) {
        // normalize the first row
        for(int j=result.cols-1; j>=0; j--)
            result[i][j] /= result[i][i];
        for (int k=i+1; k<rows; k++) {
            float coeff = result[k][i];
            for (int j=0; j<result.cols; j++) {
                result[k][j] -= result[i][j]*coeff;
            }
        }
    }
    // normalize the last row
    for(int j=result.cols-1; j>=rows-1; j--)
        result[rows-1][j] /= result[rows-1][rows-1];
    // second pass
    for (int i=rows-1; i>0; i--) {
        for (int k=i-1; k>=0; k--) {
            float coeff = result[k][i];
            for (int j=0; j<result.cols; j++) {
                result[k][j] -= result[i][j]*coeff;
            }
        }
    }
    // cut the identity matrix back
    Matrix truncate(rows, cols);
    for(int i=0; i<rows; i++)
        for(int j=0; j<cols; j++)
            truncate[i][j] = result[i][j+cols];
    return truncate;
}

std::ostream& operator<<(std::ostream& s, Matrix& m) {
    for (int i=0; i<m.rows; i++)  {
        for (int j=0; j<m.cols; j++) {
            s << m[i][j];
            if (j<m.cols-1) s << "\t";
        }
        s << "\n";
    }
    return s;
}




