#ifndef CONE_HPP
#define CONE_HPP

#include "Plane.hpp"
#include "Shape.hpp"

class Cone : public Shape
{
private:
    Point c_;       // cone base center
    Point vertice_; // cone top vertice
    Vector n_;      // cone axis (unit vector)
    double height_;
    double radius_;
    Point scale_aux;

public:
    Cone();
    Cone(Point c, Vector n, double height, double radius, Material *material);
    Point *get_center();
    Point *get_vertice();
    Vector *get_axis();
    double *get_height();
    double *get_radius();
    void set_params(Point *p0, Point *v, Vector *ns, double *height, double *radius);

    Vector surface_normal(Point &p_int) override;
    bool intersects(Ray &ray, double &t_int) override;
    void translate(Matrix t_matrix) override;
    void rotate(Matrix t_matrix) override;
    void scale(Matrix t_matrix) override;
    virtual Cone *clone() const { return new Cone(*this); }
};

#endif