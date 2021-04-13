
#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>

#include "Point.hpp"
#include "Vector.hpp"

class Matrix
{
private:
    double m[4][4];

public:
    Matrix();
    Matrix operator+(Matrix &m);
    Matrix operator-(Matrix &m);
    Matrix operator*(Matrix &m);
    double &operator()(int i, int j);
    Point operator*(Point &p);
    Vector operator*(Vector &v);
    void identity();
    Matrix inverse();
    friend std::ostream &operator<<(std::ostream &stream, Matrix &matrix);
};

#endif