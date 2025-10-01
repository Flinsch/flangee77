#ifndef ML7_VECTOR3_H
#define ML7_VECTOR3_H

#include <MathLabs/math.h>



namespace ml7 {



template <std::floating_point T>
struct Vector3
{

    using scalar_type = T;

    static const Vector3 ZERO;
    static const Vector3 X;
    static const Vector3 Y;
    static const Vector3 Z;



    // #############################################################################
    // Attributes
    // #############################################################################

    union
    {
        struct
        {
            // NOLINTBEGIN(*-use-default-member-init)
            /** The vector's x value. */
            T x;
            /** The vector's y value. */
            T y;
            /** The vector's z value. */
            T z;
            // NOLINTEND(*-use-default-member-init)
        }; // struct

        /** Array of all three components. */
        T data[3];
    }; // union



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes the vector with x = y = z = 0.
     */
    constexpr Vector3() noexcept
        : x(T{0})
        , y(T{0})
        , z(T{0})
    {
    }

    /**
     * Explicit constructor with parameters for x, y, and z.
     */
    constexpr Vector3(T x, T y, T z) noexcept
        : x(x)
        , y(y)
        , z(z)
    {
    }

    /**
     * Explicit constructor with one parameter for x, y, and z.
     */
    constexpr explicit Vector3(T c) noexcept
        : x(c)
        , y(c)
        , z(c)
    {
    }

