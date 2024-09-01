#ifndef VEC3_H
#define VEC3_H

class vec3 {
    public:
        double e[3];

        // Function 'initializer lists'
        vec3() : e{0, 0, 0} {}
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

        // Functions to return x, y, z to protect data integrity
        double x() const {return e[0];}
        double y() const {return e[1];}
        double z() const {return e[2];}

        // Return a negative version of the class when the negative operator is applied
        vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]);}

        // Returns the object at index specified in the subscript operator in read-only
        double operator[](int i) const {return e[i];}
        // Returns the object at the index as read-write
        double& operator[](int i) {return e[i];}

        // *= operator overloading (vec3 *= constant)
        vec3& operator*=(double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        // /= operator overloading (vec3 /= constant)
        vec3& operator/=(double t) {
            return *this *= 1/t; // Using the *= overload from before to multiply this object's values with a constant
        }

        // Function to calculate and return length of object
        double length() const {
            return std::sqrt(length_squared());
        }

        double length_squared() const {
            return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
        }

        // Check if vector is near zero
        bool near_zero() const {
            auto s = 1e-8; // 1 * 10^-8 scientific notation
            return (std::fabs(e[0]) < s && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s));
        }

        static vec3 random() {
            return vec3(random_double(), random_double(), random_double());
        }

        static vec3 random(double min, double max) {
            return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
        }
};

// Alias for vec3; useful for geometric clarity in code
using point3 = vec3;

// Vector Utility Functions

/*
    NOTE:
    Inline functions are the equivalent of copy & pasting the code in the function into the spot that the function is called.
*/

// Over loading the << operator
inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// Overloading the + operator
inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// Overloading the - operator
inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// Overloading the * operator
inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return v * t;
}

// Overloading the / operator
inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}

// Dot product
inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2];
}

// Cross product
inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// Convert vector to unit vector
inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

inline vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() < 1) {
            return p;
        }
    }
}

inline vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1,1);
        if (p.length_squared() < 1)
            return p;
    }
}

inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    // Check if the point is on the same hemisphere as the normal
    if (dot(on_unit_sphere, normal) > 0.0) {
        // The point is on the same hemisphere as the normal
        return on_unit_sphere;
    } else {
        // The point isn't on the the same hemisphere as the normal
        // Invert the point so that it is
        return -on_unit_sphere;
    }
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

#endif