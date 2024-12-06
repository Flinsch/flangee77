#pragma once
#ifndef ML7_VECTOR2_H
#define ML7_VECTOR2_H

#include <MathLabs/math.h>



namespace ml7 {



class Vector2
{

public:
    static const Vector2 ZERO;
    static const Vector2 X;
    static const Vector2 Y;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default constructor. Initializes the vector with x = y = 0.
     */
    constexpr Vector2()
        : x( 0.0f )
        , y( 0.0f )
    {
    }

    /**
     * Explicit constructor with parameters for x and y.
     */
    constexpr Vector2(float x, float y)
        : x( x )
        , y( y )
    {
    }

    /**
     * Explicit constructor with one parameter for both x and y.
     */
    constexpr explicit Vector2(float c)
        : x( c )
        , y( c )
    {
    }

    /**
     * Swap operation.
     */
    void swap(Vector2& rhs);



    // #############################################################################
    // Attributes
    // #############################################################################
public:
    union
    {
        struct
        {
            /** The vector's x value (x = u = s). */
            float x;
            /** The vector's y value (y = v = t). */
            float y;
        }; // struct

        struct
        {
            /** The texture coordinate u (u = s = x). */
            float u;
            /** The texture coordinate v (v = t = y). */
            float v;
        };

        struct
        {
            /** The texture coordinate s (s = u = x). */
            float s;
            /** The texture coordinate t (t = v = y). */
            float t;
        };

        /** Array of the two components. */
        float data[2];
    }; // union



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /** Returns the magnitude of this vector. */
    float length() const { return ::sqrtf( x*x + y*y ); }

    /** Returns the squared magnitude of this vector. */
    float lensqr() const { return x*x + y*y; }

    /**
     * Returns the azimuth angle of this vector in the signed range [-pi;+pi], with
     * the positive x-axis pointing east and the positive y-axis pointing north.
     */
    float azimuth_symmetric() const;

    /**
     * Returns the azimuth angle of this vector in the positive range [0;2pi], with
     * the positive x-axis pointing east and the positive y-axis pointing north.
     */
    float azimuth_asymmetric() const;

    /**
     * Returns the elevation angle of this vector in the range [-pi/2;+pi/2], with
     * the positive y-axis pointing up.
     */
    float elevation() const;

    /** Returns a new vector with the x component of this (y = 0). */
    Vector2 x_() const { return Vector2( x, 0.0f ); }
    /** Returns a new vector with the y component of this (x = 0). */
    Vector2 _y() const { return Vector2( 0.0f, y ); }

    /** Returns a new vector with all of its components set to the x value of this. */
    Vector2 xx() const { return Vector2( x, x ); }
    /** Returns a new vector with all of its components set to the y value of this. */
    Vector2 yy() const { return Vector2( y, y ); }



    // #############################################################################
    // Transformations
    // #############################################################################
public:
    /**
     * Returns a copy of this vector normalized.
     */
    Vector2 normalized() const
    {
        float d = lensqr();
        if ( d == 0.0f )
            return ZERO; // x = y = 0
        d = 1.0f / ::sqrtf( d );
        return Vector2( x*d, y*d );
    }

    /**
     * Returns this vector having positive components.
     */
    Vector2 abs() const
    {
        return Vector2( ::abs(x), ::abs(y) );
    }

    /**
     * Returns the dot/scalar product of this and a given vector v.
     */
    float dot(const Vector2& v) const
    {
        return x*v.x + y*v.y;
    }

    /**
     * Returns the squared dot/scalar product of this and a given vector v.
     */
    float dotsqr(const Vector2& v) const
    {
        const float tmp = dot(v);
        return tmp*tmp;
    }

    /**
     * Returns the perpendicular of this vector. The perpendicular corresponds to
     * the vector rotated 90 degrees to the left (counter-clockwise).
     */
    Vector2 perp() const
    {
        return Vector2( -y, x );
    }

    /**
     * Returns the perp-dot product of this vector and a given vector v, i.e. the
     * dot/scalar product of both vectors in which this vector is replaced by the
     * perpendicular rotated 90 degrees to the left (counter-clockwise).
     * So, a.perpdot(b) is the same as a.perp().dot(b).
     */
    float perpdot(const Vector2& v) const
    {
        return x*v.y - y*v.x;
    }

    /**
     * Returns this vector reflected across a plane defined by a given normal
     * vector n.
     */
    Vector2 reflected(const Vector2& n) const
    {
        const ml7::Vector2 n2dot = n * 2.0f * dot(n);
        return *this - n2dot;
    }

    /**
     * Returns this vector refracted across a plane defined by a given normal
     * vector n with respect to the specified refractive index.
     */
    Vector2 refracted(const Vector2& n, float index) const;

    /**
     * Returns the scalar projection of a given vector v onto this one.
     */
    float comp(const Vector2& v) const
    {
        return dot( v ) / length();
    }

    /**
     * Returns the vector projection of a given vector v onto this one.
     */
    Vector2 proj(const Vector2& v) const
    {
        return *this * ( dot( v ) / lensqr() );
    }



    // #############################################################################
    // Manipulations
    // #############################################################################
public:
    /**
     * Sets x = y = 0.
     */
    Vector2& clear()
    {
        x = y = 0.0f;
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
    Vector2& length(float length)
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
        *this = reflected( n );
        return *this;
    }

    /**
     * Refracts this vector across a plane defined by a given normal vector n with
     * respect to the specified refractive index.
     */
    Vector2& refract(const Vector2& n, float index)
    {
        *this = refracted( n, index );
        return *this;
    }



    // #############################################################################
    // Access Operators
    // #############################################################################
public:
    float operator[] (unsigned i) const { assert( i < 2 ); return data[ i ]; }
    float& operator[] (unsigned i) { assert( i < 2 ); return data[ i ]; }



