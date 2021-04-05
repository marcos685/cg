#include <cmath>

#include "Point.hpp"
#include "Vector.hpp"

using std::pow, std::sqrt;

Vector::Vector()
{
    x_ = 0;
    y_ = 0;
    z_ = 0;
}

Vector::Vector(double x, double y, double z)
{
    origin_ = Point();
    x_ = x;
    y_ = y;
    z_ = z;
}

Vector::Vector(double *coords)
{
    x_ = coords[0];
    y_ = coords[1];
    z_ = coords[2];
}

Vector::Vector(Point *start, Point *end)
{
    origin_ = *start;
    x_ = end->get_x() - start->get_x();
    y_ = end->get_y() - start->get_y();
    z_ = end->get_z() - start->get_z();
}

double Vector::get_x() { return x_; }
double Vector::get_y() { return y_; }
double Vector::get_z() { return z_; }

void Vector::get_coordinates(double *x, double *y, double *z)
{
    *x = x_;
    *y = y_;
    *z = z_;
}

void Vector::set_coordinates(double x, double y, double z)
{
    x_ = x;
    y_ = y;
    z_ = z;
}

void Vector::to_array(double *arr)
{
    arr[0] = x_;
    arr[1] = y_;
    arr[2] = z_;
}

double Vector::dot_product(Vector *v)
{
    return x_ * v->get_x() + y_ * v->get_y() + z_ * v->get_z();
}

double Vector::norm() { return sqrt(dot_product(this)); }

void Vector::normalize()
{
    double norm = this->norm();
    x_ /= norm;
    y_ /= norm;
    z_ /= norm;
}

Vector Vector::operator+(Vector &v)
{
    return Vector(x_ + v.get_x(), y_ + v.get_y(), z_ + v.get_z());
}

Vector Vector::operator-(Vector &v)
{
    return Vector(x_ - v.get_x(), y_ - v.get_y(), z_ - v.get_z());
}

Vector Vector::operator*(double s)
{
    return Vector(x_ * s, y_ * s, z_ * s);
}

Vector Vector::operator/(double s)
{
    return Vector(x_ / s, y_ / s, z_ / s);
}

Vector &Vector::operator+=(Vector &v)
{
    x_ += v.get_x();
    y_ += v.get_y();
    z_ += v.get_z();
    return *this;
}

Vector &Vector::operator-=(Vector &v)
{
    x_ -= v.get_x();
    y_ -= v.get_y();
    z_ -= v.get_z();
    return *this;
}

Vector &Vector::operator*=(Vector &v)
{
    x_ *= v.get_x();
    y_ *= v.get_y();
    z_ *= v.get_z();
    return *this;
}

Vector &Vector::operator/=(Vector &v)
{
    x_ /= v.get_x();
    y_ /= v.get_y();
    z_ /= v.get_z();
    return *this;
}

Vector Vector::cross_product(Vector *v)
{
    return Vector(y_ * v->get_z() - z_ * v->get_y(), z_ * v->get_x() - x_ * v->get_z(), x_ * v->get_y() - y_ * v->get_x());
}

bool Vector::is_orthogonal(Vector *v)
{
    return dot_product(v) == 0;
}