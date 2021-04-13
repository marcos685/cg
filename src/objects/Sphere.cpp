#include <cmath>
#include "Sphere.hpp"

using std::pow, std::sqrt, std::atan, std::asin;

Sphere::Sphere() : Shape()
{
    center_ = Point(); // unit vector perpendicular to the plane
    radius_ = 1;
    scale_aux = Point(radius_, radius_, radius_);
}
Sphere::Sphere(Point center, double radius, Material *material) : Shape(material)
{
    center_ = center;
    radius_ = radius;
    scale_aux = Point(radius_, radius_, radius_);
}

Point Sphere::get_center() { return center_; }
double Sphere::get_radius() { return radius_; }

void Sphere::set_params(Point *center, double *radius)
{
    center_ = *center;
    radius_ = *radius;
}

Vector Sphere::surface_normal(Point &p_int)
{
    Vector normal = Vector(&center_, &p_int);
    normal = normal / radius_;
    normal.normalize();
    return normal;
}

bool Sphere::intersects(Ray &ray, double &t_min)
{
    Point ray_p0 = ray.get_p0();
    Vector ray_d = ray.get_d();
    Vector c_rp0 = Vector(&center_, &ray_p0);
    double a = ray_d.dot_product(&ray_d);
    double b = c_rp0.dot_product(&ray_d);
    double c = c_rp0.dot_product(&c_rp0) - pow(radius_, 2);
    double delta = pow(b, 2) - a * c;
    if (delta < 0)
        return false;
    double t_int0 = (-b + sqrt(delta)) / a;
    double t_int1 = (-b - sqrt(delta)) / a;
    t_min = t_int0 < t_int1 ? t_int0 : t_int1;
    return true;
}

void Sphere::translate(Matrix t_matrix)
{
    center_ = t_matrix * center_;
}

void Sphere::scale(Matrix t_matrix)
{
    center_ = t_matrix * center_;
    radius_ = t_matrix(0, 0) * radius_;
}

void Sphere::rotate(Matrix t_matrix)
{
    return;
}
