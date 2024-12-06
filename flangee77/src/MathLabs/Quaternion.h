#pragma once
#ifndef ML7_QUATERNION_H
#define ML7_QUATERNION_H

#include "./Matrix3x3.h"
#include "./Vector3.h"



namespace ml7 {



class Quaternion
{

public:
    static const Quaternion ZERO;
    static const Quaternion IDENTITY;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default constructor. Initializes the quaternion with v = (0, 0, 0) and s = 1.
     */
    constexpr Quaternion()
        : x( 0.0f )
        , y( 0.0f )
        , z( 0.0f )
        , w( 1.0f )
    {
    }

    /**
     * Explicit constructor with parameters for the vector/imaginary part
     * v = (x, y, z) and the scalar/real part s = w.
     */
    constexpr Quaternion(float x, float y, float z, float w)
        : x( x )
        , y( y )
        , z( z )
        , w( w )
    {
    }

    /**
     * Explicit constructor with parameters for the vector/imaginary part v and the
     * scalar/real part s.
     */
    constexpr Quaternion(const ml7::Vector3& v, float s)
        : x( v.x )
        , y( v.y )
        , z( v.z )
        , w( s )
    {
    }

    /**
     * Explicit constructor that initializes a quaternion from the specified
     * rotation matrix.
     */
    constexpr explicit Quaternion(const ml7::Matrix3x3& m)
    {
        // https://www.ljll.fr/~frey/papers/scientific%20visualisation/Shoemake%20K.,%20Quaternions.pdf
        const float t = m._11 + m._22 + m._33;
        if ( t >= 0.0f )
        {
            const float r = ::sqrtf( t + 1.0f );
            const float _05i = 0.5f / r;
            x = (m._32 - m._23) * _05i;
            y = (m._13 - m._31) * _05i;
            z = (m._21 - m._12) * _05i;
            w = 0.5f * r;
        }
        else // => t < 0.0f
        {
            unsigned i = 0;
            if ( m._22 > m._11 ) i = 1;
            if ( m._33 > m[i][i] ) i = 2;
            unsigned j = (i + 1) % 3;
            unsigned k = (i + 2) % 3;

            const float r = ::sqrtf( m[i][i] - m[j][j] - m[k][k] + 1.0f );
            const float _05i = 0.5f / r;
            data[i] = 0.5f * r;
            data[j] = (m[i][j] + m[j][i]) * _05i;
            data[k] = (m[k][i] + m[i][k]) * _05i;
            w       = (m[k][j] - m[j][k]) * _05i;
        }
    }

    /**
     * Initializes a quaternion from the specified rotation matrix.
     */
    static constexpr Quaternion from_matrix3x3(const ml7::Matrix3x3& m)
    {
        return Quaternion( m );
    }

    /**
     * Initializes a quaternion from the specified basis vectors that define the
     * transformed coordinate system.
     */
    static constexpr Quaternion from_axes(const ml7::Vector3& x, const ml7::Vector3& y, const ml7::Vector3& z)
    {
        return from_matrix3x3( ml7::Matrix3x3::from_axes( x, y, z ) );
    }

    /**
     * Initializes a quaternion representing a rotation by a certain angle (in
     * radians) around the x-axis.
     */
    static Quaternion rotx(float angle)
    {
        const float sn05 = ::sinf( angle * 0.5f );
        const float cs05 = ::cosf( angle * 0.5f );
        return {
            sn05,
            0.0f,
            0.0f,
            cs05,
        };
    }

    /**
     * Initializes a quaternion representing a rotation by a certain angle (in
     * radians) around the y-axis.
     */
    static Quaternion roty(float angle)
    {
        const float sn05 = ::sinf( angle * 0.5f );
        const float cs05 = ::cosf( angle * 0.5f );
        return {
            0.0f,
            sn05,
            0.0f,
            cs05,
        };
    }

    /**
     * Initializes a quaternion representing a rotation by a certain angle (in
     * radians) around the z-axis.
     */
    static Quaternion rotz(float angle)
    {
        const float sn05 = ::sinf( angle * 0.5f );
        const float cs05 = ::cosf( angle * 0.5f );
        return {
            0.0f,
            0.0f,
            sn05,
            cs05,
        };
    }

