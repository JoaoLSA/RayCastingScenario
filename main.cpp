#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "cylinder.h"
#include "cone.h"
#include "triangle.h"
#include "material.h"
#include "aarect.h"


#include <iostream>


hittable_list simple_light() {
    hittable_list objects;

    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    auto yellow = make_shared<solid_color>(color(1, 1, 0));
    auto blue = make_shared<solid_color>(color(0, 0, 0.5));

    auto green = make_shared<solid_color>(color(0, 0.7, 0));

    //objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(checker)));

    shared_ptr<hittable> sphere1 = make_shared<sphere>(point3(40,50,0), 7, make_shared<lambertian>(blue));
    shared_ptr<hittable> sphere2 = make_shared<sphere>(point3(40 + 14,50,0), 7, make_shared<lambertian>(blue));
    shared_ptr<hittable> sphere3 = make_shared<sphere>(point3(40 + 2*14,50,0), 7, make_shared<lambertian>(blue));
    shared_ptr<hittable> sphere4 = make_shared<sphere>(point3(40 - 7,50 - 7,0), 7, make_shared<lambertian>(blue));
    shared_ptr<hittable> sphere5 = make_shared<sphere>(point3(40 + 7,50 - 7,0), 7, make_shared<lambertian>(blue));
    shared_ptr<hittable> sphere6 = make_shared<sphere>(point3(40 + 7 + 14,50 - 7,0), 7, make_shared<lambertian>(blue));
    shared_ptr<hittable> sphere7 = make_shared<sphere>(point3(65,50 - 7,0), 7, make_shared<lambertian>(blue));

    shared_ptr<hittable> triangle1 = make_shared<triangle>(point3(-40,-20,0), point3(0, 20, 0), point3(40, -20, 0), make_shared<lambertian>(yellow));
    shared_ptr<hittable> triangle2 = make_shared<triangle>(point3(-30,-20 - 40,0), point3(-30, -20, 0), point3(30, -20 - 40, 0), make_shared<lambertian>(yellow));
    shared_ptr<hittable> triangle3 = make_shared<triangle>(point3(-30,-20,0), point3(30, -20, 0), point3(30, -20 - 40, 0), make_shared<lambertian>(yellow));

    shared_ptr<hittable> cylinder1 = make_shared<cylinder>(point3(0,0,0), 14, 30, make_shared<lambertian>(yellow));
    shared_ptr<hittable> cylinder2 = make_shared<cylinder>(point3(0,0,0), 14, 30, make_shared<lambertian>(yellow));

    shared_ptr<hittable> car1 = make_shared<triangle>(point3(-150,-125, 30), point3(-10, -125, 30), point3(-10, -100, 30), make_shared<lambertian>(yellow));
    shared_ptr<hittable> car2 = make_shared<triangle>(point3(-150,-125, 30), point3(-150, -100, 30), point3(-10, -100, 30), make_shared<lambertian>(yellow));

    shared_ptr<hittable> car3 = make_shared<triangle>(point3(-120,-100, 30), point3(-30, -100, 30), point3(-30, -75, 30), make_shared<lambertian>(yellow));
    shared_ptr<hittable> car4 = make_shared<triangle>(point3(-120,-100, 30), point3(-120, -75, 30), point3(-30, -75, 30), make_shared<lambertian>(yellow));

    shared_ptr<hittable> carRear1 = make_shared<triangle>(point3(-120,-100, 30), point3(-30, -100, 30), point3(-30, -75, 30), make_shared<lambertian>(yellow));
    shared_ptr<hittable> carRear2 = make_shared<triangle>(point3(-120,-100, 30), point3(-120, -75, 30), point3(-30, -75, 30), make_shared<lambertian>(yellow));

    shared_ptr<hittable> carWheelF = make_shared<cylinder>(point3(0,0, 0), 15, 25, make_shared<lambertian>(yellow));
    carWheelF = make_shared<translate>(carWheelF, point3(-120,-115, 12.7));
 
    shared_ptr<hittable> carWheelB = make_shared<cylinder>(point3(-30,-115, 12.7), 15, 25, make_shared<lambertian>(yellow));

    auto tree1_base = make_shared<cylinder>(point3(400,-115,30), 20, 60, make_shared<lambertian>(green));
    objects.add(make_shared<rotate_x>(tree1_base, 90));

    auto tree1_top = make_shared<cone>(point3(400,-115 + 30, 30), 60, 40, make_shared<lambertian>(green));
    objects.add(make_shared<rotate_x>(tree1_top, 90));



    // objects.add(sphere1);
    // objects.add(sphere2);
    // objects.add(sphere3);
    // objects.add(sphere4);
    // objects.add(sphere5);
    // objects.add(sphere6);
    // objects.add(sphere7);


    objects.add(triangle1);
    objects.add(triangle2);
    objects.add(triangle3);


    objects.add(car1);
    objects.add(car2);

    objects.add(car3);
    objects.add(car4);
    objects.add(carWheelF);
    objects.add(carWheelB);


    // Esfera que emite luz
    auto difflight = make_shared<diffuse_light>(color(10,10,10));
    auto sun = make_shared<sphere>(point3(0,0,0), 10, difflight);
    objects.add(make_shared<translate>(sun, point3(0, 60, 50)));


    return objects;
}

double hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - sqrt(discriminant) ) / (2.0*a);
    }
}


// Simple color:
color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1,1,1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}


// Ray casting color:
color ray_color(const ray& r, const color& background, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    // If the ray hits nothing, return the background color.
    //Calculando origens de raios refletidos com tolerância
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth-1);
}

int main() {

    // Image
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50; //limitar o número de raios filhos
    point3 lookfrom;
    point3 lookat;

    color background(0,0,0);

    background = color(0,0,0);

    // World
    hittable_list world;    
    world = simple_light();
    lookfrom = point3(0,0,250);
    lookat = point3(0,2,0);
    // Camera
    camera cam(lookfrom, lookat, vec3(0,1,0), 90, aspect_ratio);


    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    /*adicionar a cor total a cada iteração e, em seguida, realizar uma única 
    divisão no final (pelo número de amostras) ao escrever a cor.*/
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                //pixel_color += ray_color(r, background, world, max_depth);
                pixel_color += ray_color(r, world);

            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
        std::cerr << "\nDone.\n";
}