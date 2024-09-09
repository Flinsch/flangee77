#pragma once
#ifndef ML7_MATRIX3X4_H
#define ML7_MATRIX3X4_H

#include "./Matrix3x3.h"



namespace ml7 {



class Matrix3x4
{

public:
    static const Matrix3x4 ZERO;
    static const Matrix3x4 IDENTITY;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default constructor. Initializes an identity matrix representing a neutral
     * transformation.
     */
    constexpr Matrix3x4()
        : _11( 1.0f ), _12( 0.0f ), _13( 0.0f ), _14( 0.0f )
        , _21( 0.0f ), _22( 1.0f ), _23( 0.0f ), _24( 0.0f )
        , _31( 0.0f ), _32( 0.0f ), _33( 1.0f ), _34( 0.0f )
    {
    }

    /**
     * Explicit constructor with parameters for each element.
     */
    constexpr Matrix3x4(
        float _11, float _12, float _13, float _14,
        float _21, float _22, float _23, float _24,
        float _31, float _32, float _33, float _34)
        : _11( _11 ), _12( _12 ), _13( _13 ), _14( _14 )
        , _21( _21 ), _22( _22 ), _23( _23 ), _24( _24 )
        , _31( _31 ), _32( _32 ), _33( _33 ), _34( _34 )
    {
    }

    /**
     * Explicit constructor from a 3x3 matrix.
     */
    constexpr explicit Matrix3x4(const Matrix3x3& m3x3)
        : _11( m3x3._11 ), _12( m3x3._12 ), _13( m3x3._13 ), _14( 0.0f )
        , _21( m3x3._21 ), _22( m3x3._22 ), _23( m3x3._23 ), _24( 0.0f )
        , _31( m3x3._31 ), _32( m3x3._32 ), _33( m3x3._33 ), _34( 0.0f )
    {
    }

    /**
     * Explicit constructor from a 3x3 matrix and a translation vector.
     */
    constexpr Matrix3x4(const Matrix3x3& m3x3, const ml7::Vector3& translation)
        : _11( m3x3._11 ), _12( m3x3._12 ), _13( m3x3._13 ), _14( translation.x )
        , _21( m3x3._21 ), _22( m3x3._22 ), _23( m3x3._23 ), _24( translation.y )
        , _31( m3x3._31 ), _32( m3x3._32 ), _33( m3x3._33 ), _34( translation.z )
    {
    }

    /**
     * Initializes a transformation matrix from the specified basis vectors that
     * define the transformed coordinate system (as the column vectors of the matrix).
     */
    static constexpr Matrix3x4 from_axes(const ml7::Vector3& x, const ml7::Vector3& y, const ml7::Vector3& z)
    {
        return {
            x.x, y.x, z.x, 0.0f,
            x.y, y.y, z.y, 0.0f,
            x.z, y.z, z.z, 0.0f,
        };
    }

    /**
     * Initializes a transformation matrix from the specified basis vectors and
     * origin/translation vector that define the transformed coordinate system (as
     * the column vectors of the matrix).
     */
    static constexpr Matrix3x4 from_axes(const ml7::Vector3& x, const ml7::Vector3& y, const ml7::Vector3& z, const ml7::Vector3& origin)
    {
        return {
            x.x, y.x, z.x, origin.x,
            x.y, y.y, z.y, origin.y,
            x.z, y.z, z.z, origin.z,
        };
    }

    /**
     * Initializes a scaling matrix with scaling factor s.
     */
    static constexpr Matrix3x4 scaling(float s)
    {
        return {
            s, 0.0f, 0.0f, 0.0f,
            0.0f, s, 0.0f, 0.0f,
            0.0f, 0.0f, s, 0.0f,
        };
    }

    /**
     * Initializes a scaling matrix with scaling vector s.
     */
    static constexpr Matrix3x4 scaling(const ml7::Vector3& s)
    {
        return {
            s.x, 0.0f, 0.0f, 0.0f,
            0.0f, s.y, 0.0f, 0.0f,
            0.0f, 0.0f, s.z, 0.0f,
        };
    }

    /**
     * Initializes a rotation matrix representing a clockwise rotation by angle theta
     * (in radians) around the x-axis.
     */
    static Matrix3x4 rotx(float theta) { return rotx( ::sinf( theta ), ::cosf( theta ) ); }

    /**
     * Initializes a rotation matrix representing a clockwise rotation around the
     * x-axis based on ready-made sine and cosine values.
     */
    static Matrix3x4 rotx(float sin, float cos)
    {
        return {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f,  cos, -sin, 0.0f,
            0.0f,  sin,  cos, 0.0f,
        };
    }

    /**
     * Initializes a rotation matrix representing a clockwise rotation by angle theta
     * (in radians) around the y-axis.
     */
    static Matrix3x4 roty(float theta) { return roty( ::sinf( theta ), ::cosf( theta ) ); }

