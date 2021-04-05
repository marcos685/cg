#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "../Material.hpp"
#include "../Light.hpp"
#include "../math/Matrix.hpp"
#include "../math/Ray.hpp"

class Shape
{
private:
    Material *material_;
    Color calculate_diffuse(Light *light, Point &intersection);
    Color calculate_specular(Light *light, Point &observer, Point &intersection);

public:
    Shape();
    Shape(Material *material);
    Material *get_material();
    Vector light_direction(Light *light, Point &intersection);
    Color calculate_color(Light *light, Point &observer, Point &intersection);
    virtual void uv(Point &p_int, double &u, double &v) = 0;
    virtual Vector surface_normal(Point &p_int) = 0;
    virtual bool intersects(Ray &ray, double &t_int) = 0;
    virtual void translate(Matrix t_matrix) = 0;
    virtual void rotate(Matrix t_matrix) = 0;
    virtual void scale(Matrix t_matrix) = 0;
    virtual Shape *clone() const = 0;
};

#endif