#include "./objects/Cylinder.hpp"
#include "./objects/Shape.hpp"
#include "./objects/Object.hpp"
#include "./objects/Bounding_Box.hpp"
#include "./objects/Triangle.hpp"
#include "./objects/Cone.hpp"
#include "./objects/Sphere.hpp"
#include "Intersection.hpp"

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
double *unormalized_buffer = new double[resolution * resolution * 3];
GLubyte *PixelBuffer = new GLubyte[resolution * resolution * 3];

static double observer[3] = {70.0f, 25.0f, 80.0f};
static double lookat[3] = {40.0f, 5.0f, 5.0f};
static double viewup[3] = {70.0f, 26.0f, 80.0f};

double width = 6.0;
double dist = 6.0;

Camera *camera = new Camera(observer, lookat, viewup);

Color bg_color = Color(0.02, 0, 0.08);
//Ambient light
Light *ambient_light = new Light(Color(0.02, 0.0, 0.08), Vector(), AMBIENT);

vector<Light *> lights = {ambient_light};

vector<Object *> objects;

//Materials
Material *ground_material = new Material(
    Color(17.0 / 255, 124.0 / 255, 19.0 / 255),
    Color(19.0 / 255, 133.0 / 255, 16.0 / 255),
    Color(65.0 / 255, 152.0 / 255, 10.0 / 255),
    4);

Material *sidewalk_material = new Material(
    Color(9.0 / 255, 97.0 / 255, 101.0 / 255),
    Color(114.0 / 255, 112.0 / 255, 116.0 / 255),
    Color(150.0 / 255, 152.0 / 255, 154.0 / 255),
    1);

Material *road_material = new Material(
    Color(30.0 / 255, 30.0 / 255, 30.0 / 255),
    Color(40.0 / 255, 40.0 / 255, 40.0 / 255),
    Color(80.0 / 255, 80.0 / 255, 80.0 / 255),
    1);

Material *base_material = new Material(
    Color(215.0 / 255, 215.0 / 255, 180.0 / 255),
    Color(239.0 / 255, 239.0 / 255, 200.0 / 255),
    Color(253.0 / 255, 253.0 / 255, 249.0 / 255),
    4);

Material *roof_material = new Material(
    Color(231.0 / 255, 70.0 / 255, 0.0 / 255),
    Color(255.0 / 255, 78.0 / 255, 1.0 / 255),
    Color(255.0 / 255, 114.0 / 255, 52.0 / 255),
    16);

Material *darkwood_material = new Material(
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
            t_min = t_int;
            shape_hit = shape;
            object_index = i;
        }
    }

    if (object_index != -1)
    {
        double episilon = 0.01;
        Color color = Color();
        Point p_int = ray.calc_point(t_int);
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
        return color;
    }
    return bg_color;
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
                        int y1 = resolution - 1 - y;
                        int position = (x + y1 * resolution) * 3;
                        unormalized_buffer[position] = color.r;
                        unormalized_buffer[position + 1] = color.g;
                        unormalized_buffer[position + 2] = color.b;
                    }
                }
            },
            t * resolution / nthreads, (t + 1) == nthreads ? resolution : (t + 1) * resolution / nthreads, t));
    }

    for (size_t i = 0; i < nthreads; i++)
        threads[i].join();

    double max_intensity = 0;
    for (int i = 0; i < resolution * resolution * 3; i++)
    {
        if (unormalized_buffer[i] > 1 && unormalized_buffer[i] > max_intensity)
            max_intensity = unormalized_buffer[i];
    }
    if (max_intensity > 1)
    {
        for (int i = 0; i < resolution * resolution * 3; i++)
        {
            PixelBuffer[i] = floor((unormalized_buffer[i] / max_intensity) * 255);
        }
    }
    else
    {
        for (int i = 0; i < resolution * resolution * 3; i++)
        {
            PixelBuffer[i] = floor(unormalized_buffer[i] * 255);
        }
    }

    render_end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = render_end - render_start;

    cout << "[SCENE] Rendered in " << setprecision(2);
    cout << elapsed_seconds.count() << "s\n";

    glDrawPixels(resolution, resolution, GL_RGB, GL_UNSIGNED_BYTE, PixelBuffer);
    glFlush();
}

