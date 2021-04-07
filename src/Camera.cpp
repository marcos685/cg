#include "Camera.hpp"

Camera::Camera()
{
    eye_ = Point(0, 0, 0);
    lookat_ = Point(0, 0, 1);
    up_ = Point(0, 1, 0);
}

Camera::Camera(Point eye, Point lookat, Point up)
{
    eye_ = eye;
    lookat_ = lookat;
    up_ = up;
    update();
}
Camera::Camera(double *eye, double *lookat, double *up)
{
    eye_ = Point(eye);
    lookat_ = Point(lookat);
    up_ = Point(up);
    update();
}

void Camera::calculate_matrices()
{
    camtoworld(0, 0) = cx_.get_x();
    camtoworld(1, 0) = cx_.get_y();
    camtoworld(2, 0) = cx_.get_z();
    camtoworld(3, 0) = 0;

    camtoworld(0, 1) = cy_.get_x();
    camtoworld(1, 1) = cy_.get_y();
    camtoworld(2, 1) = cy_.get_z();
    camtoworld(3, 1) = 0;

    camtoworld(0, 2) = cz_.get_x();
    camtoworld(1, 2) = cz_.get_y();
    camtoworld(2, 2) = cz_.get_z();
    camtoworld(3, 2) = 0;

    camtoworld(0, 3) = eye_.get_x();
    camtoworld(1, 3) = eye_.get_y();
    camtoworld(2, 3) = eye_.get_z();
    camtoworld(3, 3) = 1;

    worldtocam = camtoworld.inverse();
}

void Camera::update()
{
    Vector v = Vector(&eye_, &up_);

    cz_ = Vector(&lookat_, &eye_);
    cz_.normalize();
    cx_ = v.cross_product(&cz_);
    cx_.normalize();
    cy_ = cz_.cross_product(&cx_);
    calculate_matrices();
}

Matrix Camera::world_to_camera()
{
    return this->worldtocam;
}

Matrix Camera::camera_to_world()
{
    return this->camtoworld;
}

Vector *Camera::x_axis() { return &cx_; }
Vector *Camera::y_axis() { return &cy_; }
Vector *Camera::z_axis() { return &cz_; }
Point *Camera::get_eye() { return &eye_; }
void Camera::set_eye(double *coords)
{
    eye_.set_coordinates(coords[0], coords[1], coords[2]);
    update();
}
void Camera::set_lookat(double *coords)
{
    lookat_.set_coordinates(coords[0], coords[1], coords[2]);
    update();
}
void Camera::set_viewup(double *coords)
{
    up_.set_coordinates(coords[0], coords[1], coords[2]);
    update();
}