    /**
     * Initializes a rotation matrix representing a clockwise rotation around the
     * y-axis based on ready-made sine and cosine values.
     */
    static Matrix3x4 roty(float sin, float cos)
    {
        return {
             cos, 0.0f,  sin, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -sin, 0.0f,  cos, 0.0f,
        };
    }

    /**
     * Initializes a rotation matrix representing a clockwise rotation by angle theta
     * (in radians) around the z-axis.
     */
    static Matrix3x4 rotz(float theta) { return rotz( ::sinf( theta ), ::cosf( theta ) ); }

    /**
     * Initializes a rotation matrix representing a clockwise rotation around the
     * z-axis based on ready-made sine and cosine values.
     */
    static Matrix3x4 rotz(float sin, float cos)
    {
        return {
             cos, -sin, 0.0f, 0.0f,
             sin,  cos, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
        };
    }

    /**
     * Initializes a rotation matrix representing a clockwise rotation around the
     * specified axis, with an angle equal to the magnitude of the specified vector
     * (in radians).
     */
    static Matrix3x4 rotation(const ml7::Vector3& v)
    {
        const float theta = v.length();
        if ( !theta )
            return IDENTITY;
        return rotation_normalized( v / theta, theta );
    }

    /**
     * Initializes a rotation matrix representing a clockwise rotation by angle theta
     * (in radians) around the specified axis.
     */
    static Matrix3x4 rotation(const ml7::Vector3& v, float theta)
    {
        if ( !theta )
            return IDENTITY;
        return rotation_normalized( v.normalized(), theta );
    }

    /**
     * Initializes a rotation matrix representing a clockwise rotation by angle theta
     * (in radians) around an axis specified as a normalized unit vector.
     */
    static Matrix3x4 rotation_normalized(const ml7::Vector3& u, float theta);

    /**
     * Initializes a translation matrix from the specified translation vector.
     */
    static Matrix3x4 translation(const ml7::Vector3& translation)
    {
        return {
            1.0f, 0.0f, 0.0f, translation.x,
            0.0f, 1.0f, 0.0f, translation.y,
            0.0f, 0.0f, 1.0f, translation.z,
        };
    }

    /**
     * Initializes a transformation matrix from the specified scaling factor, a
     * clockwise rotation angle theta (in radians) around the specified axis, and a
     * translation vector.
     */
    static Matrix3x4 compose(float scaling, const ml7::Vector3& axis, float theta, const ml7::Vector3& translation)
    {
        return compose( { scaling, scaling, scaling }, axis, theta, translation );
    }

    /**
     * Initializes a transformation matrix from the specified scaling vector, a
     * clockwise rotation angle theta (in radians) around the specified axis, and a
     * translation vector.
     */
    static Matrix3x4 compose(const ml7::Vector3& scaling, const ml7::Vector3& axis, float theta, const ml7::Vector3& translation);


    /**
     * Swap operation.
     */
    void swap(Matrix3x4& rhs);



    // #############################################################################
    // Attributes
    // #############################################################################
public:
    union
    {
        struct
        {
            /** Element in the 1st row and 1st column. */
            float _11;
            /** Element in the 1st row and 2nd column. */
            float _12;
            /** Element in the 1st row and 3rd column. */
            float _13;
            /** Element in the 1st row and 4th column. */
            float _14;
            /** Element in the 2nd row and 1st column. */
            float _21;
            /** Element in the 2nd row and 2nd column. */
            float _22;
            /** Element in the 2nd row and 3rd column. */
            float _23;
            /** Element in the 2nd row and 4th column. */
            float _24;
            /** Element in the 3rd row and 1st column. */
            float _31;
            /** Element in the 3rd row and 2nd column. */
            float _32;
            /** Element in the 3rd row and 3rd column. */
            float _33;
            /** Element in the 3rd row and 4th column. */
            float _34;
        }; // struct

        /** Array of the elements. */
        float data[12];

        /** 2D array of the elements. */
        float m[3][4];
    }; // union



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the determinant of the 3x3 part of the matrix.
     */
    float determinant() const
    {
        return
            + _11*_22*_33 + _12*_23*_31 + _13*_21*_32
            - _31*_22*_13 - _32*_23*_11 - _33*_21*_12;
    }



    // #############################################################################
    // Transformations
    // #############################################################################
public:
    /**
     * Returns a copy of this matrix inverted (if possible).
     */
    Matrix3x4 inverted() const;

    /**
     * Returns the j-th (0-indexed) column vector of this matrix.
     */
    Vector3 get_column(unsigned j) const
    {
        assert( j < 4 );
        return {
            m[0][j],
            m[1][j],
            m[2][j],
        };
    }