int main(int argc, char **argv)
{
    //=========Objects========>
    //ground
    Point ground1 = Point();
    Point ground2 = Point(150, 0, 0);
    Point ground3 = Point(0, 0, 25);
    Point ground4 = Point(150, 0, 25);
    Object *ground = new Object(
        "ground",
        Bounding_Box(Point(), Point(150, 0, 25)),
        vector<Shape *>{
            new Triangle(ground3, ground2, ground1, ground_material),
            new Triangle(ground4, ground2, ground3, ground_material)});

    ground->translate(Vector(-20, 0, 0));

    //sidewalk
    Point sidewalk1 = Point(0, 0, 30);
    Point sidewalk2 = Point(150, 0, 30);
    Object *sidewalk = new Object(
        "ground",
        Bounding_Box(ground3, sidewalk2),
        vector<Shape *>{
            new Triangle(ground3, ground4, sidewalk1, sidewalk_material),
            new Triangle(ground4, sidewalk2, sidewalk1, sidewalk_material)});

    //road
    Point road1 = Point(0, 0, 100);
    Point road2 = Point(150, 0, 100);
    Object *road = new Object(
        "ground",
        Bounding_Box(sidewalk1, road2),
        vector<Shape *>{
            new Triangle(road1, sidewalk2, sidewalk1, road_material),
            new Triangle(road2, road1, sidewalk2, road_material)});

    // //house
    Point base1 = Point(0, 0, 0);
    Point base2 = Point(10, 0, 0);
    Point base3 = Point(0, 0, 10);
    Point base4 = Point(10, 0, 10);
    Point roof1 = Point(0, 10, 0);
    Point roof2 = Point(10, 10, 0);
    Point roof3 = Point(0, 10, 10);
    Point roof4 = Point(10, 10, 10);
    Point roof5 = Point(5, 15, 5);

    Object *house = new Object(
        "house",
        Bounding_Box(Point(), Point(10, 15, 10)),
        vector<Shape *>{
            new Triangle(base1, base2, base3, base_material),
            new Triangle(base4, base3, base2, base_material),
            new Triangle(base1, roof1, base2, base_material),
            new Triangle(base3, roof1, base1, base_material),
            new Triangle(roof1, roof2, base2, base_material),
            new Triangle(base3, roof3, roof1, base_material),
            new Triangle(roof4, roof3, base3, base_material),
            new Triangle(base4, roof4, base3, base_material),
            new Triangle(base4, base2, roof4, base_material),
            new Triangle(base2, roof2, roof4, base_material),
            new Triangle(roof2, roof1, roof5, roof_material),
            new Triangle(roof1, roof3, roof5, roof_material),
            new Triangle(roof4, roof2, roof5, roof_material),
            new Triangle(roof3, roof4, roof5, roof_material)});

    house->translate(Vector(30, 0, 5));

    //lamppost
    Object *lamppost = new Object(
        "lamppost",
        Bounding_Box(Point(-0.5, 0, -0.5), Point(0.5, 12, 5.5)),
        vector<Shape *>{
            new Cylinder(Point(), Vector(0, 1, 0), 10, 0.5, darkwood_material),
            new Cylinder(Point(0, 9.5, 0.5), Vector(0, 0, 1), 4, 0.25, sidewalk_material),
            new Sphere(Point(0, 9.5, 5), 0.5, sidewalk_material)});

    lamppost->translate(Vector(20, 0, 25));

    //tree
    Object *tree = new Object(
        "tree",
        Bounding_Box(Point(-3, 0, -3), Point(3, 20, 3)),
        vector<Shape *>{
            new Cylinder(Point(), Vector(0, 1, 0), 7.5, 1, darkwood_material),
            new Cone(Point(0, 7.5, 0), Vector(0, 1, 0), 12.5, 3, ground_material)});

    tree->translate(Vector(50, 0, 10));

    Object *trashcan = new Object(
        "trashcan",
        Bounding_Box(Point(-0.75, 0, -0.75), Point(0.75, 2, 0.75)),
        vector<Shape *>{
            new Cylinder(Point(), Vector(0, 1, 0), 2, 0.75, road_material),
        });

    trashcan->translate(Vector(24, 0, 28));

    objects.push_back(ground);
    objects.push_back(sidewalk);
    objects.push_back(road);
    objects.push_back(house);
    objects.push_back(lamppost);
    objects.push_back(tree);
    objects.push_back(trashcan);

    //==========LIGHTS==========>
    //light on the side of the house
    Light *p_light = new Light(Color(0.3, 0.3, .3), Vector(45.1, 5.0, 7.5));
    Light *r_light = new Light(Color(0.15, 0.15, 0.15), Vector(1, -1, 1), REMOTE);
    Light *s_light = new Light(Color(0.4, 0.4, 0.4), Point(20, 9, 30), Vector(0, -1, 0), 0.0, M_PI / 4, 1.0);

    lights.push_back(p_light);
    lights.push_back(r_light);
    lights.push_back(s_light);

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