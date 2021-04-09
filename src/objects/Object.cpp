#include <limits>
#include <cmath>
#include <iostream>

#include "Object.hpp"
#include "Triangle.hpp"

Object::Object(const char *name, Bounding_Box bounding_box, std::vector<Shape *> mesh, bool visible)
{
    this->bounding_box_ = bounding_box;
    this->name = name;
    this->visible_ = visible;
    for (unsigned i = 0; i < mesh.size(); i++)
        mesh_.push_back(mesh[i]->clone());
}

Object::Object(const char *name, const char *obj_path, Material *material, bool visible)
{
    this->name = name;
    this->visible_ = visible;
    Point minb;
    Point maxb;
    this->bounding_box_ = Bounding_Box(minb, maxb);
}

void Object::get(Bounding_Box &bb, std::vector<Shape *> &mesh)
{
    bb = bounding_box_;
    mesh = mesh_;
}

Object *Object::clone(const char *n_name)
{
    if (n_name)
        return new Object(n_name, bounding_box_, mesh_, visible_);
    else
        return new Object(name, bounding_box_, mesh_, visible_);
}

void Object::set_visible(bool visible) { visible_ = visible; }
bool Object::visible() { return visible_; }
bool *Object::visible_ptr() { return &visible_; }

bool Object::trace(Ray &ray, Shape **shape_hit, double &t_int)
{
    double t_min = std::numeric_limits<double>::infinity();
    bool hit = false;

    if (!visible_ || !bounding_box_.intersects(ray, t_int))
    {
        return hit;
    }

    for (unsigned i = 0; i < mesh_.size(); i++)
    {

        if (mesh_[i]->intersects(ray, t_int) && t_int >= 0 && t_int < t_min)
        {
            t_min = t_int;
            *shape_hit = mesh_[i];

            hit = true;
        }
    }
    return hit;
}

void Object::translate(Vector t_vec)
{
    Matrix translation_m;
    translation_m.identity();
    translation_m(0, 3) = t_vec.get_x();
    translation_m(1, 3) = t_vec.get_y();
    translation_m(2, 3) = t_vec.get_z();
    bounding_box_.translate(translation_m);
    for (unsigned i = 0; i < mesh_.size(); i++)
        mesh_[i]->translate(translation_m);
}

void Object::scale(double sx, double sy, double sz)
{
    Matrix scale_m;
    scale_m(0, 0) = sx;
    scale_m(1, 1) = sy;
    scale_m(2, 2) = sz;
    scale_m(3, 3) = 1;
    bounding_box_.scale(scale_m);
    for (unsigned i = 0; i < mesh_.size(); i++)
        mesh_[i]->scale(scale_m);
}

void Object::rotate(double angle, Vector axis)
{
    Point origin;
    Point ref = bounding_box_.get_ref();
    Vector p_orig = Vector(&ref, &origin);
    Matrix to_origin;
    to_origin.identity();
    to_origin(0, 3) = p_orig.get_x();
    to_origin(1, 3) = p_orig.get_y();
    to_origin(2, 3) = p_orig.get_z();
    Matrix from_origin = to_origin.inverse();

    angle /= 2;
    Vector qrv = axis * std::sin(angle);
    double wq = std::cos(angle);
    double vqx, vqy, vqz;
    qrv.get_coordinates(&vqx, &vqy, &vqz);

    Matrix rotation_m;
    // omitted fields are set to 0 by default
    rotation_m(0, 0) = vqx * vqx - vqy * vqy - vqz * vqz + wq * wq;
    rotation_m(0, 1) = 2 * vqx * vqy - 2 * vqz * wq;
    rotation_m(0, 2) = 2 * vqx * vqz + 2 * vqy * wq;

    rotation_m(1, 0) = 2 * vqz * vqy + 2 * vqz * wq;
    rotation_m(1, 1) = -(vqx * vqx) + vqy * vqy - vqz * vqz + wq * wq;
    rotation_m(1, 2) = 2 * vqy * vqz - 2 * vqx * wq;

    rotation_m(2, 0) = 2 * vqx * vqz - 2 * vqy * wq;
    rotation_m(2, 1) = 2 * vqy * vqz + 2 * vqx * wq;
    rotation_m(2, 2) = -(vqx * vqx) - vqy * vqy + vqz * vqz + wq * wq;

    rotation_m(3, 3) = 1;

    bounding_box_.translate(to_origin);
    for (unsigned i = 0; i < mesh_.size(); i++)
        mesh_[i]->translate(to_origin);

    bounding_box_.rotate(rotation_m);
    for (unsigned i = 0; i < mesh_.size(); i++)
        mesh_[i]->rotate(rotation_m);

    bounding_box_.translate(from_origin);
    for (unsigned i = 0; i < mesh_.size(); i++)
        mesh_[i]->translate(from_origin);
}