
#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "Point.hpp"
#include "Vector.hpp"
#include "Quaternion.hpp"

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
    Quaternion operator*(Quaternion &q);
    void identity();
    Matrix inverse();
};

#endif