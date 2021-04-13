#include <cmath>

#include "Triangle.hpp"

using std::abs;

Triangle::Triangle() : Shape()
{
    vertex0 = Point(1, 0, 0);
    vertex1 = Point(-1, 0, 0);
    vertex2 = Point(0, 1, 0);
}
Triangle::Triangle(Point vertex0, Point vertex1, Point vertex2, Material *material) : Shape(material, "triangle")
{
    this->vertex0 = vertex0;
    this->vertex1 = vertex1;
    this->vertex2 = vertex2;
}
Triangle::Triangle(double *vertex0, double *vertex1, double *vertex2, Material *material) : Shape(material)
{
    this->vertex0 = Point(vertex0);
    this->vertex1 = Point(vertex1);
    this->vertex2 = Point(vertex2);
}
void Triangle::set_vertices(double *vertex0, double *vertex1, double *vertex2)
{
    this->vertex0 = Point(vertex0);
    this->vertex1 = Point(vertex1);
    this->vertex2 = Point(vertex2);
}
Point *Triangle::operator[](int i)
{
    switch (i)
    {
    case 0:
        return &vertex0;
        break;
    case 1:
        return &vertex1;
    case 2:
        return &vertex2;
    default:
        return nullptr;
        break;
    }
}

Vector Triangle::surface_normal(Point &p_int)
{
    Vector edge1 = Vector(&vertex0, &vertex1);
    Vector edge2 = Vector(&vertex0, &vertex2);
    Vector normal = edge1.cross_product(&edge2);
    normal.normalize();
    return normal;
}

bool Triangle::intersects(Ray &ray, double &t_int)
{
    double episilon = 0.0001;
    // Möller–Trumbore intersection
    Point ray_p0 = ray.get_p0();
    Vector ray_d = ray.get_d();
    Vector edge1, edge2, h, s, q;
    double a, f, u, v;

    edge1 = Vector(&vertex0, &vertex1);
    edge2 = Vector(&vertex0, &vertex2);
    h = ray_d.cross_product(&edge2);
    a = edge1.dot_product(&h);
    if (abs(a) < episilon)
        return false;

    f = 1.0 / a;
    s = Vector(&vertex0, &ray_p0);
    u = f * s.dot_product(&h);
    if (u < 0.0 || u > 1.0)
        return false;

    q = s.cross_product(&edge1);
    v = f * ray_d.dot_product(&q);
    if (v < 0.0 || u + v > 1.0)
        return false;

    t_int = f * edge2.dot_product(&q);
    if (t_int <= episilon)
        return false;
    return true;
}

void Triangle::translate(Matrix t_matrix)
{
    vertex0 = t_matrix * vertex0;
    vertex1 = t_matrix * vertex1;
    vertex2 = t_matrix * vertex2;
}

void Triangle::rotate(Matrix t_matrix)
{
    vertex0 = t_matrix * vertex0;
    vertex1 = t_matrix * vertex1;
    vertex2 = t_matrix * vertex2;
}

void Triangle::scale(Matrix t_matrix)
{
    vertex0 = t_matrix * vertex0;
    vertex1 = t_matrix * vertex1;
    vertex2 = t_matrix * vertex2;
}