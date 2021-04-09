#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

class Object; // foward declaration, avoiding dependency cycle

#include "./objects/Shape.hpp"
#include "./objects/Object.hpp"
#include "Color.hpp"

class Intersection
{
public:
    Object *object_hit; // object cluster
    Shape *shape_hit;
    float tint;
    int index;
    Color color;
    Intersection()
    {
        object_hit = NULL;
        shape_hit = NULL;
        tint = 0;
        index = -1;
        color = Color();
    }
};

#endif