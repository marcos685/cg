#include <cmath>
#include "Ray.hpp"

Ray::Ray()
{
    p0_ = Point();
    d_ = Vector();
}
Ray::Ray(Point p0, Vector d)
{
    p0_ = p0;
    d_ = d;
    d_.normalize();
}

Point Ray::calc_point(double t)
{
    double p1, p2, p3, d1, d2, d3;
    p0_.get_coordinates(&p1, &p2, &p3);
    d_.get_coordinates(&d1, &d2, &d3);
    return Point(p1 + t * d1, p2 + t * d2, p3 + t * d3);
}

Point Ray::get_p0() { return p0_; }
Vector Ray::get_d() { return d_; }

void Ray::set_params(Point *p0, Vector *d)
{
    p0_ = *p0;
    d_ = *d;
}