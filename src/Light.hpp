#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Color.hpp"
#include "./math/Point.hpp"
#include "./math/Vector.hpp"

enum LightType
{
    AMBIENT,
    REMOTE,
    POINT,
    SPOT
};

class Light
{
private:
    Color intensity_;
    Vector position_;
    LightType l_type;
    bool active_;
    double spot_angle;
    double spot_falloff;
    double spot_focus;
    Point spot_pos;

public:
    Light();
    Light(Color intensity, Vector position, LightType light_type = POINT);
    Light(double *Color, Vector position, LightType light_type = POINT);
    Light(double *Color, Point position, Vector lookat, double angle, double falloff_angle = 45, double focus = 1);
    void set_intensity(Color new_intensity);
    void set_intensity(double *Color);
    void set_position(Vector position);
    void set_spot(double *pos, double angle, double falloff, double focus);
    bool *active();
    Color get_intensity(Point &point);
    Vector get_position();
    Point get_spotpos();
    LightType type();
};

#endif