    /**
     * Initializes a quaternion representing a rotation around the specified axis,
     * with an angle equal to the magnitude of the specified vector (in radians).
     */
    static Quaternion rotation(const ml7::Vector3& axis_angle)
    {
        const float angle = axis_angle.length();
        if ( !angle )
            return IDENTITY;
        return rotation_normalized( axis_angle / angle, angle );
    }

    /**
     * Initializes a quaternion representing a rotation by a certain angle (in
     * radians) around the specified axis.
     */
    static Quaternion rotation(const ml7::Vector3& axis, float angle)
    {
        if ( !angle )
            return IDENTITY;
        return rotation_normalized( axis.normalized(), angle );
    }

    /**
     * Initializes a quaternion representing a rotation by a certain angle (in
     * radians) around an axis specified as a normalized unit vector.
     */
    static Quaternion rotation_normalized(const ml7::Vector3& unit_axis, float angle)
    {
        const float sn05 = ::sinf( angle * 0.5f );
        const float cs05 = ::cosf( angle * 0.5f );
        return {
            unit_axis.x * sn05,
            unit_axis.y * sn05,
            unit_axis.z * sn05,
            cs05,
        };
    }

    /**
     * Initializes a quaternion representing a "view" rotation for a left-handed
     * coordinate system using a camera "look" direction and an "up" direction.
     */
    static Quaternion look_lh(const Vector3& look, const Vector3& up)
    {
        return from_matrix3x3( ml7::Matrix3x3::look_lh( look, up ) );
    }

    /**
     * Initializes a quaternion representing a "view" rotation for a right-handed
     * coordinate system using a camera "look" direction and an "up" direction.
     */
    static Quaternion look_rh(const Vector3& look, const Vector3& up)
    {
        return from_matrix3x3( ml7::Matrix3x3::look_rh( look, up ) );
    }

    /**
     * Swap operation.
     */
    void swap(Quaternion& rhs);



    // #############################################################################
    // Attributes
    // #############################################################################
public:
    union
    {
        struct
        {
            /** The quaternion's first imaginary component. */
            float x;
            /** The quaternion's second imaginary component. */
            float y;
            /** The quaternion's third imaginary component. */
            float z;
            /** The quaternion's scalar/real part. */
            float w;
        }; // struct

        /** Array of all four components. */
        float data[4];
    }; // union



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Tells whether this quaternion is invertible (i.e., whether it is non-zero).
     */
    bool is_invertible() const { return (x*x + y*y + z*z + w*w) != 0.0f; }

    /**
     * Returns the magnitude of this quaternion.
     */
    float norm() const { return ::sqrtf( x*x + y*y + z*z + w*w ); }



    // #############################################################################
    // Transformations
    // #############################################################################
public:
    /**
     * Returns a copy of this quaternion normalized (if possible).
     */
    Quaternion normalized() const;

    /**
     * Returns the conjugate of this quaternion.
     */
    Quaternion conjugated() const { return { -x, -y, -z, w }; }

    /**
     * Returns a copy of this quaternion inverted (if possible).
     */
    Quaternion inverted() const;

    /**
     * Returns the dot/scalar product of this and a given quaternion q.
     */
    float dot(const Quaternion& q) const
    {
        return x*q.x + y*q.y + z*q.z + w*q.w;
    }

    /**
     * Returns the rotation matrix corresponding to this object quaternion.
     */
    Matrix3x3 to_matrix3x3() const;

    /**
     * Extracts the basis vectors that define the transformed coordinate system.
     */
    void to_axes(ml7::Vector3& x, ml7::Vector3& y, ml7::Vector3& z) const;

    /**
     * Tries to extract the rotation axis and the rotation angle (in the range
     * [0;pi]) this quaternion is composed of.
     */
    bool to_axis_angle(ml7::Vector3& axis, float& angle) const;

    /**
     * Assumes that this quaternion represents a "view" rotation for a left-handed
     * coordinate system and tries to extract the camera "look" direction and the
     * "up" direction.
     */
    bool is_look_lh(ml7::Vector3& look, ml7::Vector3& up) const;

    /**
     * Assumes that this quaternion represents a "view" rotation for a right-handed
     * coordinate system and tries to extract the camera "look" direction and the
     * "up" direction.
     */
    bool is_look_rh(ml7::Vector3& look, ml7::Vector3& up) const;

