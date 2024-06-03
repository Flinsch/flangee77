#pragma once
#ifndef ML7_VECTOR3_H
#define ML7_VECTOR3_H

#include <CoreLabs/root.h>



namespace ml7 {



class Vector3
{

public:
    static const Vector3 ZERO;
    static const Vector3 X;
    static const Vector3 Y;
    static const Vector3 Z;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default constructor. Initializes the vector with x = y = z = 0.
     */
    constexpr Vector3(void)
        : x( 0.0f )
        , y( 0.0f )
        , z( 0.0f )
    {
    }

    /**
     * Explicit constructor with parameters for x, y, and z.
     */
    constexpr Vector3(float x, float y, float z)
        : x( x )
        , y( y )
        , z( z )
    {
    }

    /**
     * Explicit constructor with one parameter for x, y, and z.
     */
    constexpr explicit Vector3(float c)
        : x( c )
        , y( c )
        , z( c )
    {
    }

    /**
     * Swap operation.
     */
    void swap(Vector3& rhs);



    // #############################################################################
    // Attributes
    // #############################################################################
public:
    union
    {
        struct
        {
            /** The vector's x value. */
            float x;
            /** The vector's y value. */
            float y;
            /** The vector's z value. */
            float z;
        }; // struct

        /** Array of all three componets. */
        float data[3];
    }; // union



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /** Returns the magnitude of the vector. */
    float length() const { return ::sqrtf( x*x + y*y + z*z ); }

    /** Returns the squared magnitude of the vector. */
    float lensqr() const { return x*x + y*y + z*z; }

    /**
     * Returns the azimuth angle of this vector in the signed range [-pi;+pi], with
     * the positive x-axis pointing east and the positive z-axis pointing north.
     */
    float azimuth_symmetric() const;

    /**
     * Returns the azimuth angle of this vector in the positive range [0;2pi], with
     * the positive x-axis pointing east and the positive z-axis pointing north.
     */
    float azimuth_asymmetric() const;

    /**
     * Returns the elevation angle of this vector in the range [-pi/2;+pi/2], with
     * the positive y-axis pointing up.
     */
    float elevation() const;

    /** Returns a new vector with the x component of this (y = z = 0). */
    Vector3 x__() const { return Vector3( x, 0.0f, 0.0f ); }
    /** Returns a new vector with the y component of this (x = z = 0). */
    Vector3 _y_() const { return Vector3( 0.0f, y, 0.0f ); }
    /** Returns a new vector with the y component of this (x = y = 0). */
    Vector3 __z() const { return Vector3( 0.0f, 0.0f, z ); }

    /** Returns a new vector with the x and y components of this (z = 0). */
    Vector3 xy_() const { return Vector3( x, y, 0.0f ); }
    /** Returns a new vector with the x and z components of this (y = 0). */
    Vector3 x_z() const { return Vector3( x, 0.0f, z ); }
    /** Returns a new vector with the y and z components of this (x = 0). */
    Vector3 _yz() const { return Vector3( 0.0f, y, z ); }

    /** Returns a new vector with all of its components set to the x value of this. */
    Vector3 xxx() const { return Vector3( x, x, x ); }
    /** Returns a new vector with all of its components set to the y value of this. */
    Vector3 yyy() const { return Vector3( y, y, y ); }
    /** Returns a new vector with all of its components set to the y value of this. */
    Vector3 zzz() const { return Vector3( z, z, z ); }



    // #############################################################################
    // Transformations
    // #############################################################################
public:
    /**
     * Returns a copy of this vector normalized.
     */
    Vector3 normalized() const
    {
        float d = lensqr();
        if ( d == 0.0f )
            return Vector3( 0.0f ); // x = y = z = 0
        d = 1.0f / ::sqrtf( d );
        return Vector3( x*d, y*d, z*d );
    }

    /**
     * Returns this vector having positive components.
     */
    Vector3 abs() const
    {
        return Vector3( ::abs(x), ::abs(y), ::abs(z) );
    }

    /**
     * Returns the dot product of this and a given vector v.
     */
    float dot(const Vector3& v) const
    {
        return x*v.x + y*v.y + z*v.z;
    }

    /**
     * Returns the squared dot product of this and a given vector v.
     */
    float dotsqr(const Vector3& v) const
    {
        const float tmp = dot(v);
        return tmp*tmp;
    }

    /**
     * Returns the cross product of this and a given vector v.
     */
    Vector3 cross(const Vector3& v) const
    {
        return Vector3(
            y*v.z - z*v.y,
            z*v.x - x*v.z,
            x*v.y - y*v.x );
    }

    /**
     * Returns this vector reflected across a plane defined by a given normal
     * vector n.
     */
    Vector3 reflected(const Vector3& n) const
    {
        const ml7::Vector3 n2dot = n * 2.0f * dot(n);
        return *this - n2dot;
    }

    /**
     * Returns this vector refracted across a plane defined by a given normal
     * vector n with respect to the specified refractive index.
     */
    Vector3 refracted(const Vector3& n, float index) const;

    /**
     * Returns the scalar projection of a given vector v onto this.
     */
    float comp(const Vector3& v) const
    {
        return dot( v ) / length();
    }

    /**
     * Returns the vector projection of a given vector v onto this.
     */
    Vector3 proj(const Vector3& v) const
    {
        return *this * ( dot( v ) / lensqr() );
    }



