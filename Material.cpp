#include "Material.hpp"

Material::Material()
{
    ambient = Color();
    diffuse = Color();
    specular = Color();
    glossiness = 0;
}
Material::Material(Color ambient, Color diffuse, Color specular, double glossiness)
{
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->glossiness = glossiness;
}

void Material::set_ambient(double *color)
{
    this->ambient.r = color[0];
    this->ambient.g = color[1];
    this->ambient.b = color[2];
}
void Material::set_diffuse(double *color)
{
    this->diffuse.r = color[0];
    this->diffuse.g = color[1];
    this->diffuse.b = color[2];
}
void Material::set_specular(double *color)
{
    this->specular.r = color[0];
    this->specular.g = color[1];
    this->specular.b = color[2];
}

void Material::set_glossiness(double s) { glossiness = s; }
