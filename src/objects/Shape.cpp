#include "Shape.hpp"
#include <cmath>
#include <iostream>

using std::pow;

Shape::Shape()
{
    this->material_ = new Material();
    this->name = "shape";
}

Shape::Shape(Material *material)
{
    this->material_ = material;
    this->name = "shape";
};

Shape::Shape(Material *material, char *name)
{
    this->material_ = material;
    this->name = name;
};

Material *Shape::get_material() { return material_; }

Vector Shape::light_direction(Light *light, Point &intersection)
{
    Vector ld = light->get_position();
    Point light_point = Point(ld.get_x(), ld.get_y(), ld.get_z());
    switch (light->type())
    {
    case AMBIENT:
        return Vector();
        break;
    case SPOT:
    case REMOTE:
        ld = ld * -1;
        break;
    default:
        ld = Vector(&intersection, &light_point);
        break;
    }
    ld.normalize();
    return ld;
}

Color Shape::calculate_diffuse(Light *light, Point &intersection)
{
    if (light->type() == AMBIENT)
        return Color();

    Vector ld = light_direction(light, intersection);
    Color intensity = light->get_intensity(intersection);
    Vector normal = surface_normal(intersection);
    normal.normalize();
    ld.normalize();

    //std::cout << normal.get_x() << '|' << normal.get_y() << '|' << normal.get_z() << '\n';

    double fd = normal.dot_product(&ld);
    fd = fd < 0 ? 0 : fd;

    Color Id = intensity * material_->diffuse * fd;
    return Id;
}

Color Shape::calculate_specular(Light *light, Point &observer, Point &intersection)
{
    if (light->type() == AMBIENT)
        return Color();

    Vector ld = light_direction(light, intersection);
    Color intensity = light->get_intensity(intersection);
    Vector normal = surface_normal(intersection);
    Vector v = Vector(&intersection, &observer);
    Vector r = normal * normal.dot_product(&ld) * 2 - ld;
    v.normalize();
    r.normalize();

    double vr_product = v.dot_product(&r);
    vr_product = vr_product <= 0 ? 0.0f : vr_product;
    double fs = pow(vr_product, material_->glossiness);

    Color Is = intensity * material_->specular * fs;
    return Is;
}

Color Shape::calculate_color(Light *light, Point &observer, Point &intersection)
{
    if (light->type() == AMBIENT)
        return light->get_intensity(intersection) * material_->ambient;
    Color Id = calculate_diffuse(light, intersection);
    Color Is = calculate_specular(light, observer, intersection);
    return Id + Is;
}