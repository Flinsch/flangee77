#ifndef ML7_MATRIX2X3_H
#define ML7_MATRIX2X3_H

#include "./Matrix2x2.h"
#include "./Vector2.h"



namespace ml7 {



struct Matrix2x3
{

    static const Matrix2x3 ZERO;
    static const Matrix2x3 IDENTITY;



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
            // NOLINTEND(*-use-default-member-init)
        }; // struct

        struct
        {
            /** Element in the 1st row and 1st column. */
            float a;
            /** Element in the 1st row and 2nd column. */
            float b;
            /** Element in the 1nd row and 3rd column. */
            float c;
            /** Element in the 2nd row and 1st column. */
            float d;
            /** Element in the 2nd row and 2nd column. */
            float e;
            /** Element in the 2nd row and 3rd column. */
            float f;
        };

        /** Array of the elements. */
        float data[6];

        /** 2D array of the elements. */
        float m[2][3];
    }; // union



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes an identity matrix representing a neutral
     * transformation.
     */
    constexpr Matrix2x3() noexcept
        : _11(1.0f), _12(0.0f), _13(0.0f)
        , _21(0.0f), _22(1.0f), _23(0.0f)
    {
    }

    /**
     * Explicit constructor with parameters for each element.
     */
    constexpr Matrix2x3(
        float _11, float _12, float _13,
        float _21, float _22, float _23) noexcept
        : _11(_11), _12(_12), _13(_13)
        , _21(_21), _22(_22), _23(_23)
    {
    }

    /**
     * Explicit constructor from a 2x2 matrix.
     */
    constexpr explicit Matrix2x3(const Matrix2x2& m2x2) noexcept
        : _11(m2x2._11), _12(m2x2._12), _13(0.0f)
        , _21(m2x2._21), _22(m2x2._22), _23(0.0f)
    {
    }

    /**
     * Explicit constructor from a 2x2 matrix and a translation vector.
     */
    constexpr Matrix2x3(const Matrix2x2& m2x2, const ml7::Vector2& translation) noexcept
        : _11(m2x2._11), _12(m2x2._12), _13(translation.x)
        , _21(m2x2._21), _22(m2x2._22), _23(translation.y)
    {
    }

    /**
     * Swap operation.
     */
    void swap(Matrix2x3& other) noexcept;



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Tells whether this matrix is invertible (i.e., whether the determinant of its
     * 2x2 part is non-zero).
     */
    bool is_invertible() const { return determinant() != 0.0f; }

    /**
     * Returns the determinant of the 2x2 part of the matrix.
     */
    float determinant() const { return _11*_22 - _12*_21; }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this matrix inverted (if possible).
     */
    Matrix2x3 inverted() const;

    /**
     * Returns the j-th (0-indexed) column vector of this matrix.
     */
    Vector2 get_column(unsigned j) const
    {
        assert(j < 3);
        return {
            m[0][j],
            m[1][j],
        };
    }

    /**
     * Extracts the basis vectors that define the transformed coordinate system
     * (i.e., the column vectors of the matrix).
     */
    void to_axes(ml7::Vector2& x, ml7::Vector2& y) const
    {
        x.x = _11;   y.x = _12;
        x.y = _21;   y.y = _22;
    }

    /**
     * Extracts the basis vectors and origin/translation vector that define the
     * transformed coordinate system (i.e., the column vectors of the matrix).
     */
    void to_axes(ml7::Vector2& x, ml7::Vector2& y, ml7::Vector2& origin) const
    {
        x.x = _11;   y.x = _12;   origin.x = _13;
        x.y = _21;   y.y = _22;   origin.y = _23;
    }

    /**
     * Returns the 2x2 part of this matrix.
     */
    Matrix2x2 to_matrix2x2() const
    {
        return {
            _11, _12,
            _21, _22,
        };
    }

    /**
     * Decomposes this matrix into a 2x2 matrix and a translation vector.
     */
    void decompose(ml7::Matrix2x2& m2x2, ml7::Vector2& translation) const
    {
        m2x2._11 = _11;   m2x2._12 = _12;   translation.x = _13;
        m2x2._21 = _21;   m2x2._22 = _22;   translation.y = _23;
    }

    /**
     * Tries to extract the scaling vector, the counter-clockwise rotation angle (in
     * the range [-pi;+pi]), and the translation vector this matrix is composed of.
     * This only works if the matrix actually consists of translations, rotations and
     * (positive) scalings in the "common" order (no shears, negative scalings, etc.).
     */
    bool decompose(ml7::Vector2& scaling, float& angle, ml7::Vector2& translation) const;

    /**
     * Returns a copy of the given (column) vector transformed by this matrix.
     * Used to transform position vectors rather than direction vectors.
     */
    constexpr Vector2 transform(const Vector2& v) const
    {
        return {
            _11*v.x + _12*v.y + _13,
            _21*v.x + _22*v.y + _23,
        };
    }

    /**
     * Returns a copy of the given (column) vector transformed by the 2x2 part of
     * this matrix.
     * Used to transform direction vectors rather than position vectors.
     */
    constexpr Vector2 transform2x2(const Vector2& v) const
    {
        return {
            _11*v.x + _12*v.y,
            _21*v.x + _22*v.y,
        };
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix
     * inverted (if possible).
     * Used to transform position vectors rather than direction vectors.
     */
    Vector2 transform_inverted(const Vector2& v) const;

    /**
     * Returns a copy of the given (column) vector transformed by the 2x2 part of
     * this matrix inverted (if possible).
     * Used to transform direction vectors rather than position vectors.
     */
    Vector2 transform2x2_inverted(const Vector2& v) const;



    // #############################################################################
    // Manipulations
    // #############################################################################

    /**
     * Inverts this matrix (if possible).
     */
    Matrix2x3& invert()
    {
        *this = inverted();
        return *this;
    }

    /**
     * Sets the j-th (0-indexed) column vector of this matrix.
     */
    Matrix2x3& set_column(unsigned j, Vector2 v)
    {
        assert(j < 3);
        m[0][j] = v[0];
        m[1][j] = v[1];
        return *this;
    }



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const Matrix2x3& m) const { for (unsigned k = 0; k < 6; ++k) if (data[k] != m.data[k]) return false; return true; }
    bool operator!=(const Matrix2x3& m) const { for (unsigned k = 0; k < 6; ++k) if (data[k] != m.data[k]) return true; return false; }



    // #############################################################################
    // Arithmetic Operators
    // #############################################################################

    /** Returns a copy of this matrix unmodified. */
    constexpr Matrix2x3 operator+() const { return *this; }
    /** Returns a copy of this matrix with the signs of the elements flipped. */
    constexpr Matrix2x3 operator-() const { return {-a, -b, -c, -d, -e, -f}; }

    /** Returns the (element-wise) matrix sum of two matrices. */
    constexpr Matrix2x3 operator+(const Matrix2x3& m) const { return {a + m.a, b + m.b, c + m.c, d + m.d, e + m.e, f + m.f}; }
    /** Returns the (element-wise) matrix difference of two matrices. */
    constexpr Matrix2x3 operator-(const Matrix2x3& m) const { return {a - m.a, b - m.b, c - m.c, d - m.d, e - m.e, f - m.f}; }

    /** Returns a copy of this matrix "scaled" by the specified factor (scalar multiplication). */
    constexpr Matrix2x3 operator*(float s) const { return {a * s, b * s, c * s, d * s, e * s, f * s}; }
    /** Returns a copy of this matrix inversely "scaled" by the specified factor (scalar division). */
    constexpr Matrix2x3 operator/(float s) const { return {a / s, b / s, c / s, d / s, e / s, f / s}; }

    /** Returns the matrix product of two matrices (matrix multiplication). */
    constexpr Matrix2x3 operator*(const Matrix2x3& m) const
    {
        return {
            _11*m._11 + _12*m._21,   _11*m._12 + _12*m._22,   _11*m._13 + _12*m._23 + _13,
            _21*m._11 + _22*m._21,   _21*m._12 + _22*m._22,   _21*m._13 + _22*m._23 + _23,
        };
    }

    /** Returns a copy of the given (column vector) transformed by this matrix. */
    constexpr Vector2 operator*(const Vector2& v) const { return transform(v); }



    // #############################################################################
    // Arithmetic Assignment Operators
    // #############################################################################

    /** Adds the given matrix to this one, resulting in the (element-wise) matrix sum. */
    constexpr Matrix2x3& operator+=(const Matrix2x3& m) { for (unsigned k = 0; k < 6; ++k) data[k] += m.data[k]; return *this; }
    /** Subtracts the given matrix from this one, resulting in the (element-wise) matrix difference. */
    constexpr Matrix2x3& operator-=(const Matrix2x3& m) { for (unsigned k = 0; k < 6; ++k) data[k] -= m.data[k]; return *this; }

    /** "Scales" this matrix by the specified factor (scalar multiplication). */
    constexpr Matrix2x3& operator*=(float s) { for (float& k : data) k *= s; return *this; }
    /** Inversely "scales" this matrix by the specified factor (scalar division). */
    constexpr Matrix2x3& operator/=(float s) { for (float& k : data) k /= s; return *this; }



    // #############################################################################
    // Access Operators
    // #############################################################################

    const float* operator[](unsigned i) const { assert(i < 2); return m[i]; }
    float* operator[](unsigned i) { assert(i < 2); return m[i]; }



    // #############################################################################
    // Static Functions
    // #############################################################################

    /**
     * Initializes a transformation matrix from the specified basis vectors that
     * define the transformed coordinate system (as the column vectors of the matrix).
     */
    static constexpr Matrix2x3 from_axes(const ml7::Vector2& x, const ml7::Vector2& y)
    {
        return {
            x.x, y.x, 0.0f,
            x.y, y.y, 0.0f,
        };
    }

    /**
     * Initializes a transformation matrix from the specified basis vectors and
     * origin/translation vector that define the transformed coordinate system (as
     * the column vectors of the matrix).
     */
    static constexpr Matrix2x3 from_axes(const ml7::Vector2& x, const ml7::Vector2& y, const ml7::Vector2& origin)
    {
        return {
            x.x, y.x, origin.x,
            x.y, y.y, origin.y,
        };
    }

    /**
     * Initializes a scaling matrix with scaling factor s.
     */
    static constexpr Matrix2x3 scaling(float s)
    {
        return {
            s, 0.0f, 0.0f,
            0.0f, s, 0.0f,
        };
    }

    /**
     * Initializes a scaling matrix with scaling vector s.
     */
    static constexpr Matrix2x3 scaling(const ml7::Vector2& s)
    {
        return {
            s.x, 0.0f, 0.0f,
            0.0f, s.y, 0.0f,
        };
    }

    /**
     * Initializes a rotation matrix representing a counter-clockwise rotation by
     * a certain angle (in radians).
     */
    static Matrix2x3 rotation(float angle)
    {
        const float cs = ::cosf(angle);
        const float sn = ::sinf(angle);
        return {
            cs, -sn, 0.0f,
            sn, cs, 0.0f,
        };
    }

    /**
     * Initializes a translation matrix from the specified translation vector.
     */
    static Matrix2x3 translation(const ml7::Vector2& translation)
    {
        return {
            1.0f, 0.0f, translation.x,
            0.0f, 1.0f, translation.y,
        };
    }

    /**
     * Initializes a transformation matrix from the specified scaling factor, a
     * counter-clockwise rotation angle (in radians), and a translation vector.
     */
    static Matrix2x3 compose(float scaling, float angle, const ml7::Vector2& translation)
    {
        return compose({scaling, scaling}, angle, translation);
    }

    /**
     * Initializes a transformation matrix from the specified scaling vector, a
     * counter-clockwise rotation angle (in radians), and a translation vector.
     */
    static Matrix2x3 compose(const ml7::Vector2& scaling, float angle, const ml7::Vector2& translation);

}; // struct Matrix2x3



    // #############################################################################
    // Additional Operators
    // #############################################################################

    /** "Scales" a matrix by the specified factor (scalar multiplication). */
    constexpr Matrix2x3 operator*(float s, const Matrix2x3& m) { return m * s; }



} // namespace ml7

#endif // ML7_MATRIX2X3_H