    /**
     * Extracts the basis vectors that define the transformed coordinate system
     * (i.e., the column vectors of the matrix).
     */
    void to_axes(ml7::Vector3& x, ml7::Vector3& y, ml7::Vector3& z) const
    {
        x.x = _11;   y.x = _12;   z.x = _13;
        x.y = _21;   y.y = _22;   z.y = _23;
        x.z = _31;   y.z = _32;   z.z = _33;
    }

    /**
     * Extracts the basis vectors and origin/translation vector that define the
     * transformed coordinate system (i.e., the column vectors of the matrix).
     */
    void to_axes(ml7::Vector3& x, ml7::Vector3& y, ml7::Vector3& z, ml7::Vector3& origin) const
    {
        x.x = _11;   y.x = _12;   z.x = _13;   origin.x = _14;
        x.y = _21;   y.y = _22;   z.y = _23;   origin.y = _24;
        x.z = _31;   y.z = _32;   z.z = _33;   origin.z = _34;
    }

    /**
     * Tries to extract the rotation axis and the clockwise rotation angle theta (in
     * the range [0;pi]) the 3x3 part of this matrix is composed of.
     * This only works if the 3x3 part actually consists of rotations only (no
     * scalings, shears, etc.).
     */
    bool to_axis_angle(ml7::Vector3& axis, float& theta) const;

    /**
     * Returns the 3x3 part of this matrix.
     */
    Matrix3x3 to_matrix3x3() const
    {
        return Matrix3x3{
            _11, _12, _13,
            _21, _22, _23,
            _31, _32, _33,
        };
    }

    /**
     * Decomposes this matrix into a 3x3 matrix and a translation vector
     */
    void decompose(ml7::Matrix3x3& m3x3, ml7::Vector3& translation) const
    {
        m3x3._11 = _11;   m3x3._12 = _12;   m3x3._13 = _13;   translation.x = _14;
        m3x3._21 = _21;   m3x3._22 = _22;   m3x3._23 = _23;   translation.y = _24;
        m3x3._31 = _31;   m3x3._32 = _32;   m3x3._33 = _33;   translation.z = _34;
    }

    /**
     * Tries to extract the scaling vector, the rotation axis, the clockwise rotation
     * angle theta (in the range [0;pi]), and the translation vector this matrix is
     * composed of.
     * This only works if the matrix actually consists of translations, rotations and
     * (positive) scalings in the "common" order (no shears, negative scalings, etc.).
     */
    bool decompose(ml7::Vector3& scaling, ml7::Vector3& axis, float& theta, ml7::Vector3& translation) const;

    /**
     * Returns a copy of the given (column) vector transformed by this matrix.
     * Used to transform position vectors rather than direction vectors.
     */
    constexpr Vector3 transform(const Vector3& v) const
    {
        return {
            _11*v.x + _12*v.y + _13*v.z + _14,
            _21*v.x + _22*v.y + _23*v.z + _24,
            _31*v.x + _32*v.y + _33*v.z + _34,
        };
    }

    /**
     * Returns a copy of the given (column) vector transformed by the 3x3 part of
     * this matrix.
     * Used to transform direction vectors rather than position vectors.
     */
    constexpr Vector3 transform3x3(const Vector3& v) const
    {
        return {
            _11*v.x + _12*v.y + _13*v.z,
            _21*v.x + _22*v.y + _23*v.z,
            _31*v.x + _32*v.y + _33*v.z,
        };
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix.
     * Used to transform position vectors rather than direction vectors.
     */
    Vector3 transform_inverted(const Vector3& v) const;

    /**
     * Returns a copy of the given (column) vector transformed by the 3x3 part of
     * this matrix.
     * Used to transform direction vectors rather than position vectors.
     */
    Vector3 transform3x3_inverted(const Vector3& v) const;



    // #############################################################################
    // Manipulations
    // #############################################################################
public:
    /**
     * Inverts this matrix (if possible).
     */
    Matrix3x4& invert()
    {
        *this = inverted();
        return *this;
    }

    /**
     * Sets the j-th (0-indexed) column vector of this matrix.
     */
    Matrix3x4& set_column(unsigned j, Vector3 v)
    {
        assert( j < 4 );
        m[0][j] = v[0];
        m[1][j] = v[1];
        m[2][j] = v[2];
        return *this;
    }



    // #############################################################################
    // Access Operators
    // #############################################################################
public:
    const float* operator[] (unsigned i) const { assert( i < 3 ); return m[ i ]; }
    float* operator[] (unsigned i) { assert( i < 3 ); return m[ i ]; }



    // #############################################################################
    // Arithmetic Operators
    // #############################################################################
public:
    /** Returns a copy of this matrix unmodified. */
    constexpr Matrix3x4 operator + () const { return *this; }
    /** Returns a copy of this matrix with the signs of the elements flipped. */
    constexpr Matrix3x4 operator - () const { return Matrix3x4( -_11, -_12, -_13, -_14, -_21, -_22, -_23, -_24, -_31, -_32, -_33, -_34 ); }

    /** Returns the (element-wise) matrix sum of two matrices. */
    constexpr Matrix3x4 operator + (const Matrix3x4& m) const { return Matrix3x4( _11 + m._11, _12 + m._12, _13 + m._13, _14 + m._14, _21 + m._21, _22 + m._22, _23 + m._23, _24 + m._24, _31 + m._31, _32 + m._32, _33 + m._33, _34 + m._34 ); }
    /** Returns the (element-wise) matrix difference of two matrices. */
    constexpr Matrix3x4 operator - (const Matrix3x4& m) const { return Matrix3x4( _11 - m._11, _12 - m._12, _13 - m._13, _14 - m._14, _21 - m._21, _22 - m._22, _23 - m._23, _24 - m._24, _31 - m._31, _32 - m._32, _33 - m._33, _34 - m._34 ); }

    /** Returns a copy of this vector "scaled" by the specified factor (scalar multiplication). */
    constexpr Matrix3x4 operator * (float s) const { return Matrix3x4( _11 * s, _12 * s, _13 * s, _14 * s, _21 * s, _22 * s, _23 * s, _24 * s, _31 * s, _32 * s, _33 * s, _34 * s ); }
    /** Returns a copy of this vector inversely "scaled" by the specified factor (scalar division). */
    constexpr Matrix3x4 operator / (float s) const { return Matrix3x4( _11 / s, _12 / s, _13 / s, _14 / s, _21 / s, _22 / s, _23 / s, _24 / s, _31 / s, _32 / s, _33 / s, _34 / s ); }

    /** Returns the matrix product of two matrices (matrix multiplication). */
    constexpr Matrix3x4 operator * (const Matrix3x4& m) const
    {
        return Matrix3x4(
            _11*m._11 + _12*m._21 + _13*m._31,   _11*m._12 + _12*m._22 + _13*m._32,   _11*m._13 + _12*m._23 + _13*m._33,   _11*m._14 + _12*m._24 + _13*m._34 + _14,
            _21*m._11 + _22*m._21 + _23*m._31,   _21*m._12 + _22*m._22 + _23*m._32,   _21*m._13 + _22*m._23 + _23*m._33,   _21*m._14 + _22*m._24 + _23*m._34 + _24,
            _31*m._11 + _32*m._21 + _33*m._31,   _31*m._12 + _32*m._22 + _33*m._32,   _31*m._13 + _32*m._23 + _33*m._33,   _31*m._14 + _32*m._24 + _33*m._34 + _34 );
    }

    /** Returns a copy of the given (column vector) transformed by this matrix. */
    constexpr Vector3 operator * (const Vector3& v) const { return transform( v ); }



    // #############################################################################
    // Arithmetic Assignment Operators
    // #############################################################################
public:
    /** Adds the given matrix to this one, resulting in the (element-wise) matrix sum. */
    constexpr Matrix3x4& operator += (const Matrix3x4& m) { for ( unsigned k = 0; k < 12; ++k ) data[k] += m.data[k]; return *this; }
    /** Subtracts the given matrix from this one, resulting in the (element-wise) matrix difference. */
    constexpr Matrix3x4& operator -= (const Matrix3x4& m) { for ( unsigned k = 0; k < 12; ++k ) data[k] -= m.data[k]; return *this; }

    /** "Scales" this matrix by the specified factor (scalar multiplication). */
    constexpr Matrix3x4& operator *= (float s) { for ( unsigned k = 0; k < 12; ++k ) data[k] *= s; return *this; }
    /** Inversely scales this vector by the specified factor (scalar division). */
    constexpr Matrix3x4& operator /= (float s) { for ( unsigned k = 0; k < 12; ++k ) data[k] /= s; return *this; }



    // #############################################################################
    // Comparison Operators
    // #############################################################################
public:
    bool operator == (const Matrix3x4& m) const { for ( unsigned k = 0; k < 12; ++k ) if ( data[k] != m.data[k] ) return false; return true; }
    bool operator != (const Matrix3x4& m) const { for ( unsigned k = 0; k < 12; ++k ) if ( data[k] != m.data[k] ) return true; return false; }

}; // class Matrix3x4



    // #############################################################################
    // Additional Operators
    // #############################################################################

    /** "Scales" a matrix by the specified factor (scalar multiplication). */
    constexpr Matrix3x4 operator * (float s, const Matrix3x4& m) { return m * s; }



} // namespace ml7

#endif // ML7_MATRIX3X4_H
