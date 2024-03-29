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
    Shape(Material *material, char *name);
    Material *get_material();
    const char *name;
    Vector light_direction(Light *light, Point &intersection);
    Color calculate_color(Light *light, Point &observer, Point &intersection);
    virtual Vector surface_normal(Point &p_int) = 0;
    virtual bool intersects(Ray &ray, double &t_int) = 0;
    virtual void translate(Matrix t_matrix) = 0;
    virtual void rotate(Matrix t_matrix) = 0;
    virtual void scale(Matrix t_matrix) = 0;
    virtual Shape *clone() const = 0;
};

#endif