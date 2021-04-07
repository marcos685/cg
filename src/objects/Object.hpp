#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>

#include "Bounding_Box.hpp"
#include "Shape.hpp"

class Object
{
private:
    Bounding_Box bounding_box_;
    std::vector<Shape *> mesh_;
    Point pivot;
    bool visible_;

public:
    const char *name;
    Object(const char *name, Bounding_Box bounding_box, std::vector<Shape *> mesh, bool visible = true);
    Object(const char *name, const char *obj_path, Material *material, bool visible = true);
    Object *clone(const char *n_name = NULL);
    void include(const char *obj_path, Material *material);
    void get(Bounding_Box &bb, std::vector<Shape *> &mesh);
    void set_visible(bool visible);
    bool visible();
    bool *visible_ptr();
    bool trace(Ray &ray, Shape **shape_hit, double &t_int);
    void translate(Vector t_vec);
    void scale(double sx, double sy, double sz);
    void rotate(double angle, Vector axis);
};

#endif