#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "Shape.hpp"

class Triangle : public Shape
{
private:
    Point vertex0;
    Point vertex1;
    Point vertex2;

public:
    Triangle();
    Triangle(Point vertex0, Point vertex1, Point vertex2, Material *material);
    Triangle(double *vertex0, double *vertex1, double *vertex2, Material *material);
    void set_vertices(double *vertex0, double *vertex1, double *vertex2);
    Point *operator[](int i);

    Vector surface_normal(Point &p_int) override;
    bool intersects(Ray &ray, double &t_int) override;
    void translate(Matrix t_matrix) override;
    void rotate(Matrix t_matrix) override;
    void scale(Matrix t_matrix) override;
    virtual Triangle *clone() const { return new Triangle(*this); }
};

#endif