    // #############################################################################
    // Manipulations
    // #############################################################################
public:
    /**
     * Sets x = y = z = 0.
     */
    Vector3& clear()
    {
        x = y = z = 0.0f;
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
    Vector3& length(float length)
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
        *this = reflected( n );
        return *this;
    }

    /**
     * Refracts this vector across a plane defined by a given normal vector n with
     * respect to the specified refractive index.
     */
    Vector3& refract(const Vector3& n, float index)
    {
        *this = refracted( n, index );
        return *this;
    }



    // #############################################################################
    // Access Operators
    // #############################################################################
public:
    float operator[] (unsigned i) const { assert( i < 3 ); return data[ i ]; }
    float& operator[] (unsigned i) { assert( i < 3 ); return data[ i ]; }



    // #############################################################################
    // Arithmetic Operators
    // #############################################################################
public:
    constexpr Vector3 operator + () const { return *this; }
    constexpr Vector3 operator - () const { return Vector3( -x, -y, -z ); }

    constexpr Vector3 operator + (const Vector3& v) const { return Vector3( x + v.x, y + v.y, z + v.z ); }
    constexpr Vector3 operator - (const Vector3& v) const { return Vector3( x - v.x, y - v.y, z - v.z ); }

    constexpr Vector3 operator * (const Vector3& v) const { return Vector3( x * v.x, y * v.y, z * v.z ); }
    constexpr Vector3 operator / (const Vector3& v) const { return Vector3( x / v.x, y / v.y, z / v.z ); }

    constexpr Vector3 operator * (float s) const { return Vector3( x * s, y * s, z * s ); }
    constexpr Vector3 operator / (float s) const { return Vector3( x / s, y / s, z / s ); }



    // #############################################################################
    // Arithmetic Assignment Operators
    // #############################################################################
public:
    constexpr Vector3& operator += (const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
    constexpr Vector3& operator -= (const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }

    constexpr Vector3& operator *= (const Vector3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
    constexpr Vector3& operator /= (const Vector3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }

    constexpr Vector3& operator *= (float s) { x *= s; y *= s; z *= s; return *this; }
    constexpr Vector3& operator /= (float s) { x /= s; y /= s; z /= s; return *this; }



    // #############################################################################
    // Comparison Operators
    // #############################################################################
public:
    bool operator == (const Vector3& v) const { return (x == v.x) && (y == v.y) && (z == v.z); }
    bool operator != (const Vector3& v) const { return (x != v.x) || (y != v.y) || (z != v.z); }

    /**
     * Checks if this vector is component-by-component less than a given vector v.
     */
    bool operator < (const Vector3& v) const
    {
        return (x < v.x) && (y < v.y) && (z < v.z);
    }

    /**
     * Checks if this vector is component-by-component greater than a given vector v.
     */
    bool operator > (const Vector3& v) const
    {
        return (x > v.x) && (y > v.y) && (z > v.z);
    }

    /**
     * Checks if this vector is component-by-component less than or equal to a given
     * vector v.
     */
    bool operator <= (const Vector3& v) const
    {
        return (x <= v.x) && (y <= v.y) && (z <= v.z);
    }

    /**
     * Checks if this vector is component-by-component greater than or equal to a
     * given vector v.
     */
    bool operator >= (const Vector3& v) const
    {
        return (x >= v.x) && (y >= v.y) && (z >= v.z);
    }



    // #############################################################################
    // Static Methods
    // #############################################################################
public:
    /** Returns a vector having the minimum components of two given vectors. */
    static Vector3 min2(const Vector3& a, const Vector3& b);

    /** Returns a vector having the maximum components of two given vectors. */
    static Vector3 max2(const Vector3& a, const Vector3& b);

    /**
     * Performs a linear interpolation between a and b, with y = a + (b-a)x.
     */
    static Vector3 lerp(const Vector3& a, const Vector3& b, float x);

    /**
     * Performs a cosine interpolation between a and b, with y = a + (b-a)z and
     * z = (1-cos(wx))/2, w = pi.
     */
    static Vector3 terp(const Vector3& a, const Vector3& b, float x);

    /**
     * Performs a cubic interpolation between a and b (affected by a0, the point
     * "before" a, and b1, the point "after" b), with y = Px^3 + Qx^2 + Rx + S and
     * P = (b1-b)-(a0-a), Q = (a0-a)-P, R = b-a0, S = a.
     */
    static Vector3 cerp(const Vector3& a0, const Vector3& a, const Vector3& b, const Vector3& b1, float x);



    // #############################################################################
    // Sorting
    // #############################################################################
public:
    struct less
    {
        bool operator () (const ml7::Vector3& a, const ml7::Vector3& b) const
        {
            if ( a.x < b.x ) return true;
            if ( a.x > b.x ) return false;
            if ( a.y < b.y ) return true;
            if ( a.y > b.y ) return false;
            return a.z < b.z;
        }
    }; // struct less

}; // class Vector3



    // #############################################################################
    // Additional Operators
    // #############################################################################

    constexpr Vector3 operator * (float s, const Vector3& v) { return Vector3( s * v.x, s * v.y, s * v.z ); }



} // namespace ml7

#endif // ML7_VECTOR3_H
