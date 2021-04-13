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
    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(checker)));

    //objects.add(make_shared<sphere>(point3(0,10,0), 10, make_shared<lambertian>(checker)));
    objects.add(make_shared<cone>(point3(10,10,0), 10, 10, make_shared<lambertian>(checker)));



    auto difflight = make_shared<diffuse_light>(color(4,4,4));
    objects.add(make_shared<sphere>(point3(0,30,0), 10, difflight));

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
    lookfrom = point3(26,3,6);
    lookat = point3(0,2,0);
    // Camera
    camera cam(lookfrom, lookat, vec3(0,1,0), 120, aspect_ratio);


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
                pixel_color += ray_color(r, background, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
        std::cerr << "\nDone.\n";
}