    /**
     * Swap operation.
     */
    void swap(Vector3& other) noexcept
    {
        std::swap(x, other.x);
        std::swap(y, other.y);
        std::swap(z, other.z);
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /** Returns the magnitude of this vector. */
    T length() const { return std::sqrt(x*x + y*y + z*z); }

    /** Returns the squared magnitude of this vector. */
    T length_squared() const { return x*x + y*y + z*z; }

    /**
     * Returns the azimuth angle of this vector in the signed range [-pi;+pi], with
     * the positive x-axis pointing east and the positive z-axis pointing north.
     */
    T azimuth_symmetric() const
    {
        return std::atan2(x, z);
    }

    /**
     * Returns the azimuth angle of this vector in the positive range [0;2pi], with
     * the positive x-axis pointing east and the positive z-axis pointing north.
     */
    T azimuth_asymmetric() const
    {
        T a = std::atan2(x, z);
        if (x < T{0})
            a += constants<T>::pi2;
        return a;
    }

    /**
     * Returns the elevation angle of this vector in the range [-pi/2;+pi/2], with
     * the positive y-axis pointing up.
     */
    T elevation() const
    {
        return std::atan2(y, std::sqrt(x*x + z*z));
    }

    /** Returns a new vector with the x component of this (y = z = 0). */
    Vector3 x__() const { return {x, T{0}, T{0}}; } // NOLINT
    /** Returns a new vector with the y component of this (x = z = 0). */
    Vector3 _y_() const { return {T{0}, y, T{0}}; }
    /** Returns a new vector with the y component of this (x = y = 0). */
    Vector3 __z() const { return {T{0}, T{0}, z}; } // NOLINT

    /** Returns a new vector with the x and y components of this (z = 0). */
    Vector3 xy_() const { return {x, y, T{0}}; }
    /** Returns a new vector with the x and z components of this (y = 0). */
    Vector3 x_z() const { return {x, T{0}, z}; }
    /** Returns a new vector with the y and z components of this (x = 0). */
    Vector3 _yz() const { return {T{0}, y, z}; }

    /** Returns a new vector with all of its components set to the x value of this. */
    Vector3 xxx() const { return {x, x, x}; }
    /** Returns a new vector with all of its components set to the y value of this. */
    Vector3 yyy() const { return {y, y, y}; }
    /** Returns a new vector with all of its components set to the y value of this. */
    Vector3 zzz() const { return {z, z, z}; }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this vector normalized such that it has a magnitude of 1.
     */
    Vector3 normalized() const
    {
        T d = length_squared();
        if (d == T{0})
            return ZERO; // x = y = z = 0
        d = T{1} / std::sqrt(d);
        return {x * d, y * d, z * d};
    }

    /**
     * Returns this vector having positive components.
     */
    Vector3 abs() const
    {
        return {std::abs(x), std::abs(y), std::abs(z)};
    }

    /**
     * Returns the dot/scalar product of this and a given vector v.
     */
    T dot(const Vector3& v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }

    /**
     * Returns the squared dot/scalar product of this and a given vector v.
     */
    T dot_squared(const Vector3& v) const
    {
        const T tmp = dot(v);
        return tmp*tmp;
    }

    /**
     * Returns the cross product of this and a given vector v.
     */
    Vector3 cross(const Vector3& v) const
    {
        return {
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x,
        };
    }

    /**
     * Returns this vector reflected across a plane defined by a given normal
     * vector n.
     */
    Vector3 reflected(const Vector3& n) const
    {
        const Vector3 n2dot = n * T{2} * dot(n);
        return *this - n2dot;
    }

    /**
     * Returns this vector refracted across a plane defined by a given normal
     * vector n with respect to the specified refractive index.
     */
    Vector3 refracted(const Vector3& n, T index) const
    {
        const T t = dot(n);
        const T r = 1 - index*index * (1 - t*t);

        if (r < T{0})
            return {};

        const T s = index * t + std::sqrt(r);
        return index * *this - s * n;
    }

    /**
     * Returns the scalar projection of a given vector v onto this one.
     */
    T comp(const Vector3& v) const
    {
        return dot(v) / length();
    }

    /**
     * Returns the vector projection of a given vector v onto this one.
     */
    Vector3 proj(const Vector3& v) const
    {
        return *this * (dot(v) / length_squared());
    }



    // #############################################################################
    // Manipulations
    // #############################################################################

    /**
     * Sets x = y = z = 0.
     */
    Vector3& clear()
    {
        x = y = z = T{0};
        return *this;
    }

    /**
     * Inverts the sign of the components of this vector.
     */
    Vector3& invert()
    {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    /**
     * Normalizes this vector that it has a magnitude of 1.
     */
    Vector3& normalize()
    {
        *this = normalized();
        return *this;
    }

    /**
     * Lets this vector have a magnitude of the given length.
     */
    Vector3& length(T length)
    {
        normalize();
        x *= length;
        y *= length;
        z *= length;
        return *this;
    }

    /**
     * Reflects this vector across a plane defined by a given normal vector n.
     */
    Vector3& reflect(const Vector3& n)
    {
        *this = reflected(n);
        return *this;
    }

    /**
     * Refracts this vector across a plane defined by a given normal vector n with
     * respect to the specified refractive index.
     */
    Vector3& refract(const Vector3& n, T index)
    {
        *this = refracted(n, index);
        return *this;
    }



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const Vector3& v) const noexcept { return (x == v.x) && (y == v.y) && (z == v.z); }
    bool operator!=(const Vector3& v) const noexcept { return (x != v.x) || (y != v.y) || (z != v.z); }

    /**
     * Checks if this vector is component-by-component less than a given vector v.
     */
    bool operator<(const Vector3& v) const noexcept
    {
        return (x < v.x) && (y < v.y) && (z < v.z);
    }

    /**
     * Checks if this vector is component-by-component greater than a given vector v.
     */
    bool operator>(const Vector3& v) const noexcept
    {
        return (x > v.x) && (y > v.y) && (z > v.z);
    }

    /**
     * Checks if this vector is component-by-component less than or equal to a given
     * vector v.
     */
    bool operator<=(const Vector3& v) const noexcept
    {
        return (x <= v.x) && (y <= v.y) && (z <= v.z);
    }

    /**
     * Checks if this vector is component-by-component greater than or equal to a
     * given vector v.
     */
    bool operator>=(const Vector3& v) const noexcept
    {
        return (x >= v.x) && (y >= v.y) && (z >= v.z);
    }



    // #############################################################################
    // Arithmetic Operators
    // #############################################################################

    /** Returns a copy of this vector unmodified. */
    constexpr Vector3 operator+() const { return *this; }
    /** Returns a copy of this vector with the signs of the elements flipped. */
    constexpr Vector3 operator-() const { return {-x, -y, -z}; }

    /** Returns the (component-wise) vector sum of two vectors. */
    constexpr Vector3 operator+(const Vector3& v) const { return {x + v.x, y + v.y, z + v.z}; }
    /** Returns the (component-wise) vector difference of two vectors. */
    constexpr Vector3 operator-(const Vector3& v) const { return {x - v.x, y - v.y, z - v.z}; }

    /** Returns the (component-wise) Hadamard product of two vectors. */
    constexpr Vector3 operator*(const Vector3& v) const { return {x * v.x, y * v.y, z * v.z}; }
    /** Returns the (component-wise) Hadamard quotient of two vectors. */
    constexpr Vector3 operator/(const Vector3& v) const { return {x / v.x, y / v.y, z / v.z}; }

    /** Returns a copy of this vector scaled by the specified factor (scalar multiplication). */
    constexpr Vector3 operator*(T s) const { return {x * s, y * s, z * s}; }
    /** Returns a copy of this vector inversely scaled by the specified factor (scalar division). */
    constexpr Vector3 operator/(T s) const { return {x / s, y / s, z / s}; }

    /** Scales a vector by the specified factor (scalar multiplication). */
    friend constexpr Vector3 operator*(T s, const Vector3& v) { return v * s; }



    // #############################################################################
    // Arithmetic Assignment Operators
    // #############################################################################

    /** Adds the given vector to this one, resulting in the (component-wise) vector sum. */
    constexpr Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
    /** Subtracts the given vector from this one, resulting in the (component-wise) vector difference. */
    constexpr Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }

    /** Multiplies the given vector with this one, resulting in the (component-wise) Hadamard product. */
    constexpr Vector3& operator*=(const Vector3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
    /** Divides this vector by the given one, resulting in the (component-wise) Hadamard quotient. */
    constexpr Vector3& operator/=(const Vector3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }

    /** Scales this vector by the specified factor (scalar multiplication). */
    constexpr Vector3& operator*=(T s) { x *= s; y *= s; z *= s; return *this; }
    /** Inversely scales this vector by the specified factor (scalar division). */
    constexpr Vector3& operator/=(T s) { x /= s; y /= s; z /= s; return *this; }



    // #############################################################################
    // Access Operators
    // #############################################################################

    T operator[](unsigned i) const { assert(i < 3); return data[i]; }
    T& operator[](unsigned i) { assert(i < 3); return data[i]; }



    // #############################################################################
    // Static Methods
    // #############################################################################

    /** Returns a vector having the minimum components of two given vectors. */
    static Vector3 min2(const Vector3& a, const Vector3& b)
    {
        return {ml7::min2(a.x, b.x), ml7::min2(a.y, b.y), ml7::min2(a.z, b.z)};
    }

    /** Returns a vector having the maximum components of two given vectors. */
    static Vector3 max2(const Vector3& a, const Vector3& b)
    {
        return {ml7::max2(a.x, b.x), ml7::max2(a.y, b.y), ml7::max2(a.z, b.z)};
    }

    /**
     * Performs a linear interpolation between a and b, with y = a + (b-a)x.
     */
    static Vector3 lerp(const Vector3& a, const Vector3& b, T x)
    {
        return a + (b - a) * x;
    }

    /**
     * Performs a cosine interpolation between a and b, with y = a + (b-a)z and
     * z = (1-cos(wx))/2, w = pi.
     */
    static Vector3 terp(const Vector3& a, const Vector3& b, T x)
    {
        x = (T{1} - std::cos(constants<T>::pi * x)) * T{0.5};
        return a + (b - a) * x;
    }

    /**
     * Performs a cubic interpolation between a and b (affected by a0, the point
     * "before" a, and b1, the point "after" b), with y = Px^3 + Qx^2 + Rx + S and
     * P = (b1-b)-(a0-a), Q = (a0-a)-P, R = b-a0, S = a.
     */
    static Vector3 cerp(const Vector3& a0, const Vector3& a, const Vector3& b, const Vector3& b1, T x)
    {
        const Vector3 P = (b1 - b) - (a0 - a);
        const Vector3 Q = (a0 - a) - P;
        const Vector3 R = b - a0;
        const Vector3 S = a;
        const T x2 = x*x;
        return P * x2*x + Q * x2 + R * x + S;
    }



    // #############################################################################
    // Sorting
    // #############################################################################

    struct less
    {
        bool operator()(const Vector3& a, const Vector3& b) const
        {
            if (a.x < b.x) return true;
            if (a.x > b.x) return false;
            if (a.y < b.y) return true;
            if (a.y > b.y) return false;
            return a.z < b.z;
        }
    }; // struct less

}; // struct Vector3



    template <std::floating_point T>
    const Vector3<T> Vector3<T>::ZERO =   {T{0}, T{0}, T{0}};
    template <std::floating_point T>
    const Vector3<T> Vector3<T>::X =      {T{1}, T{0}, T{0}};
    template <std::floating_point T>
    const Vector3<T> Vector3<T>::Y =      {T{0}, T{1}, T{0}};
    template <std::floating_point T>
    const Vector3<T> Vector3<T>::Z =      {T{0}, T{0}, T{1}};



using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;
using Vector3ld = Vector3<long double>;



} // namespace ml7

#endif // ML7_VECTOR3_H
