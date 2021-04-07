#ifndef POINT_HPP
#define POINT_HPP

class Point
{
private:
    double x_, y_, z_;

public:
    Point();
    Point(double x, double y, double z);
    Point(double *coords);
    double get_x();
    double get_y();
    double get_z();
    void get_coordinates(double *x, double *y, double *z);
    void set_coordinates(double x, double y, double z);
    void to_array(double *arr);
    double distance_from(Point *p);
    Point operator+(Point &p);
    Point operator-(Point &p);
    Point operator*(double value);
};

#endif
