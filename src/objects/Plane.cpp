#include "Plane.hpp"

Plane::Plane() : Shape()
{
    p0_ = Point(); // known point of the plane
    n_ = Vector(); // unit vector perpendicular to the plane
}
Plane::Plane(Point p0, Vector n) : Shape()
{
    p0_ = p0;
    n_ = n;
    n_.normalize();
}

Point Plane::get_p0() { return p0_; }
Vector Plane::get_n() { return n_; }

void Plane::set_params(Point *p0, Vector *n)
{
    p0_ = *p0;
    n_ = *n;
}

bool Plane::intersects(Ray &ray, double &t_int)
{
    Point ray_p0 = ray.get_p0();
    Vector d = ray.get_d();
    if (d.dot_product(&n_) == 0)
        return false;
    Vector resultant = Vector(&ray_p0, &p0_);
    t_int = resultant.dot_product(&n_) / d.dot_product(&n_);
    return true;
}

void Plane::translate(Matrix t_matrix)
{
    p0_ = t_matrix * p0_;
    n_ = t_matrix * n_;
}

void Plane::rotate(Matrix t_matrix)
{
    n_ = t_matrix * n_;
    n_.normalize();
}

void Plane::scale(Matrix t_matrix)
{
    return;
}
