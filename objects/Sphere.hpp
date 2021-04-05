#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Shape.hpp"

class Sphere : public Shape
{
private:
    Point center_; // center of the sphere
    double radius_;
    Point scale_aux;

public:
    Sphere();
    Sphere(Point center, double radius, Material *material);
    Point get_center();
    double get_radius();
    void set_params(Point *center, double *radius);
    Vector surface_normal(Point &p_int) override;
    bool intersects(Ray &ray, double &t_int) override;
    void translate(Matrix t_matrix) override;
    void rotate(Matrix t_matrix) override;
    void scale(Matrix t_matrix) override;
    void uv(Point &p_int, double &u, double &v) override;
    virtual Sphere *clone() const { return new Sphere(*this); }
};

#endif