#include "Light.hpp"
#include <cmath>

Light::Light()
{
    intensity_.r = 0;
    intensity_.g = 0;
    intensity_.b = 0;
    position_ = Vector();
    l_type = AMBIENT;
    active_ = true;
}

Light::Light(Color intensity, Vector position, LightType light_type)
{
    position_ = position;
    intensity_ = intensity;
    l_type = light_type;
    active_ = true;
}

Light::Light(double *color, Vector position, LightType light_type)
{
    position_ = position;
    intensity_.r = color[0];
    intensity_.g = color[1];
    intensity_.b = color[2];
    l_type = light_type;
    active_ = true;
}

Light::Light(double *color, Point position, Vector lookat, double angle, double falloff_angle, double focus)
{
    intensity_.r = color[0];
    intensity_.g = color[1];
    intensity_.b = color[2];
    position_ = lookat;
    spot_pos = position;
    spot_angle = angle;
    spot_falloff = falloff_angle;
    spot_focus = focus;
    l_type = SPOT;
    active_ = true;
}

Light::Light(Color color, Point position, Vector lookat, double angle, double falloff_angle, double focus)
{
    intensity_= color;
    position_ = lookat;
    spot_pos = position;
    spot_angle = angle;
    spot_falloff = falloff_angle;
    spot_focus = focus;
    l_type = SPOT;
    active_ = true;
}

void Light::set_intensity(Color intensity)
{
    intensity_ = intensity;
}

void Light::set_intensity(double *color)
{
    intensity_.r = color[0];
    intensity_.g = color[1];
    intensity_.b = color[2];
}

Color Light::get_intensity(Point &point)
{
    if (!active_)
        return Color();

    if (l_type == SPOT)
    {
        Vector lpos_p = Vector(&spot_pos, &point);
        lpos_p.normalize();

        double angle = std::acos(lpos_p.dot_product(&position_));
        if (angle > spot_angle + spot_falloff)
            return Color();
        if (angle > spot_angle)
        {
            double gradient = 1 - (spot_focus * (angle - spot_angle)) / spot_falloff;
            return Color(intensity_.r * gradient, intensity_.r * gradient, intensity_.r * gradient);
        }
    }

    return intensity_;
}

void Light::set_position(Vector position) { position_ = position; }
bool *Light::active() { return &active_; }
Vector Light::get_position() { return position_; }
void Light::set_spot(double *pos, double angle, double falloff, double focus)
{
    this->spot_pos = Point(pos);
    this->spot_angle = angle;
    this->spot_falloff = falloff;
    this->spot_focus = focus;
}
Point Light::get_spotpos()
{
    if (l_type == SPOT)
        return spot_pos;
    else
        return Point();
}
LightType Light::type() { return l_type; }