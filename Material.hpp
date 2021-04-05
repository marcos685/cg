#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Color.hpp"
#include "math/Point.hpp"

class Material
{
private:
public:
    Color ambient;
    Color diffuse;
    Color specular;
    double glossiness;
    Material();
    Material(Color ambient, Color diffuse, Color specular, double glossiness = 1);
    void set_ambient(double *Color);
    void set_diffuse(double *Color);
    void set_specular(double *Color);
    void set_glossiness(double s);
};

#endif