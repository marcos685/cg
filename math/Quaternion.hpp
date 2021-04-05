#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "Vector.hpp"

class Quaternion
{
private:
    Vector v_;
    double w_;

public:
    Quaternion();
    Quaternion(Vector v, double w);
    Quaternion(double x, double y, double z, double w);
    Vector *get_vec();
    double get_scalar();
    void set_vec(Vector &v);
    void set_scalar(double w);
    void set(double x, double y, double z, double w);
    double length();
    Quaternion conjugate();
    Quaternion operator*(Quaternion &qr);
};

#endif