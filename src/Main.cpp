#include "./objects/Cylinder.hpp"
#include "./objects/Shape.hpp"
#include "./objects/Object.hpp"
#include "./objects/Bounding_Box.hpp"

#include <limits>
#include <GL/freeglut.h>

#include "Color.hpp"
#include "./objects/Object.hpp"
#include "Camera.hpp"
#include "Light.hpp"

#include <thread>
#include <functional>
#include <chrono>
#include <vector>

#include <cmath>
#include <ctime>
#include <iostream>
#include <iomanip>

using namespace std;

int resolution = 500;
double upscaling = 1.0f;
GLubyte *PixelBuffer = new GLubyte[resolution * resolution * 3];

static double observer[3] = {0.0f, 180.0f, 355.0f};
static double lookat[3] = {300.0f, 160.0f, 0.0f};
static double viewup[3] = {0.0f, 181.0f, 355.0f};

double width = 6.0;
double dist = 3.0;

Camera *camera = new Camera(observer, lookat, viewup);

//Ambient light
Light *ambient_light = new Light(Color(0.2, 0.8, 0.2), Vector(), AMBIENT);

vector<Light *> lights = {ambient_light};

vector<Object *> objects;

Material *mat_darkwood = new Material(
    Color(0.149019, 0.090196, 0.062745),
    Color(0.01, 0.01, 0.01),
    Color(0.1, 0.1, 0.1),
    38.0);

Color trace_ray(int x, int y)
{
    double pixel_width = width / resolution;
    Point observer = *(camera->get_eye());

    double x_pos = -width / 2 + pixel_width / 2 + x * pixel_width;
    double y_pos = width / 2 - pixel_width / 2 - y * pixel_width;

    Point hole_point = Point(x_pos, y_pos, -dist);
    hole_point = camera->camera_to_world() * hole_point;
    Vector ray_direction = Vector(&observer, &hole_point);
    Ray ray = Ray(observer, ray_direction);

    double t_min = numeric_limits<double>::infinity();

    Shape *shape, *shape_hit;
    double t_int;
    int object_index = -1;

    for (unsigned i = 0; i < objects.size(); i++)
    {

        if (objects[i]->trace(ray, &shape, t_int) && t_int < t_min)
        {
            std::cout << "here" << '\n';
            std::cout << shape << '\n';
            std::cout << "here" << '\n';
            t_min = t_int;
            shape_hit = shape;
            object_index = i;
        }
    }

    Color color = Color();
    if (object_index != -1)
    {
        double episilon = 0.01;

        Point p_int = ray.calc_point(t_int);
        std::cout << shape_hit->name << '\n';
        Vector surfc_normal = shape_hit->surface_normal(p_int);

        surfc_normal = surfc_normal * episilon;

        // shifts intersection so there is no shadow acne
        p_int.set_coordinates(
            p_int.get_x() + surfc_normal.get_x(),
            p_int.get_y() + surfc_normal.get_y(),
            p_int.get_z() + surfc_normal.get_z());

        // cast shadow rays for each light
        for (unsigned i = 0; i < lights.size(); i++)
        {
            if (!*(lights[i]->active()))
                continue;
            if (lights[i]->type() == AMBIENT)
            {
                color += shape_hit->calculate_color(lights[i], observer, p_int);
                continue;
            }

            Vector light_dir = shape_hit->light_direction(lights[i], p_int);
            Ray shadowray = Ray(p_int, light_dir);

            Vector lvp = lights[i]->get_position();
            Point light_point = lights[i]->type() == SPOT ? lights[i]->get_spotpos() : Point(lvp.get_x(), lvp.get_y(), lvp.get_z());

            int visible = 1;
            unsigned k = 0;
            Shape *shadow_shape_hit;
            double shadow_t_int;
            while (visible && k < objects.size())
            {
                if (objects[k]->trace(shadowray, &shadow_shape_hit, shadow_t_int))
                {
                    visible = 0;
                    if (lights[i]->type() == POINT || lights[i]->type() == SPOT)
                    {
                        Point shadow_point = shadowray.calc_point(t_int);
                        if (p_int.distance_from(&shadow_point) > p_int.distance_from(&light_point))
                            visible = 1; // light is closer to the object
                    }
                }
                k++;
            }
            color += shape_hit->calculate_color(lights[i], observer, p_int) * visible;
        }
    }
    return color;
}

void set_pixel(int x, int y, Color color)
{
    y = resolution - 1 - y;
    int position = (x + y * resolution) * 3;
    PixelBuffer[position] = floor(color.r * 255);
    PixelBuffer[position + 1] = floor(color.g * 255);
    PixelBuffer[position + 2] = floor(color.b * 255);
}

void keyboard(unsigned char key, int mousex, int mousey)
{
    if (key == 'q' || key == 'Q')
        exit(0);
}

void raycasting()
{
    cout << "[SCENE] Started rendering" << '\n';
    chrono::time_point<chrono::system_clock> render_start, render_end;

    glClearColor(0.0, 0.8, 0.8, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    const size_t nthreads = thread::hardware_concurrency();
    vector<thread> threads(nthreads);

    render_start = chrono::system_clock::now();

    for (size_t t = 0; t < nthreads; t++)
    {
        threads[t] = thread(bind(
            [&](const int start_i, const int end_i, const int t) {
                for (int y = start_i; y < end_i; y++)
                {
                    for (int x = 0; x < resolution; x++)
                    {
                        Color color = trace_ray(x, y);
                        set_pixel(x, y, color);
                    }
                }
            },
            t * resolution / nthreads, (t + 1) == nthreads ? resolution : (t + 1) * resolution / nthreads, t));
    }

    for (size_t i = 0; i < nthreads; i++)
        threads[i].join();

    render_end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = render_end - render_start;

    cout << "[SCENE] Rendered in " << setprecision(2);
    cout << elapsed_seconds.count() << "s\n";

    glDrawPixels(resolution, resolution, GL_RGB, GL_UNSIGNED_BYTE, PixelBuffer);
    glFlush();
}

int main(int argc, char **argv)
{
    Object *red_cup = new Object(
        "Red cup",
        Bounding_Box(Point(-30, 0, -30), Point(30, 150, 30)),
        vector<Shape *>{
            new Cylinder(Point(), Vector(), 15, 3, new Material(Color(0.4588, 0.1803, 0.1960), Color(0.6117, 0.1686, 0.2470), Color(0.5, 0.5, 0.5), 32))});

    red_cup->translate(Vector());

    objects.push_back(red_cup);
    // Negotiating window section
    glutInit(&argc, argv);

    // Seting the window properties
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(resolution, resolution);
    glutInitWindowPosition(0, 0);

    // Creating the OpenGL window
    glutCreateWindow("Test");

    // Defining the callback functions
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(raycasting);

    // Entering the main event loop
    glutMainLoop();
}