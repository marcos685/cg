
#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "Point.hpp"

class Vector
{
private:
    Point origin_;
    double x_, y_, z_;

public:
    Vector();
    Vector(double x, double y, double z);
    Vector(Point *o, double x, double y, double z);
    Vector(double *coords);
    Vector(Point *start, Point *end);
    double get_x();
    double get_y();
    double get_z();
    void get_coordinates(double *x, double *y, double *z);
    void set_coordinates(double x, double y, double z);
    void to_array(double *arr);
    Vector operator+(Vector &v);
    Vector operator-(Vector &v);
    Vector operator*(double value);
    Vector operator*(Vector &v);
    Vector operator/(double value);
    Vector &operator+=(Vector &v);
    Vector &operator-=(Vector &v);
    Vector &operator*=(Vector &v);
    Vector &operator/=(Vector &v);
    double norm();
    void normalize();
    double dot_product(Vector *v);
    Vector cross_product(Vector *v);
    bool is_orthogonal(Vector *v);
};

#endif