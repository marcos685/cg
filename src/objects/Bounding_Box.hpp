#ifndef Bounding_Box_HPP
#define Bounding_Box_HPP

#include "Shape.hpp"

class Bounding_Box : public Shape
{
private:
    Point max_bound;
    Point min_bound;
    Matrix modelmatrix;
    Matrix invmatrix;

public:
    Bounding_Box();
    Bounding_Box(Point min_bound, Point max_bound);     // Bounding box 1
    Bounding_Box(double *min_bound, double *max_bound); // Bounding box 1
    Point get_ref();
    void bounds(Point &minb, Point &maxb);
    void set_bounds(Point &minb, Point &maxb);
    void origin_translate(Matrix t_matrix);
    Vector surface_normal(Point &p_int) override;
    bool intersects(Ray &ray, double &t_int) override;
    void translate(Matrix t_matrix) override;
    void rotate(Matrix t_matrix) override;
    void scale(Matrix t_matrix) override;
    virtual Bounding_Box *clone() const { return new Bounding_Box(*this); }
};

#endif