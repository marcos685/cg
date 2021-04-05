#ifndef PLANE_HPP
#define PLANE_HPP

#include "Shape.hpp"

class Plane : public Shape
{
private:
    Point p0_; // known point of the plane
    Vector n_; // unit vector orthogonal to the plane
public:
    Plane();
    Plane(Point p0, Vector n);
    Point get_p0();
    Vector get_n();
    void set_params(Point *p0, Vector *n);
    void uv(Point &p_int, double &u, double &v) override{};
    Vector surface_normal(Point &p_int) override { return n_; };
    bool intersects(Ray &ray, double &t_int) override;
    void translate(Matrix t_matrix) override;
    void rotate(Matrix t_matrix) override;
    void scale(Matrix t_matrix) override;
    virtual Plane *clone() const { return new Plane(*this); }
};

#endif