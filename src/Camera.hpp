#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <vector>
#include "./math/Vector.hpp"
#include "./math/Matrix.hpp"

class Camera
{
private:
    Point eye_;
    Point lookat_;
    Point up_;
    Vector cx_, cy_, cz_;
    Matrix camtoworld;
    Matrix worldtocam;
    void update();
    void calculate_matrices();

public:
    Camera();
    Camera(Point eye, Point lookat, Point up);
    Camera(double *eye, double *lookat, double *up);
    Matrix camera_to_world();
    Matrix world_to_camera();
    Vector *x_axis();
    Vector *y_axis();
    Vector *z_axis();
    Point *get_eye();
    void set_eye(double *coords);
    void set_lookat(double *coords);
    void set_viewup(double *coords);
};

#endif