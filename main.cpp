#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "cylinder.h"
#include "cone.h"
#include "triangle.h"


#include <iostream>


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

color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1,1,1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;

    // World
    hittable_list world;
    shared_ptr<hittable> sphere1 = make_shared<sphere>(point3(40,50,0), 7);
    shared_ptr<hittable> sphere2 = make_shared<sphere>(point3(40 + 14,50,0), 7);
    shared_ptr<hittable> sphere3 = make_shared<sphere>(point3(40 + 2*14,50,0), 7);
    shared_ptr<hittable> sphere4 = make_shared<sphere>(point3(40 - 7,50 - 7,0), 7);
    shared_ptr<hittable> sphere5 = make_shared<sphere>(point3(40 + 7,50 - 7,0), 7);
    shared_ptr<hittable> sphere6 = make_shared<sphere>(point3(40 + 7 + 14,50 - 7,0), 7);
    shared_ptr<hittable> sphere7 = make_shared<sphere>(point3(40 + 7 + 14 + 14,50 - 7,0), 7);

    shared_ptr<hittable> cone1 = make_shared<cone>(point3(10,0,0), 10, 20);
    shared_ptr<hittable> cylinder1 = make_shared<cylinder>(point3(0,0,0), 10, 20);
    shared_ptr<hittable> triangle1 = make_shared<triangle>(point3(-40,-20,0), point3(0, 20, 0), point3(40, -20, 0));


    world.add(sphere1);
    world.add(sphere2);
    world.add(sphere3);
    world.add(sphere4);
    world.add(sphere5);
    world.add(sphere6);
    world.add(sphere7);


    world.add(triangle1);

    // world.add(cone1);
    // world.add(cylinder1);
    //world.add(triangle1);


    // Camera
    camera cam(point3(0,15,60), point3(0,0,0), vec3(0,1,0), 120, aspect_ratio);


    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
        std::cerr << "\nDone.\n";
}