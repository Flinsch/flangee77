#ifndef ML7_MATRIX3X3_H
#define ML7_MATRIX3X3_H

#include "./Vector3.h"

#include <span>



namespace ml7 {



struct Matrix3x3
{


    static const Matrix3x3 ZERO;
    static const Matrix3x3 IDENTITY;



    // #############################################################################
    // Attributes
    // #############################################################################

    union
    {
        struct
        {
            // NOLINTBEGIN(*-use-default-member-init)
            /** Element in the 1st row and 1st column. */
            float _11;
            /** Element in the 1st row and 2nd column. */
            float _12;
            /** Element in the 1st row and 3rd column. */
            float _13;
            /** Element in the 2nd row and 1st column. */
            float _21;
            /** Element in the 2nd row and 2nd column. */
            float _22;
            /** Element in the 2nd row and 3rd column. */
            float _23;
            /** Element in the 3rd row and 1st column. */
            float _31;
            /** Element in the 3rd row and 2nd column. */
            float _32;
            /** Element in the 3rd row and 3rd column. */
            float _33;
            // NOLINTEND(*-use-default-member-init)
        }; // struct

        struct
        {
            /** Element in the 1st row and 1st column. */
            float a;
            /** Element in the 1st row and 2nd column. */
            float b;
            /** Element in the 1st row and 3rd column. */
            float c;
            /** Element in the 2nd row and 1st column. */
            float d;
            /** Element in the 2nd row and 2nd column. */
            float e;
            /** Element in the 2nd row and 3rd column. */
            float f;
            /** Element in the 3rd row and 1st column. */
            float g;
            /** Element in the 3rd row and 2nd column. */
            float h;
            /** Element in the 3rd row and 3rd column. */
            float i;
        };

        /** Array of the elements. */
        float data[9];

        /** 2D array of the elements. */
        float m[3][3];
    }; // union



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes an identity matrix representing a neutral
     * transformation.
     */
    constexpr Matrix3x3() noexcept
        : _11(1.0f), _12(0.0f), _13(0.0f)
        , _21(0.0f), _22(1.0f), _23(0.0f)
        , _31(0.0f), _32(0.0f), _33(1.0f)
    {
    }

    /**
     * Explicit constructor with parameters for each element.
     */
    constexpr Matrix3x3(
        float _11, float _12, float _13,
        float _21, float _22, float _23,
        float _31, float _32, float _33) noexcept
        : _11(_11), _12(_12), _13(_13)
        , _21(_21), _22(_22), _23(_23)
        , _31(_31), _32(_32), _33(_33)
    {
    }

    /**
     * Swap operation.
     */
    void swap(Matrix3x3& other) noexcept;



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Tells whether this matrix is invertible (i.e., whether its determinant is
     * non-zero).
     */
    bool is_invertible() const { return determinant() != 0.0f; }

    /**
     * Returns the determinant of the matrix.
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

    /**
     * Returns a copy of this matrix transposed.
     */
    Matrix3x3 transposed() const
    {
        return {
            _11, _21, _31,
            _12, _22, _32,
            _13, _23, _33,
        };
    }

    /**
     * Returns a copy of this matrix inverted (if possible).
     */
    Matrix3x3 inverted() const;

    /**
     * Returns the i-th (0-indexed) row vector of this matrix.
     */
    Vector3 get_row(unsigned i) const
    {
        assert(i < 3);
        return {
            m[i][0],
            m[i][1],
            m[i][2],
        };
    }

