#include <cmath>
#include "Point.hpp"

using std::pow, std::sqrt;

Point::Point()
{
    x_ = 0;
    y_ = 0;
    z_ = 0;
}
Point::Point(double x, double y, double z)
{
    x_ = x;
    y_ = y;
    z_ = z;
}
Point::Point(double *coords)
{
    x_ = coords[0];
    y_ = coords[1];
    z_ = coords[2];
}

double Point::get_x() { return x_; }
double Point::get_y() { return y_; }
double Point::get_z() { return z_; }

void Point::get_coordinates(double *x, double *y, double *z)
{
    *x = x_;
    *y = y_;
    *z = z_;
}

void Point::set_coordinates(double x, double y, double z)
{
    x_ = x;
    y_ = y;
    z_ = z;
}

void Point::to_array(double *arr)
{
    arr[0] = x_;
    arr[1] = y_;
    arr[2] = z_;
}

double Point::distance_from(Point *p)
{
    double x, y, z;
    p->get_coordinates(&x, &y, &z);
    return sqrt(pow(x_ - x, 2) + pow(y_ - y, 2) + pow(z_ - z, 2));
}

Point Point::operator+(Point &p)
{
    return Point(x_ + p.get_x(), y_ + p.get_y(), z_ + p.get_z());
}

Point Point::operator-(Point &p)
{
    return Point(x_ - p.get_x(), y_ - p.get_y(), z_ - p.get_z());
}

Point Point::operator*(double value)
{
    return Point(x_ * value, y_ * value, z_ * value);
}
