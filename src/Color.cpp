#include "Color.hpp"

Color::Color()
{
    r = 0;
    g = 0;
    b = 0;
}
Color::Color(double r, double g, double b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

void Color::limit_range(double &value)
{
    if (value < 0)
        value = 0;
    if (value > 1)
        value = 1;
}

Color Color::operator+(Color &op_intensity)
{
    Color new_intensity;
    new_intensity.r = r + op_intensity.r;
    new_intensity.g = g + op_intensity.g;
    new_intensity.b = b + op_intensity.b;
    // limit_range(new_intensity.r);
    // limit_range(new_intensity.g);
    // limit_range(new_intensity.b);
    return new_intensity;
}
Color &Color::operator+=(Color op_intensity)
{
    r += op_intensity.r;
    g += op_intensity.g;
    b += op_intensity.b;
    // limit_range(r);
    // limit_range(g);
    // limit_range(b);
    return *this;
}
Color Color::operator*(Color &op_intensity)
{
    Color new_intensity;
    new_intensity.r = r * op_intensity.r;
    new_intensity.g = g * op_intensity.g;
    new_intensity.b = b * op_intensity.b;
    // limit_range(new_intensity.r);
    // limit_range(new_intensity.g);
    // limit_range(new_intensity.b);
    return new_intensity;
}
Color Color::operator*(double &value)
{
    Color new_intensity;
    new_intensity.r = r * value;
    new_intensity.g = g * value;
    new_intensity.b = b * value;
    // limit_range(new_intensity.r);
    // limit_range(new_intensity.g);
    // limit_range(new_intensity.b);
    return new_intensity;
}
Color Color::operator*(int &value)
{
    Color new_intensity;
    new_intensity.r = r * value;
    new_intensity.g = g * value;
    new_intensity.b = b * value;
    // limit_range(new_intensity.r);
    // limit_range(new_intensity.g);
    // limit_range(new_intensity.b);
    return new_intensity;
}