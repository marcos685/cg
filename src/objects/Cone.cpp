#include "Cone.hpp"
#include <cmath>
#include <vector>

using std::pow, std::sqrt, std::vector;

Cone::Cone() : Shape()
{
    c_ = Point(0, 0, 0);
    vertice_ = Point(0, 1, 0);
    n_ = Vector(0, 1, 0);
    height_ = 1;
    radius_ = 1;
    scale_aux = Point(radius_, height_, radius_);
    n_.normalize();
}

Cone::Cone(Point c, Vector n, double height, double radius, Material *material) : Shape(material)
{
    c_ = c;
    n_ = n;
    height_ = height;
    radius_ = radius;
    n_.normalize();
    double ox, oy, oz, vx, vy, vz;
    c_.get_coordinates(&ox, &oy, &oz);
    n_.get_coordinates(&vx, &vy, &vz);
    ox += vx * height_;
    oy += vy * height_;
    oz += vz * height_;
    vertice_ = Point(ox, oy, oz);
    scale_aux = Point(radius_, height_, radius_);
}

Point *Cone::get_center() { return &c_; }
Point *Cone::get_vertice() { return &vertice_; }
Vector *Cone::get_axis() { return &n_; }
double *Cone::get_height() { return &height_; }
double *Cone::get_radius() { return &radius_; }
void Cone::set_params(Point *c, Point *v, Vector *n, double *height, double *radius)
{
    c_ = *c;
    vertice_ = *v;
    n_ = *n;
    height_ = *height;
    radius_ = *radius;
}

Vector Cone::surface_normal(Point &p_int)
{
    Vector center_p = Vector(&c_, &p_int);
    center_p = n_ * (center_p.dot_product(&n_));
    double px, py, pz;
    c_.get_coordinates(&px, &py, &pz);
    px += center_p.get_x();
    py += center_p.get_y();
    pz += center_p.get_z();

    Point pe = Point(px, py, pz);
    Vector pep_int = Vector(&pe, &p_int);
    Vector p_intv = Vector(&p_int, &vertice_);
    Vector t = p_intv.cross_product(&pep_int);

    Vector normal = t.cross_product(&p_intv);
    normal.normalize();
    return normal;
}

bool Cone::intersects(Ray &ray, double &t_min)
{
    Point p0 = ray.get_p0();
    Vector d = ray.get_d();

    // v = V - P0
    Vector v = Vector(&p0, &vertice_);
    // cos_theta = adj / sqrt(gˆ2 + rˆ2)
    // using cos_theta ^ 2
    double cos_sqrd_theta = pow(height_ / sqrt(pow(height_, 2) + pow(radius_, 2)), 2);
    double a = pow(d.dot_product(&n_), 2) - (d.dot_product(&d) * cos_sqrd_theta);
    double b = (v.dot_product(&d) * cos_sqrd_theta) - (v.dot_product(&n_) * d.dot_product(&n_));
    double c = pow(v.dot_product(&n_), 2) - (v.dot_product(&v) * cos_sqrd_theta);
    double delta = pow(b, 2) - 4 * a * c;
    if (delta < 0)
        return false;
    double t_int0 = (-b + sqrt(delta)) / 2 * a;
    double t_int1 = (-b - sqrt(delta)) / 2 * a;
    Point p1 = ray.calc_point(t_int0);
    Point p2 = ray.calc_point(t_int1);
    double p1_dotproduct = Vector(&p1, &vertice_).dot_product(&n_);
    double p2_dotproduct = Vector(&p2, &vertice_).dot_product(&n_);

    vector<double> intersections;

    if (t_int0 >= 0 && (0 <= p1_dotproduct && p1_dotproduct <= height_))
        intersections.push_back(t_int0);
    if (t_int1 >= 0 && (0 <= p2_dotproduct && p2_dotproduct <= height_))
        intersections.push_back(t_int1);

    // one intersection with cone surface
    // the other might happen with the base
    if (intersections.size() == 1)
    {
        Plane base_plane = Plane(c_, n_);
        double t_base;
        bool base_intersection = base_plane.intersects(ray, t_base);
        if (base_intersection)
        {
            Point p_base = ray.calc_point(t_base);
            Vector cbase = Vector(&c_, &p_base);
            if (t_base >= 0 && cbase.norm() < radius_)
                intersections.push_back(t_base);
        }
    }
    int int_candidates = intersections.size();

    t_min = int_candidates != 0 ? intersections[0] : t_min;
    for (int i = 1; i < int_candidates; i++)
        if (intersections[i] < t_min)
            t_min = intersections[i];

    return int_candidates >= 1;
}

void Cone::translate(Matrix t_matrix)
{
    c_ = t_matrix * c_;
    vertice_ = t_matrix * vertice_;
}

void Cone::rotate(Matrix t_matrix)
{
    n_ = t_matrix * n_;
}

void Cone::scale(Matrix t_matrix)
{
    scale_aux = t_matrix * scale_aux;
}
