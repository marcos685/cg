#ifndef RAY_HPP
#define RAY_HPP

#include "Point.hpp"
#include "Vector.hpp"

class Ray
{
private:
    Point p0_; // known point of the ray
    Vector d_; // unit vector that determines the ray's direction
public:
    Ray();
    Ray(Point p0, Vector d);
    Point calc_point(double t);
    Point get_p0();
    Vector get_d();
    void set_params(Point *p0, Vector *d);
};

#endif