    // #############################################################################
    // Arithmetic Operators
    // #############################################################################
public:
    /** Returns a copy of this vector unmodified. */
    constexpr Vector2 operator + () const { return *this; }
    /** Returns a copy of this vector with the signs of the elements flipped. */
    constexpr Vector2 operator - () const { return Vector2( -x, -y ); }

    /** Returns the (component-wise) vector sum of two vectors. */
    constexpr Vector2 operator + (const Vector2& v) const { return Vector2( x + v.x, y + v.y ); }
    /** Returns the (component-wise) vector difference of two vectors. */
    constexpr Vector2 operator - (const Vector2& v) const { return Vector2( x - v.x, y - v.y ); }

    /** Returns the (component-wise) Hadamard product of two vectors. */
    constexpr Vector2 operator * (const Vector2& v) const { return Vector2( x * v.x, y * v.y ); }
    /** Returns the (component-wise) Hadamard quotient of two vectors. */
    constexpr Vector2 operator / (const Vector2& v) const { return Vector2( x / v.x, y / v.y ); }

    /** Returns a copy of this vector scaled by the specified factor (scalar multiplication). */
    constexpr Vector2 operator * (float s) const { return Vector2( x * s, y * s ); }
    /** Returns a copy of this vector inversely scaled by the specified factor (scalar division). */
    constexpr Vector2 operator / (float s) const { return Vector2( x / s, y / s ); }



    // #############################################################################
    // Arithmetic Assignment Operators
    // #############################################################################
public:
    /** Adds the given vector to this one, resulting in the (component-wise) vector sum. */
    constexpr Vector2& operator += (const Vector2& v) { x += v.x; y += v.y; return *this; }
    /** Subtracts the given vector from this one, resulting in the (component-wise) vector difference. */
    constexpr Vector2& operator -= (const Vector2& v) { x -= v.x; y -= v.y; return *this; }

    /** Multiplies the given vector with this one, resulting in the (component-wise) Hadamard product. */
    constexpr Vector2& operator *= (const Vector2& v) { x *= v.x; y *= v.y; return *this; }
    /** Divides this vector by the given one, resulting in the (component-wise) Hadamard quotient. */
    constexpr Vector2& operator /= (const Vector2& v) { x /= v.x; y /= v.y; return *this; }

    /** Scales this vector by the specified factor (scalar multiplication). */
    constexpr Vector2& operator *= (float s) { x *= s; y *= s; return *this; }
    /** Inversely scales this vector by the specified factor (scalar division). */
    constexpr Vector2& operator /= (float s) { x /= s; y /= s; return *this; }



    // #############################################################################
    // Comparison Operators
    // #############################################################################
public:
    bool operator == (const Vector2& v) const { return (x == v.x) && (y == v.y); }
    bool operator != (const Vector2& v) const { return (x != v.x) || (y != v.y); }

    /**
     * Checks if this vector is component-by-component less than a given vector v.
     */
    bool operator < (const Vector2& v) const
    {
        return (x < v.x) && (y < v.y);
    }

    /**
     * Checks if this vector is component-by-component greater than a given vector v.
     */
    bool operator > (const Vector2& v) const
    {
        return (x > v.x) && (y > v.y);
    }

    /**
     * Checks if this vector is component-by-component less than or equal to a given
     * vector v.
     */
    bool operator <= (const Vector2& v) const
    {
        return (x <= v.x) && (y <= v.y);
    }

    /**
     * Checks if this vector is component-by-component greater than or equal to a
     * given vector v.
     */
    bool operator >= (const Vector2& v) const
    {
        return (x >= v.x) && (y >= v.y);
    }



    // #############################################################################
    // Static Methods
    // #############################################################################
public:
    /** Returns a vector having the minimum components of two given vectors. */
    static Vector2 min2(const Vector2& a, const Vector2& b);

    /** Returns a vector having the maximum components of two given vectors. */
    static Vector2 max2(const Vector2& a, const Vector2& b);

    /**
     * Performs a linear interpolation between a and b, with y = a + (b-a)x.
     */
    static Vector2 lerp(const Vector2& a, const Vector2& b, float x);

    /**
     * Performs a cosine interpolation between a and b, with y = a + (b-a)z and
     * z = (1-cos(wx))/2, w = pi.
     */
    static Vector2 terp(const Vector2& a, const Vector2& b, float x);

    /**
     * Performs a cubic interpolation between a and b (affected by a0, the point
     * "before" a, and b1, the point "after" b), with y = Px^3 + Qx^2 + Rx + S and
     * P = (b1-b)-(a0-a), Q = (a0-a)-P, R = b-a0, S = a.
     */
    static Vector2 cerp(const Vector2& a0, const Vector2& a, const Vector2& b, const Vector2& b1, float x);

    /**
     * Checks whether the three given points are a counter-clockwise turn. In that
     * case, the function returns a value greater than 0. A value less than 0
     * indicates a clockwise turn. 0 indicates that the points are collinear.
     */
    static float ccw(const ml7::Vector2& a, const ml7::Vector2& b, const ml7::Vector2& c);



    // #############################################################################
    // Sorting
    // #############################################################################
public:
    struct less
    {
        bool operator () (const ml7::Vector2& a, const ml7::Vector2& b) const
        {
            if ( a.x < b.x ) return true;
            if ( a.x > b.x ) return false;
            return a.y < b.y;
        }
    }; // struct less

}; // class Vector2



    // #############################################################################
    // Additional Operators
    // #############################################################################

    /** Scales a vector by the specified factor (scalar multiplication). */
    constexpr Vector2 operator * (float s, const Vector2& v) { return v * s; }



} // namespace ml7

#endif // ML7_VECTOR2_H
