#include "Cylinder.hpp"
#include <cmath>
#include <vector>
#include <iostream>

using std::pow, std::sqrt, std::vector;

Cylinder::Cylinder() : Shape()
{
    b_ = Point();
    u_ = Vector(0, 1, 0);
    height_ = 1;
    radius_ = 1;
    scale_aux = Point(radius_, height_, radius_);
    u_.normalize();
}
Cylinder::Cylinder(Point b, Vector u, double height, double radius, Material *material) : Shape(material)
{
    b_ = b;
    u_ = u;
    height_ = height;
    radius_ = radius;
    scale_aux = Point(radius_, height_, radius_);
}
Point *Cylinder::get_center() { return &b_; }
Vector *Cylinder::get_axis() { return &u_; }
double *Cylinder::get_radius() { return &radius_; }
double *Cylinder::get_height() { return &height_; }

void Cylinder::set_params(Point *b, Vector *u, double *height, double *radius)
{
    b_ = *b;
    u_ = *u;
    height_ = *height;
    radius_ = *radius;
}

Vector Cylinder::surface_normal(Point &p_int)
{

    Vector s1 = u_ * Vector(&b_, &p_int).dot_product(&u_);
    Point sp_int = Point(
        b_.get_x() + s1.get_x(),
        b_.get_y() + s1.get_y(),
        b_.get_z() + s1.get_z());
    Vector normal = Vector(&sp_int, &p_int);
    normal.normalize();
    return normal;
}

bool Cylinder::intersects(Ray &ray, double &t_min)
{
    Point ray_p0 = ray.get_p0();
    Vector ray_d = ray.get_d();

    Vector bp0 = Vector(&b_, &ray_p0);

    Vector section1 = u_ * bp0.dot_product(&u_);
    Vector v = bp0 - section1;
    Vector section2 = (u_ * ray_d.dot_product(&u_));
    Vector w = ray_d - section2;

    double a = w.dot_product(&w);
    double b = v.dot_product(&w);
    double c = v.dot_product(&v) - pow(radius_, 2);
    double delta = pow(b, 2) - a * c;

    if (delta < 0)
    {
        return false;
    }
    double t_int0 = (-b + sqrt(delta)) / a;
    double t_int1 = (-b - sqrt(delta)) / a;

    Point p1 = ray.calc_point(t_int0);
    Point p2 = ray.calc_point(t_int1);
    double p1_dotproduct = Vector(&b_, &p1).dot_product(&u_);
    double p2_dotproduct = Vector(&b_, &p2).dot_product(&u_);

    vector<double> intersections;

    if (t_int0 >= 0 && (0 <= p1_dotproduct && p1_dotproduct <= height_))
        intersections.push_back(t_int0);
    if (t_int1 >= 0 && (0 <= p2_dotproduct && p2_dotproduct <= height_))
        intersections.push_back(t_int1);

    // no/one intersections with cylinder surface
    // yet there may have intersections with the caps
    if ((int)intersections.size() < 2)
    {
        double x, y, z;
        u_.get_coordinates(&x, &y, &z);
        Point top_center = Point(x * height_, y * height_, z * height_);
        Plane base_plane = Plane(b_, u_);
        Plane top_plane = Plane(top_center, u_);
        double t_base, t_top;
        bool base_intersection = base_plane.intersects(ray, t_base);
        bool top_intersection = top_plane.intersects(ray, t_top);
        if (base_intersection)
        {
            Point p_base = ray.calc_point(t_base);
            Vector cbase = Vector(&b_, &p_base);
            if (t_base >= 0 && cbase.norm() < radius_)
                intersections.push_back(t_base);
        }
        if (top_intersection)
        {
            Point p_base = ray.calc_point(t_top);
            Vector ctop = Vector(&b_, &p_base);
            if (t_top >= 0 && ctop.norm() < radius_)
                intersections.push_back(t_top);
        }
    }
    int int_candidates = (int)intersections.size();

    t_min = int_candidates != 0 ? intersections[0] : t_min;
    for (int i = 1; i < int_candidates; i++)
        if (intersections[i] < t_min)
            t_min = intersections[i];

    return int_candidates >= 1;
}

void Cylinder::translate(Matrix t_matrix)
{
    b_ = t_matrix * b_;
}

void Cylinder::rotate(Matrix t_matrix)
{
    u_ = t_matrix * u_;
}

void Cylinder::scale(Matrix t_matrix)
{
    height_ = height_ * t_matrix(0, 0);
    radius_ = radius_ * t_matrix(0, 0);
}
