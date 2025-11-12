#ifndef ML7_VECTOR2_H
#define ML7_VECTOR2_H

#include <MathLabs/math.h>



namespace ml7 {



template <std::floating_point T>
struct Vector2
{

    using scalar_type = T;

    static const Vector2 ZERO;
    static const Vector2 X;
    static const Vector2 Y;



    // #############################################################################
    // Attributes
    // #############################################################################

    union
    {
        struct
        {
            // NOLINTBEGIN(*-use-default-member-init)
            /** The vector's x value (x = u = s). */
            T x;
            /** The vector's y value (y = v = t). */
            T y;
            // NOLINTEND(*-use-default-member-init)
        }; // struct

        struct
        {
            /** The texture coordinate u (u = s = x). */
            T u;
            /** The texture coordinate v (v = t = y). */
            T v;
        };

        struct
        {
            /** The texture coordinate s (s = u = x). */
            T s;
            /** The texture coordinate t (t = v = y). */
            T t;
        };

        /** Array of the two components. */
        T data[2];
    }; // union



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes the vector with x = y = 0.
     */
    constexpr Vector2() noexcept
        : x(T{0})
        , y(T{0})
    {
    }

    /**
     * Explicit constructor with parameters for x and y.
     */
    constexpr Vector2(T x, T y) noexcept
        : x(x)
        , y(y)
    {
    }

    /**
     * Explicit constructor with one parameter for both x and y.
     */
    constexpr explicit Vector2(T c) noexcept
        : x(c)
        , y(c)
    {
    }

    /**
     * Swap operation.
     */
    void swap(Vector2& other) noexcept
    {
        std::swap(x, other.x);
        std::swap(y, other.y);
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /** Returns the magnitude of this vector. */
    T length() const { return std::sqrt(x*x + y*y); }

    /** Returns the squared magnitude of this vector. */
    T length_squared() const { return x*x + y*y; }

    /**
     * Returns the azimuth angle of this vector in the signed range [-pi;+pi], with
     * the positive x-axis pointing east and the positive y-axis pointing north.
     */
    T azimuth_symmetric() const
    {
        return std::atan2(x, y);
    }

    /**
     * Returns the azimuth angle of this vector in the positive range [0;2pi], with
     * the positive x-axis pointing east and the positive y-axis pointing north.
     */
    T azimuth_asymmetric() const
    {
        T a = std::atan2(x, y);
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
        return std::atan2(y, std::abs(x));
    }

    /** Returns a new vector with the x component of this (y = 0). */
    Vector2 x_() const { return {x, T{0}}; }
    /** Returns a new vector with the y component of this (x = 0). */
    Vector2 _y() const { return {T{0}, y}; }

    /** Returns a new vector with all of its components set to the x value of this. */
    Vector2 xx() const { return {x, x}; }
    /** Returns a new vector with all of its components set to the y value of this. */
    Vector2 yy() const { return {y, y}; }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this vector normalized such that it has a magnitude of 1.
     */
    Vector2 normalized() const
    {
        T d = length_squared();
        if (d == T{0})
            return ZERO; // x = y = 0
        d = T{1} / std::sqrt(d);
        return {x * d, y * d};
    }

    /**
     * Returns this vector having positive components.
     */
    Vector2 abs() const
    {
        return {std::abs(x), std::abs(y)};
    }

    /**
     * Returns the dot/scalar product of this and a given vector v.
     */
    T dot(const Vector2& v) const
    {
        return x * v.x + y * v.y;
    }

    /**
     * Returns the squared dot/scalar product of this and a given vector v.
     */
    T dot_squared(const Vector2& v) const
    {
        const T tmp = dot(v);
        return tmp*tmp;
    }

    /**
     * Returns the perpendicular of this vector. The perpendicular corresponds to
     * the vector rotated 90 degrees to the left (counter-clockwise).
     */
    Vector2 perp() const
    {
        return {-y, x};
    }

    /**
     * Returns the perp-dot product of this vector and a given vector v, i.e. the
     * dot/scalar product of both vectors in which this vector is replaced by the
     * perpendicular rotated 90 degrees to the left (counter-clockwise).
     * So, a.perpdot(b) is the same as a.perp().dot(b).
     */
    T perpdot(const Vector2& v) const
    {
        return x * v.y - y * v.x;
    }

    /**
     * Returns the 2D version of a cross product of this vector and a given vector v.
     * This is the same as the so-called perp-dot product in which this vector is
     * replaced by the perpendicular rotated 90 degrees to the left (counter-clockwise).
     * So, a.cross(b) is the same as a.perp().dot(b).
     */
    T cross(const Vector2& v) const
    {
        return x * v.y - y * v.x;
    }

    /**
     * Returns this vector reflected across a plane defined by a given normal
     * vector n.
     */
    Vector2 reflected(const Vector2& n) const
    {
        const Vector2 n2dot = n * T{2} * dot(n);
        return *this - n2dot;
    }

    /**
     * Returns this vector refracted across a plane defined by a given normal
     * vector n with respect to the specified refractive index.
     */
    Vector2 refracted(const Vector2& n, T index) const
    {
        const T t = dot(n);
        const T r = T{1} - index*index * (T{1} - t*t);

        if (r < T{0})
            return {};

        const T s = index * t + std::sqrt(r);
        return index * *this - s * n;
    }

    /**
     * Returns the scalar projection of a given vector v onto this one.
     */
    T comp(const Vector2& v) const
    {
        return dot(v) / length();
    }

    /**
     * Returns the vector projection of a given vector v onto this one.
     */
    Vector2 proj(const Vector2& v) const
    {
        return *this * (dot(v) / length_squared());
    }

    /**
     * Checks whether the given vector represents a counter-clockwise turn with
     * respect to this vector. In that case, the function returns a value greater
     * than 0. A value less than 0 indicates a clockwise turn. 0 indicates that the
     * vectors are collinear.
     */
    T ccw(const Vector2& v) const
    {
        return x * (v.y - y) - (v.x - x) * y;
    }



    // #############################################################################
    // Manipulations
    // #############################################################################

    /**
     * Sets x = y = 0.
     */
    Vector2& clear()
    {
        x = y = T{0};
        return *this;
    }

    /**
     * Inverts the sign of the components of this vector.
     */
    Vector2& invert()
    {
        x = -x;
        y = -y;
        return *this;
    }

    /**
     * Normalizes this vector that it has a magnitude of 1.
     */
    Vector2& normalize()
    {
        *this = normalized();
        return *this;
    }

    /**
     * Lets this vector have a magnitude of the given length.
     */
    Vector2& length(T length)
    {
        normalize();
        x *= length;
        y *= length;
        return *this;
    }

    /**
     * Reflects this vector across a plane defined by a given normal vector n.
     */
    Vector2& reflect(const Vector2& n)
    {
        *this = reflected(n);
        return *this;
    }

    /**
     * Refracts this vector across a plane defined by a given normal vector n with
     * respect to the specified refractive index.
     */
    Vector2& refract(const Vector2& n, T index)
    {
        *this = refracted(n, index);
        return *this;
    }



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const Vector2& v) const noexcept { return (x == v.x) && (y == v.y); }
    bool operator!=(const Vector2& v) const noexcept { return (x != v.x) || (y != v.y); }