    /**
     * Returns the j-th (0-indexed) column vector of this matrix.
     */
    Vector3 get_column(unsigned j) const
    {
        assert(j < 3);
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
     * Tries to extract the rotation axis and the rotation angle (in the range
     * [0;pi]) this matrix is composed of.
     * This only works if the matrix actually consists of rotations only (no
     * scalings, shears, etc.).
     */
    bool to_axis_angle(ml7::Vector3& axis, float& angle) const;

    /**
     * Tries to extract the scaling vector, the rotation axis, and the rotation
     * angle (in the range [0;pi]) this matrix is composed of.
     * This only works if the matrix actually consists of rotations and (positive)
     * scalings in the "common" order (no shears, negative scalings, etc.).
     */
    bool decompose(ml7::Vector3& scaling, ml7::Vector3& axis, float& angle) const;

    /**
     * Assumes that this matrix is a view rotation matrix for a left-handed
     * coordinate system and tries to extract the camera "look" direction and the
     * "up" direction.
     */
    bool is_look_lh(ml7::Vector3& look, ml7::Vector3& up) const;

    /**
     * Assumes that this matrix is a view rotation matrix for a right-handed
     * coordinate system and tries to extract the camera "look" direction and the
     * "up" direction.
     */
    bool is_look_rh(ml7::Vector3& look, ml7::Vector3& up) const;

    /**
     * Returns a copy of the given (column) vector transformed by this matrix.
     */
    constexpr Vector3 transform(const Vector3& v) const
    {
        return {
            _11*v.x + _12*v.y + _13*v.z,
            _21*v.x + _22*v.y + _23*v.z,
            _31*v.x + _32*v.y + _33*v.z,
        };
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix
     * inverted (if possible).
     */
    Vector3 transform_inverted(const Vector3& v) const;



    // #############################################################################
    // Manipulations
    // #############################################################################

    /**
     * Transposes this matrix.
     */
    Matrix3x3& transpose()
    {
        std::swap(_12, _21);
        std::swap(_13, _31);
        std::swap(_23, _32);
        return *this;
    }

    /**
     * Inverts this matrix (if possible).
     */
    Matrix3x3& invert()
    {
        *this = inverted();
        return *this;
    }

    /**
     * Sets the i-th (0-indexed) row vector of this matrix.
     */
    Matrix3x3& set_row(unsigned i, Vector3 v)
    {
        assert(i < 3);
        m[i][0] = v[0];
        m[i][1] = v[1];
        m[i][2] = v[2];
        return *this;
    }

    /**
     * Sets the j-th (0-indexed) column vector of this matrix.
     */
    Matrix3x3& set_column(unsigned j, Vector3 v)
    {
        assert(j < 3);
        m[0][j] = v[0];
        m[1][j] = v[1];
        m[2][j] = v[2];
        return *this;
    }



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const Matrix3x3& m) const noexcept { for (unsigned k = 0; k < 9; ++k) if (data[k] != m.data[k]) return false; return true; }
    bool operator!=(const Matrix3x3& m) const noexcept { for (unsigned k = 0; k < 9; ++k) if (data[k] != m.data[k]) return true; return false; }



    // #############################################################################
    // Arithmetic Operators
    // #############################################################################

    /** Returns a copy of this matrix unmodified. */
    constexpr Matrix3x3 operator+() const { return *this; }
    /** Returns a copy of this matrix with the signs of the elements flipped. */
    constexpr Matrix3x3 operator-() const { return {-a, -b, -c, -d, -e, -f, -g, -h, -i}; }

    /** Returns the (element-wise) matrix sum of two matrices. */
    constexpr Matrix3x3 operator+(const Matrix3x3& m) const { return {a + m.a, b + m.b, c + m.c, d + m.d, e + m.e, f + m.f, g + m.g, h + m.h, i + m.i}; }
    /** Returns the (element-wise) matrix difference of two matrices. */
    constexpr Matrix3x3 operator-(const Matrix3x3& m) const { return {a - m.a, b - m.b, c - m.c, d - m.d, e - m.e, f - m.f, g - m.g, h - m.h, i - m.i}; }

    /** Returns a copy of this vector "scaled" by the specified factor (scalar multiplication). */
    constexpr Matrix3x3 operator*(float s) const { return {a * s, b * s, c * s, d * s, e * s, f * s, g * s, h * s, i * s}; }
    /** Returns a copy of this vector inversely "scaled" by the specified factor (scalar division). */
    constexpr Matrix3x3 operator/(float s) const { return {a / s, b / s, c / s, d / s, e / s, f / s, g / s, h / s, i / s}; }

    /** Returns the matrix product of two matrices (matrix multiplication). */
    constexpr Matrix3x3 operator*(const Matrix3x3& m) const
    {
        return {
            _11*m._11 + _12*m._21 + _13*m._31,   _11*m._12 + _12*m._22 + _13*m._32,   _11*m._13 + _12*m._23 + _13*m._33,
            _21*m._11 + _22*m._21 + _23*m._31,   _21*m._12 + _22*m._22 + _23*m._32,   _21*m._13 + _22*m._23 + _23*m._33,
            _31*m._11 + _32*m._21 + _33*m._31,   _31*m._12 + _32*m._22 + _33*m._32,   _31*m._13 + _32*m._23 + _33*m._33,
        };
    }

    /** Returns a copy of the given (column) vector transformed by this matrix. */
    constexpr Vector3 operator*(const Vector3& v) const { return transform(v); }

    /** "Scales" a matrix by the specified factor (scalar multiplication). */
    friend constexpr Matrix3x3 operator*(float s, const Matrix3x3& m) { return m * s; }



    // #############################################################################
    // Arithmetic Assignment Operators
    // #############################################################################

    /** Adds the given matrix to this one, resulting in the (element-wise) matrix sum. */
    constexpr Matrix3x3& operator+=(const Matrix3x3& m) { for (unsigned k = 0; k < 9; ++k) data[k] += m.data[k]; return *this; }
    /** Subtracts the given matrix from this one, resulting in the (element-wise) matrix difference. */
    constexpr Matrix3x3& operator-=(const Matrix3x3& m) { for (unsigned k = 0; k < 9; ++k) data[k] -= m.data[k]; return *this; }

    /** "Scales" this matrix by the specified factor (scalar multiplication). */
    constexpr Matrix3x3& operator*=(float s) { for (float& k : data) k *= s; return *this; }
    /** Inversely scales this matrix by the specified factor (scalar division). */
    constexpr Matrix3x3& operator/=(float s) { for (float& k : data) k /= s; return *this; }



    // #############################################################################
    // Access Operators
    // #############################################################################

    std::span<const float, 3> operator[](unsigned i) const { assert(i < 3); return std::span<const float, 3>{m[i], 3}; }
    std::span<float, 3> operator[](unsigned i) { assert(i < 3); return std::span<float, 3>{m[i], 3}; }



    // #############################################################################
    // Static Functions
    // #############################################################################

    /**
     * Initializes a transformation matrix from the specified basis vectors that
     * define the transformed coordinate system (as the column vectors of the matrix).
     */
    static constexpr Matrix3x3 from_axes(const ml7::Vector3& x, const ml7::Vector3& y, const ml7::Vector3& z)
    {
        return {
            x.x, y.x, z.x,
            x.y, y.y, z.y,
            x.z, y.z, z.z,
        };
    }

    /**
     * Initializes a scaling matrix with scaling factor s.
     */
    static constexpr Matrix3x3 scaling(float s)
    {
        return {
            s, 0.0f, 0.0f,
            0.0f, s, 0.0f,
            0.0f, 0.0f, s,
        };
    }

    /**
     * Initializes a scaling matrix with scaling vector s.
     */
    static constexpr Matrix3x3 scaling(const ml7::Vector3& s)
    {
        return {
            s.x, 0.0f, 0.0f,
            0.0f, s.y, 0.0f,
            0.0f, 0.0f, s.z,
        };
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around the x-axis.
     */
    static Matrix3x3 rotx(float angle) { return rotx(std::sin(angle), std::cos(angle)); }

    /**
     * Initializes a rotation matrix representing a rotation around the x-axis based
     * on ready-made sine and cosine values.
     */
    static Matrix3x3 rotx(float sin, float cos)
    {
        return {
            1.0f, 0.0f, 0.0f,
            0.0f,  cos, -sin,
            0.0f,  sin,  cos,
        };
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around the y-axis.
     */
    static Matrix3x3 roty(float angle) { return roty(std::sin(angle), std::cos(angle)); }

    /**
     * Initializes a rotation matrix representing a rotation around the y-axis based
     * on ready-made sine and cosine values.
     */
    static Matrix3x3 roty(float sin, float cos)
    {
        return {
             cos, 0.0f,  sin,
            0.0f, 1.0f, 0.0f,
            -sin, 0.0f,  cos,
        };
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around the z-axis.
     */
    static Matrix3x3 rotz(float angle) { return rotz(std::sin(angle), std::cos(angle)); }

    /**
     * Initializes a rotation matrix representing a rotation around the z-axis based
     * on ready-made sine and cosine values.
     */
    static Matrix3x3 rotz(float sin, float cos)
    {
        return {
             cos, -sin, 0.0f,
             sin,  cos, 0.0f,
            0.0f, 0.0f, 1.0f,
        };
    }

    /**
     * Initializes a rotation matrix representing a rotation around the specified
     * axis, with an angle equal to the magnitude of the specified vector (in
     * radians).
     */
    static Matrix3x3 rotation(const ml7::Vector3& axis_angle)
    {
        const float angle = axis_angle.length();
        if (angle == 0.0f)
            return IDENTITY;
        return rotation_normalized(axis_angle / angle, angle);
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around the specified axis.
     */
    static Matrix3x3 rotation(const ml7::Vector3& axis, float angle)
    {
        if (angle == 0.0f)
            return IDENTITY;
        return rotation_normalized(axis.normalized(), angle);
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around an axis specified as a normalized unit vector.
     */
    static Matrix3x3 rotation_normalized(const ml7::Vector3& unit_axis, float angle);

    /**
     * Initializes a transformation matrix from the specified scaling factor, a
     * rotation axis, and a rotation angle (in radians).
     */
    static Matrix3x3 compose(float scaling, const ml7::Vector3& axis, float angle)
    {
        return compose({scaling, scaling, scaling}, axis, angle);
    }

    /**
     * Initializes a transformation matrix from the specified scaling vector, a
     * rotation axis, and a rotation angle (in radians).
     */
    static Matrix3x3 compose(const ml7::Vector3& scaling, const ml7::Vector3& axis, float angle);

    /**
     * Initializes a view rotation matrix for a left-handed coordinate system using
     * a camera "look" direction and an "up" direction.
     */
    static Matrix3x3 look_lh(const Vector3& look, const Vector3& up);

    /**
     * Initializes a view rotation matrix for a right-handed coordinate system using
     * a camera "look" direction and an "up" direction.
     */
    static Matrix3x3 look_rh(const Vector3& look, const Vector3& up);

}; // struct Matrix3x3



} // namespace ml7

#endif // ML7_MATRIX3X3_H
