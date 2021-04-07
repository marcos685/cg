#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include "Plane.hpp"
#include "Shape.hpp"

class Cylinder : public Shape
{
private:
    Point b_;  // center of cylinder's base
    Vector u_; // cylinder axis (unit vector)
    double height_;
    double radius_;
    Point scale_aux;

public:
    Cylinder();
    Cylinder(Point b, Vector u, double height, double radius, Material *material);
    Point *get_center();
    Vector *get_axis();
    double *get_radius();
    double *get_height();
    void set_params(Point *b, Vector *u, double *height, double *radius);
    Vector surface_normal(Point &p_int) override;
    bool intersects(Ray &ray, double &t_int) override;
    void translate(Matrix t_matrix) override;
    void rotate(Matrix t_matrix) override;
    void scale(Matrix t_matrix) override;
    virtual Cylinder *clone() const { return new Cylinder(*this); }
};

#endif