    /**
     * Checks if this vector is component-by-component less than a given vector v.
     */
    bool operator<(const Vector2& v) const noexcept
    {
        return (x < v.x) && (y < v.y);
    }

    /**
     * Checks if this vector is component-by-component greater than a given vector v.
     */
    bool operator>(const Vector2& v) const noexcept
    {
        return (x > v.x) && (y > v.y);
    }

    /**
     * Checks if this vector is component-by-component less than or equal to a given
     * vector v.
     */
    bool operator<=(const Vector2& v) const noexcept
    {
        return (x <= v.x) && (y <= v.y);
    }

    /**
     * Checks if this vector is component-by-component greater than or equal to a
     * given vector v.
     */
    bool operator>=(const Vector2& v) const noexcept
    {
        return (x >= v.x) && (y >= v.y);
    }



    // #############################################################################
    // Arithmetic Operators
    // #############################################################################

    /** Returns a copy of this vector unmodified. */
    constexpr Vector2 operator+() const { return *this; }
    /** Returns a copy of this vector with the signs of the elements flipped. */
    constexpr Vector2 operator-() const { return {-x, -y}; }

    /** Returns the (component-wise) vector sum of two vectors. */
    constexpr Vector2 operator+(const Vector2& v) const { return {x + v.x, y + v.y}; }
    /** Returns the (component-wise) vector difference of two vectors. */
    constexpr Vector2 operator-(const Vector2& v) const { return {x - v.x, y - v.y}; }

    /** Returns the (component-wise) Hadamard product of two vectors. */
    constexpr Vector2 operator*(const Vector2& v) const { return {x * v.x, y * v.y}; }
    /** Returns the (component-wise) Hadamard quotient of two vectors. */
    constexpr Vector2 operator/(const Vector2& v) const { return {x / v.x, y / v.y}; }

