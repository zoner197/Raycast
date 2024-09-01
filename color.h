#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"

// Alias vec3 as color, since the same class is being used
using color = vec3;

// Transforms the linear space r, g or b components to gamma space components
inline double linear_to_gamma(double linear_value) {
    if (linear_value > 0) {
        return std::sqrt(linear_value);
    }

    return 0;
}

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Gamma correct the values
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Translate [0, 1] components to byte range [0, 255]
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    // Write the pixel components
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif