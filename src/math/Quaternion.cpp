#include <cmath>

#include "Quaternion.hpp"
#include "Matrix.hpp"

using std::pow, std::sqrt;

Quaternion::Quaternion()
{
    v_ = Vector();
    w_ = 1;
}

Quaternion::Quaternion(Vector v, double w)
{
    v_ = v;
    w_ = w;
}

Quaternion::Quaternion(double x, double y, double z, double w)
{
    v_ = Vector(x, y, z);
    w_ = w;
}

Vector *Quaternion::get_vec() { return &v_; }
double Quaternion::get_scalar() { return w_; }

void Quaternion::set_vec(Vector &v) { v_ = v; }
void Quaternion::set_scalar(double w) { w_ = w; }
void Quaternion::set(double x, double y, double z, double w)
{
    v_ = Vector(x, y, z);
    w_ = w;
}

double Quaternion::length()
{
    return sqrt(pow(v_.norm(), 2) + w_ * w_);
}

Quaternion Quaternion::conjugate() { return Quaternion(v_ * (-1), w_); }

Quaternion Quaternion::operator*(Quaternion &qr)
{
    Matrix tr_matrix = Matrix();
    double vlx, vly, vlz;
    v_.get_coordinates(&vlx, &vly, &vlz);

    tr_matrix(0, 0) = w_;
    tr_matrix(0, 1) = -vlz;
    tr_matrix(0, 2) = vly;
    tr_matrix(0, 3) = vlx;

    tr_matrix(1, 0) = vlz;
    tr_matrix(1, 1) = w_;
    tr_matrix(1, 2) = -vlx;
    tr_matrix(1, 3) = vly;

    tr_matrix(2, 0) = -vly;
    tr_matrix(2, 1) = vlx;
    tr_matrix(2, 2) = w_;
    tr_matrix(2, 3) = vlz;

    tr_matrix(3, 0) = -vlx;
    tr_matrix(3, 1) = -vly;
    tr_matrix(3, 2) = -vlz;
    tr_matrix(3, 3) = w_;

    return tr_matrix * qr;
}
