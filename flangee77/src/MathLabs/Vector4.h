#ifndef ML7_VECTOR4_H
#define ML7_VECTOR4_H

#include "./Vector3.h"



namespace ml7 {



template <std::floating_point T>
struct Vector4
{

    using scalar_type = T;

    static const Vector4 ZERO;
    static const Vector4 X;
    static const Vector4 Y;
    static const Vector4 Z;



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
            /** The vector's w value. */
            T w;
            // NOLINTEND(*-use-default-member-init)
        }; // struct

        /** Array of all four components. */
        T data[4];
    }; // union



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes the vector with x = y = z = w = 0.
     */
    constexpr Vector4() noexcept
        : x(T{0})
        , y(T{0})
        , z(T{0})
        , w(T{0})
    {
    }

    /**
     * Explicit constructor with parameters for x, y, and z. Sets w = 1.
     */
    constexpr Vector4(T x, T y, T z) noexcept
        : x(x)
        , y(y)
        , z(z)
        , w(T{1})
    {
    }

    /**
     * Explicit constructor with parameters for x, y, z, and w.
     */
    constexpr Vector4(T x, T y, T z, T w) noexcept
        : x(x)
        , y(y)
        , z(z)
        , w(w)
    {
    }

    /**
     * Explicit constructor with one parameter for x, y, z, and w.
     */
    constexpr explicit Vector4(T c) noexcept
        : x(c)
        , y(c)
        , z(c)
        , w(c)
    {
    }

    /**
     * Explicit constructor with one parameter for x, y, and z and one for w.
     */
    constexpr Vector4(T c, T w) noexcept
        : x(c)
        , y(c)
        , z(c)
        , w(w)
    {
    }

    /**
     * Explicit constructor with one parameter for a 3D vector. Sets w = 1.
     */
    constexpr explicit Vector4(const Vector3<T>& v) noexcept
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(T{1})
    {
    }

    /**
     * Explicit constructor with one parameter for a 3D vector and one for w.
     */
    constexpr explicit Vector4(const Vector3<T>& v, T w) noexcept
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(w)
    {
    }

    /**
     * Swap operation.
     */
    void swap(Vector4& other) noexcept
    {
        std::swap(x, other.x);
        std::swap(y, other.y);
        std::swap(z, other.z);
        std::swap(w, other.w);
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /** Returns the magnitude of this vector. */
    T length() const { return std::sqrt(x*x + y*y + z*z + w*w); }

    /** Returns the squared magnitude of this vector. */
    T length_squared() const { return x*x + y*y + z*z + w*w; }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Interprets this 4D vector as a homogeneous 3D vector and returns a
     * dehomogenized copy of it with w = 1 (if w != 0).
     */
    Vector4 dehomogenized() const
    {
        if (w == T{0})
            return *this; // Do nothing!
        const T rw = T{1} / w;
        return {x * rw, y * rw, z * rw, T{1}};
    }

    /**
     * Returns a copy of this vector normalized such that it has a magnitude of 1.
     */
    Vector4 normalized() const
    {
        T d = length_squared();
        if (d == T{0})
            return ZERO; // x = y = z = w = 0
        d = T{1} / std::sqrt(d);
        return {x * d, y * d, z * d, w * d};
    }

    /**
     * Returns this vector having positive components.
     */
    Vector4 abs() const
    {
        return {std::abs(x), std::abs(y), std::abs(z), std::abs(w)};
    }

    /**
     * Interprets this 4D vector as a homogeneous 3D vector and returns a
     * dehomogenized 3D "copy" of it.
     */
    Vector3<T> to_vector3() const
    {
        const Vector4 v = dehomogenized();
        return {v.x, v.y, v.z};
    }



    // #############################################################################
    // Manipulations
    // #############################################################################

    /**
     * Sets x = y = z = w = 0.
     */
    Vector4& clear()
    {
        x = y = z = w = T{0};
        return *this;
    }

    /**
     * Inverts the sign of the components of this vector.
     */
    Vector4& invert()
    {
        x = -x;
        y = -y;
        z = -z;
        w = -w;
        return *this;
    }

    /**
     * Interprets this 4D vector as a homogeneous 3D vector and dehomogenizes it so
     * that w = 1 (if w != 0).
     */
    Vector4& dehomogenize()
    {
        if (w == T{0})
            return *this; // Do nothing!
        const T rw = T{1} / w;
        x *= rw;
        y *= rw;
        z *= rw;
        w = T{1};
        return *this;
    }

    /**
     * Normalizes this vector that it has a magnitude of 1.
     */
    Vector4& normalize()
    {
        *this = normalized();
        return *this;
    }

    /**
     * Lets this vector have a magnitude of the given length.
     */
    Vector4& length(T length)
    {
        normalize();
        x *= length;
        y *= length;
        z *= length;
        w *= length;
        return *this;
    }



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const Vector4& v) const noexcept { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
    bool operator!=(const Vector4& v) const noexcept { return (x != v.x) || (y != v.y) || (z != v.z) || (w != v.w); }

    /**
     * Checks if this vector is component-by-component less than a given vector v.
     */
    bool operator<(const Vector4& v) const noexcept
    {
        return (x < v.x) && (y < v.y) && (z < v.z) && (w < v.w);
    }

    /**
     * Checks if this vector is component-by-component greater than a given vector v.
     */
    bool operator>(const Vector4& v) const noexcept
    {
        return (x > v.x) && (y > v.y) && (z > v.z) && (w > v.w);
    }

    /**
     * Checks if this vector is component-by-component less than or equal to a given
     * vector v.
     */
    bool operator<=(const Vector4& v) const noexcept
    {
        return (x <= v.x) && (y <= v.y) && (z <= v.z) && (w <= v.w);
    }

    /**
     * Checks if this vector is component-by-component greater than or equal to a
     * given vector v.
     */
    bool operator>=(const Vector4& v) const noexcept
    {
        return (x >= v.x) && (y >= v.y) && (z >= v.z) && (w >= v.w);
    }



    // #############################################################################
    // Arithmetic Operators
    // #############################################################################

    /** Returns a copy of this vector unmodified. */
    constexpr Vector4 operator+() const { return *this; }
    /** Returns a copy of this vector with the signs of the elements flipped. */
    constexpr Vector4 operator-() const { return {-x, -y, -z, -w}; }

    /** Returns the (component-wise) vector sum of two vectors. */
    constexpr Vector4 operator+(const Vector4& v) const { return {x + v.x, y + v.y, z + v.z, w + v.w}; }
    /** Returns the (component-wise) vector difference of two vectors. */
    constexpr Vector4 operator-(const Vector4& v) const { return {x - v.x, y - v.y, z - v.z, w - v.w}; }

    /** Returns the (component-wise) Hadamard product of two vectors. */
    constexpr Vector4 operator*(const Vector4& v) const { return {x * v.x, y * v.y, z * v.z, w * v.w}; }
    /** Returns the (component-wise) Hadamard quotient of two vectors. */
    constexpr Vector4 operator/(const Vector4& v) const { return {x / v.x, y / v.y, z / v.z, w / v.w}; }

    /** Returns a copy of this vector scaled by the specified factor (scalar multiplication). */
    constexpr Vector4 operator*(T s) const { return {x * s, y * s, z * s, w * s}; }
    /** Returns a copy of this vector inversely scaled by the specified factor (scalar division). */
    constexpr Vector4 operator/(T s) const { return {x / s, y / s, z / s, w / s}; }

    /** Scales a vector by the specified factor (scalar multiplication). */
    friend constexpr Vector4 operator*(T s, const Vector4& v) { return v * s; }



    // #############################################################################
    // Arithmetic Assignment Operators
    // #############################################################################

    /** Adds the given vector to this one, resulting in the (component-wise) vector sum. */
    constexpr Vector4& operator+=(const Vector4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
    /** Subtracts the given vector from this one, resulting in the (component-wise) vector difference. */
    constexpr Vector4& operator-=(const Vector4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }

    /** Multiplies the given vector with this one, resulting in the (component-wise) Hadamard product. */
    constexpr Vector4& operator*=(const Vector4& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
    /** Divides this vector by the given one, resulting in the (component-wise) Hadamard quotient. */
    constexpr Vector4& operator/=(const Vector4& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }

    /** Scales this vector by the specified factor (scalar multiplication). */
    constexpr Vector4& operator*=(T s) { x *= s; y *= s; z *= s; w *= s; return *this; }
    /** Inversely scales this vector by the specified factor (scalar division). */
    constexpr Vector4& operator/=(T s) { x /= s; y /= s; z /= s; w /= s; return *this; }



    // #############################################################################
    // Access Operators
    // #############################################################################

    T operator[](unsigned i) const { assert(i < 4); return data[i]; }
    T& operator[](unsigned i) { assert(i < 4); return data[i]; }



    // #############################################################################
    // Static Methods
    // #############################################################################

    /** Returns a vector having the minimum components of two given vectors. */
    static Vector4 min2(const Vector4& a, const Vector4& b)
    {
        return {ml7::min2(a.x, b.x), ml7::min2(a.y, b.y), ml7::min2(a.z, b.z), ml7::min2(a.w, b.w)};
    }

    /** Returns a vector having the maximum components of two given vectors. */
    static Vector4 max2(const Vector4& a, const Vector4& b)
    {
        return {ml7::max2(a.x, b.x), ml7::max2(a.y, b.y), ml7::max2(a.z, b.z), ml7::max2(a.w, b.w)};
    }

    /**
     * Performs a linear interpolation between a and b, with y = a + (b-a)x.
     */
    static Vector4 lerp(const Vector4& a, const Vector4& b, T x)
    {
        return a + (b - a) * x;
    }

    /**
     * Performs a cosine interpolation between a and b, with y = a + (b-a)z and
     * z = (1-cos(wx))/2, w = pi.
     */
    static Vector4 terp(const Vector4& a, const Vector4& b, T x)
    {
        x = (T{1} - std::cos(constants<T>::pi * x)) * T{0.5};
        return a + (b - a) * x;
    }

    /**
     * Performs a cubic interpolation between a and b (affected by a0, the point
     * "before" a, and b1, the point "after" b), with y = Px^3 + Qx^2 + Rx + S and
     * P = (b1-b)-(a0-a), Q = (a0-a)-P, R = b-a0, S = a.
     */
    static Vector4 cerp(const Vector4& a0, const Vector4& a, const Vector4& b, const Vector4& b1, T x)
    {
        const Vector4 P = (b1 - b) - (a0 - a);
        const Vector4 Q = (a0 - a) - P;
        const Vector4 R = b - a0;
        const Vector4 S = a;
        const T x2 = x*x;
        return P * x2*x + Q * x2 + R * x + S;
    }



    // #############################################################################
    // Sorting
    // #############################################################################

    struct less
    {
        bool operator()(const Vector4& a, const Vector4& b) const
        {
            if (a.x < b.x) return true;
            if (a.x > b.x) return false;
            if (a.y < b.y) return true;
            if (a.y > b.y) return false;
            if (a.z < b.z) return true;
            if (a.z > b.z) return false;
            return a.w < b.w;
        }
    }; // struct less

}; // struct Vector4



    template <std::floating_point T>
    const Vector4<T> Vector4<T>::ZERO =   {T{0}, T{0}, T{0}};
    template <std::floating_point T>
    const Vector4<T> Vector4<T>::X =      {T{1}, T{0}, T{0}};
    template <std::floating_point T>
    const Vector4<T> Vector4<T>::Y =      {T{0}, T{1}, T{0}};
    template <std::floating_point T>
    const Vector4<T> Vector4<T>::Z =      {T{0}, T{0}, T{1}};



using Vector4f = Vector4<float>;
using Vector4d = Vector4<double>;
using Vector4ld = Vector4<long double>;



} // namespace ml7

#endif // ML7_VECTOR4_H