    /**
     * Returns a copy of the given vector transformed by this quaternion.
     */
    Vector3 transform(const Vector3& v) const;

    /**
     * Returns a copy of the given vector transformed by this quaternion inverted
     * (if possible).
     */
    Vector3 transform_inverted(const Vector3& v) const;



    // #############################################################################
    // Manipulations
    // #############################################################################
public:
    /**
     * Normalizes this quaternion (if possible).
     */
    Quaternion& normalize()
    {
        *this = normalized();
        return *this;
    }

    /**
     * Conjugates this quaternion.
     */
    Quaternion& conjugate()
    {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    /**
     * Inverts this quaternion (if possible).
     */
    Quaternion& invert()
    {
        *this = inverted();
        return *this;
    }



    // #############################################################################
    // Access Operators
    // #############################################################################
public:
    float operator[] (unsigned i) const { assert( i < 4 ); return data[ i ]; }
    float& operator[] (unsigned i) { assert( i < 4 ); return data[ i ]; }



    // #############################################################################
    // Arithmetic Operators
    // #############################################################################
public:
    /** Returns a copy of this quaternion unmodified. */
    constexpr Quaternion operator + () const { return *this; }
    /** Returns a copy of this quaternion with the signs of the components flipped. */
    constexpr Quaternion operator - () const { return Quaternion( -x, -y, -z, -w ); }

    /** Returns the (component-wise) sum of two quaternions. */
    constexpr Quaternion operator + (const Quaternion& v) const { return Quaternion( x + v.x, y + v.y, z + v.z, w + v.w ); }
    /** Returns the (component-wise) difference of two quaternions. */
    constexpr Quaternion operator - (const Quaternion& v) const { return Quaternion( x - v.x, y - v.y, z - v.z, w - v.w ); }

    /** Returns a copy of this quaternion "scaled" by the specified factor (scalar multiplication). */
    constexpr Quaternion operator * (float s) const { return Quaternion( x * s, y * s, z * s, w * s ); }
    /** Returns a copy of this quaternion inversely "scaled" by the specified factor (scalar division). */
    constexpr Quaternion operator / (float s) const { return Quaternion( x / s, y / s, z / s, w / s ); }

    /** Returns the quaternion product of two quaternions (quaternion multiplication). */
    constexpr Quaternion operator * (const Quaternion& q) const
    {
        return {
            w*q.x + x*q.w + y*q.z - z*q.y,
            w*q.y + y*q.w + z*q.x - x*q.z,
            w*q.z + z*q.w + x*q.y - y*q.x,
            w*q.w - x*q.x - y*q.y - z*q.z,
        };
    }

    /** Returns a copy of the given vector transformed by this quaternion. */
    Vector3 operator * (const Vector3& v) const { return transform( v ); }



    // #############################################################################
    // Arithmetic Assignment Operators
    // #############################################################################
public:
    /** Adds the given quaternion to this one, resulting in the (component-wise) quaternion sum. */
    constexpr Quaternion& operator += (const Quaternion& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
    /** Subtracts the given quaternion from this one, resulting in the (component-wise) quaternion difference. */
    constexpr Quaternion& operator -= (const Quaternion& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }

    /** "Scales" this quaternion by the specified factor (scalar multiplication). */
    constexpr Quaternion& operator *= (float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
    /** Inversely "scales" this quaternion by the specified factor (scalar division). */
    constexpr Quaternion& operator /= (float s) { x /= s; y /= s; z /= s; w /= s; return *this; }



    // #############################################################################
    // Comparison Operators
    // #############################################################################
public:
    bool operator == (const Quaternion& q) const { return (x == q.x) && (y == q.y) && (z == q.z) && (w == q.w); }
    bool operator != (const Quaternion& q) const { return (x != q.x) || (y != q.y) || (z != q.z) || (w != q.w); }

}; // class Quaternion



    // #############################################################################
    // Additional Operators
    // #############################################################################

    /** "Scales" a quaternion by the specified factor (scalar multiplication). */
    constexpr Quaternion operator * (float s, const Quaternion& q) { return q * s; }



} // namespace ml7

#endif // ML7_QUATERNION_H
