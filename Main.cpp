#include "common.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

int main() {
    hittable_list world;

    auto ground = make_shared<lambertian>(color(0.839, 0.839, 0.89));
    auto sphere_1 = make_shared<lambertian>(color(0.89, 0.373, 0.373));
    auto sphere_2 = make_shared<metal>(color(1, 1, 1), 0.2);
    auto sphere_3 = make_shared<dielectric>(1.5);

    world.add(make_shared<sphere>(point3(0, -500, 0), 500, ground));
    world.add(make_shared<sphere>(point3(0, 0.5, 0), 0.5, sphere_1));
    world.add(make_shared<sphere>(point3(0.51, 0.3, 0.5), 0.3, sphere_2));
    world.add(make_shared<sphere>(point3(-1, 0.6, 0), 0.6, sphere_3));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1200;
    cam.samples_per_pixel = 100;
    cam.max_depth = 60;

    cam.vfov = 20;
    cam.lookfrom = point3(3.2, 0.4, 2);
    cam.lookat = point3(-0.25, 0.6, 0);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 1.2;
    cam.focus_dist = 3.775;

    cam.render(world);
}