    /** Returns a copy of this vector scaled by the specified factor (scalar multiplication). */
    constexpr Vector2 operator*(T s) const { return {x * s, y * s}; }
    /** Returns a copy of this vector inversely scaled by the specified factor (scalar division). */
    constexpr Vector2 operator/(T s) const { return {x / s, y / s}; }

    /** Scales a vector by the specified factor (scalar multiplication). */
    friend constexpr Vector2 operator*(T s, const Vector2& v) { return v * s; }



    // #############################################################################
    // Arithmetic Assignment Operators
    // #############################################################################

    /** Adds the given vector to this one, resulting in the (component-wise) vector sum. */
    constexpr Vector2& operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
    /** Subtracts the given vector from this one, resulting in the (component-wise) vector difference. */
    constexpr Vector2& operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }

    /** Multiplies the given vector with this one, resulting in the (component-wise) Hadamard product. */
    constexpr Vector2& operator*=(const Vector2& v) { x *= v.x; y *= v.y; return *this; }
    /** Divides this vector by the given one, resulting in the (component-wise) Hadamard quotient. */
    constexpr Vector2& operator/=(const Vector2& v) { x /= v.x; y /= v.y; return *this; }

    /** Scales this vector by the specified factor (scalar multiplication). */
    constexpr Vector2& operator*=(T s) { x *= s; y *= s; return *this; }
    /** Inversely scales this vector by the specified factor (scalar division). */
    constexpr Vector2& operator/=(T s) { x /= s; y /= s; return *this; }



    // #############################################################################
    // Access Operators
    // #############################################################################

    T operator[](unsigned i) const { assert(i < 2); return data[i]; }
    T& operator[](unsigned i) { assert(i < 2); return data[i]; }



    // #############################################################################
    // Static Methods
    // #############################################################################

    /** Returns a vector having the minimum components of two given vectors. */
    static Vector2 min2(const Vector2& a, const Vector2& b)
    {
        return {ml7::min2(a.x, b.x), ml7::min2(a.y, b.y)};
    }

    /** Returns a vector having the maximum components of two given vectors. */
    static Vector2 max2(const Vector2& a, const Vector2& b)
    {
        return {ml7::max2(a.x, b.x), ml7::max2(a.y, b.y)};
    }

    /**
     * Performs a linear interpolation between a and b, with y = a + (b-a)x.
     */
    static Vector2 lerp(const Vector2& a, const Vector2& b, T x)
    {
        return a + (b - a) * x;
    }

    /**
     * Performs a cosine interpolation between a and b, with y = a + (b-a)z and
     * z = (1-cos(wx))/2, w = pi.
     */
    static Vector2 terp(const Vector2& a, const Vector2& b, T x)
    {
        x = (T{1} - std::cos(constants<T>::pi * x)) * T{0.5};
        return a + (b - a) * x;
    }

    /**
     * Performs a cubic interpolation between a and b (affected by a0, the point
     * "before" a, and b1, the point "after" b), with y = Px^3 + Qx^2 + Rx + S and
     * P = (b1-b)-(a0-a), Q = (a0-a)-P, R = b-a0, S = a.
     */
    static Vector2 cerp(const Vector2& a0, const Vector2& a, const Vector2& b, const Vector2& b1, T x)
    {
        const Vector2 P = (b1 - b) - (a0 - a);
        const Vector2 Q = (a0 - a) - P;
        const Vector2 R = b - a0;
        const Vector2 S = a;
        const T x2 = x*x;
        return P * x2*x + Q * x2 + R * x + S;
    }

    /**
     * Checks whether the three given points are a counter-clockwise turn. In that
     * case, the function returns a value greater than 0. A value less than 0
     * indicates a clockwise turn. 0 indicates that the points are collinear.
     */
    static T ccw(const Vector2& a, const Vector2& b, const Vector2& c)
    {
        return (b.x - a.x) * (c.y - b.y) - (c.x - b.x) * (b.y - a.y);
    }



    // #############################################################################
    // Sorting
    // #############################################################################

    struct less
    {
        bool operator()(const Vector2& a, const Vector2& b) const
        {
            if (a.x < b.x) return true;
            if (a.x > b.x) return false;
            return a.y < b.y;
        }
    }; // struct less

}; // struct Vector2



    template <std::floating_point T>
    const Vector2<T> Vector2<T>::ZERO =   {T{0}, T{0}};
    template <std::floating_point T>
    const Vector2<T> Vector2<T>::X =      {T{1}, T{0}};
    template <std::floating_point T>
    const Vector2<T> Vector2<T>::Y =      {T{0}, T{1}};



using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;
using Vector2ld = Vector2<long double>;



} // namespace ml7

#endif // ML7_VECTOR2_H
