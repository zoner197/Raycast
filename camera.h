#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

#include "hittable.h"
#include "material.h"

class camera {
    public:
        // Public camera parameters
        double aspect_ratio = 1.0; // Ratio of image width over height
        int image_width = 100; // Rendered image width (in pixels)
        int samples_per_pixel = 10; // Number of random samples for each pixel
        int max_depth = 10; // Maximum ray bounces
        
        double vfov = 90; // Vertical FOV
        point3 lookfrom = point3(0, 0, 0); // Point camera is looking from
        point3 lookat = point3(0, 0, -1); // Point camera is looking at
        vec3 vup = vec3(0, 1, 0); // Camera's relative "up" direction

        double defocus_angle = 0; // Variation angle of the rays through each pixel
        double focus_dist = 10; // Distance from the camera lookfrom point to plane of perfect focus

        void render(const hittable& world) {
            initialize();

            // Setting up file headers
            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int j = 0; j < image_height; j++) {
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                for (int i = 0; i < image_width; i++) {
                    color pixel_color(0, 0, 0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(i, j);
                        pixel_color = ray_color(r, max_depth, world) + pixel_color;
                    }
                    write_color(std::cout, pixel_samples_scale * pixel_color);
                }
            }

            std::clog << "\rDone.                   \n";
        }
    private:
        int image_height; // Rendered image height
        double pixel_samples_scale; // Color scale factor for sum of pixel samples
        point3 center; // Camera center
        point3 pixel100_loc; // Location of pixel 0, 0
        vec3 pixel_delta_u; // Offset to pixel to the right
        vec3 pixel_delta_v; // Offset to the pixel below
        vec3 u, v, w; // Camera frame basis vectors
        vec3 defocus_disk_u; // Defocus disk horizontal radius
        vec3 defocus_disk_v; // Defocus disk vertical radius

        void initialize() {
            // Calculate image height
            image_height = int(image_width / aspect_ratio);
            // Ensure it's atleast 1
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;

            center = lookfrom;

            // Viewport dimensions
            auto theta = degrees_to_radians(vfov);
            auto h = std::tan(theta/2);
            auto viewport_height = 2.0 * h * focus_dist;
            auto viewport_width = viewport_height * (double(image_width)/image_height);

            // Calculate u, v, w unit basis vectors for the camera coordinate frame
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            // Vectors across horizontal and down vertical viewport edges
            auto viewport_u = viewport_width * u; // Vector across viewport horizontal edge
            auto viewport_v = viewport_height * -v; // Vector down viewport vertical edge

            // Calculate horizontal and vertical delta vectors from pixel to pixel
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Location of upper-left pixel | NOTE: Each pixel is inset by half the pixel to pixel distance
            auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
            pixel100_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

            // Calculate defocus disk basis vectors
            auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
            defocus_disk_u = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;
        }

        // Create a camera at the origin, directed at the randomly sampled point around the pixel (i, j)
        ray get_ray(int i, int j) const {
            // Construct camera ray originating from defocus disk, directed at randomly sampled point around pixel (i, j)

            auto offset = sample_square();
            auto pixel_sample = pixel100_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

            auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        // Return a random point in [-.5, -.5] - [-.5, -.5] unit square as vector
        vec3 sample_square() const {
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }

        // Returns a random point in the camera defocus disk
        point3 defocus_disk_sample() const {
            auto p = random_in_unit_disk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }

        color ray_color(const ray& r, int max_bounces, const hittable& world) {
            // If max depth has been exceeded, no more light
            if (max_bounces <= 0) {
                return color(0, 0, 0);
            }

            // If the sphere is hit
            hit_record rec;
            if (world.hit(r, interval(0.001, infinity), rec)) { // 0.001 to ignore hits very close to calculated intersection point to avoid rounding errors (shadow acne patch)
                ray scattered;
                color attenuation;
                if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                    return attenuation * ray_color(scattered, max_bounces-1, world);
                }
                return color(0, 0, 0);
            }

            // Make a blue to white or dark blue to orange gradient based on y
            vec3 unit_direction = unit_vector(r.direction()); // Convert to unit direction for vertical AND horizontal gradient
            auto a = 0.5*(unit_direction.y() + 1.0);

            // Return the lerp (linear blend)
            // return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0); // White to blue
            return (1.0 - a) * color(0.863, 0.529, 0.173) + a * color(0.616, 0.694, 0.808); // Dark blue to orange
        }
};

#endif