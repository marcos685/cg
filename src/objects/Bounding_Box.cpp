#include "Bounding_Box.hpp"
#include <cmath>
#include <algorithm>

using std::swap;

Bounding_Box::Bounding_Box() : Shape(new Material(Color(), Color(), Color()))
{
    this->min_bound = Point(-0.5, -0.5, -0.5);
    this->max_bound = Point(0.5, 0.5, 0.5);
    this->modelmatrix.identity();
}

Bounding_Box::Bounding_Box(Point min_bound, Point max_bound) : Shape(new Material(Color(0.5, 0.5, 0.5), Color(), Color()))
{
    this->min_bound = min_bound;
    this->max_bound = max_bound;
    this->modelmatrix.identity();
    this->invmatrix = modelmatrix;
}

Bounding_Box::Bounding_Box(double *min_bound, double *max_bound) : Shape(new Material(Color(0.5, 0.5, 0.5), Color(), Color()))
{
    this->min_bound = Point(min_bound);
    this->max_bound = Point(max_bound);
    this->modelmatrix.identity();
    this->invmatrix = modelmatrix;
}

Point Bounding_Box::get_ref()
{
    return Point(
        (max_bound.get_x() + min_bound.get_x()) / 2,
        (max_bound.get_y() + min_bound.get_y()) / 2,
        (max_bound.get_z() + min_bound.get_z()) / 2);
}

void Bounding_Box::bounds(Point &minb, Point &maxb)
{
    minb = this->min_bound;
    maxb = this->max_bound;
}
void Bounding_Box::set_bounds(Point &minb, Point &maxb)
{
    this->min_bound = minb;
    this->max_bound = maxb;
}
Vector Bounding_Box::surface_normal(Point &p_int) { return Vector(); }

bool Bounding_Box::intersects(Ray &ray, double &t_int)
{
    double tmin = 0.0f;
    double tmax = 100000.0f;
    Matrix invm = this->invmatrix;

    Point ray_p0 = ray.get_p0();
    Vector ray_d = ray.get_d();
    // Projects ray into object space
    ray_p0 = invm * ray_p0;
    ray_d = invm * ray_d;
    ray_d.normalize();

    double min_b[3], max_b[3], p0[3], d[3];
    min_bound.to_array(min_b);
    max_bound.to_array(max_b);
    ray_p0.to_array(p0);
    ray_d.to_array(d);

    // Checks intersection for each plane
    for (int i = 0; i < 3; i++)
    {
        double t1 = (max_b[i] - p0[i]) / d[i];
        double t2 = (min_b[i] - p0[i]) / d[i];
        if (t1 > t2)
            swap(t1, t2);
        if (t2 < tmax)
            tmax = t2;
        if (t1 > tmin)
            tmin = t1;
        if (tmax < tmin)
            return false;
    }
    t_int = tmin;
    return true;
}

void Bounding_Box::origin_translate(Matrix t_matrix)
{
    modelmatrix = t_matrix * modelmatrix;
    invmatrix = modelmatrix.inverse();
}

void Bounding_Box::translate(Matrix t_matrix)
{
    min_bound = t_matrix * min_bound;
    max_bound = t_matrix * max_bound;
}
void Bounding_Box::rotate(Matrix t_matrix)
{
    modelmatrix = t_matrix * modelmatrix;
}
void Bounding_Box::scale(Matrix t_matrix)
{
    min_bound = t_matrix * min_bound;
    max_bound = t_matrix * max_bound;
}