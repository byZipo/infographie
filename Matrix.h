#ifndef MATRIX_H
#define MATRIX_H



#include <string>
#include <iostream>
#include <cmath>
#include <vector>


using namespace std;
class Matrix
{
    vector<vector<float> > matrix;
    int rows;
    int cols;
    public:
        Matrix(int x, int y);
        static Matrix identity(int dimension);
        vector<float>& operator[](const int i);
        Matrix operator*(const Matrix& a);
        Matrix transpose();
        Matrix inverse();

        friend std::ostream& operator<<(std::ostream& s, Matrix& m);
    protected:
    private:
};

#endif // MATRIX_H
