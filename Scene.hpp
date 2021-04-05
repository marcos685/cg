#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <limits>
#include <GL/freeglut.h>

#include "Color.hpp"
#include "./objects/Object.hpp"
#include "Camera.hpp"
#include "Light.hpp"

using std::numeric_limits, std::vector;

class Scene
{
private:
    int resolution;
    double width;
    double distance;
    Camera *camera;
    vector<Light *> lights;
    void set_pixel(GLubyte *pixels, int x, int y, Color Color);

public:
    vector<Object *> objects;
    Scene(int resolution, Camera *camera, vector<Object *> objects, vector<Light *> lights, double w = 6, double d = 3);
    Color trace_ray(int x, int y);
    void print(GLubyte *pixels);
};

#endif