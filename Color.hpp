
#ifndef COLOR_HPP
#define COLOR_HPP

class Color
{
public:
    double r, g, b;
    Color();
    Color(double r, double g, double b);
    Color operator+(Color &intensity);
    Color &operator+=(Color intensity);
    Color operator*(Color &intensity);
    Color operator*(double &value);
    Color operator*(int &value);

private:
    void limit_range(double &value);
};

#endif