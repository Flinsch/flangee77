#ifndef ML7_MATRIX2X2_H
#define ML7_MATRIX2X2_H

#include "./Vector2.h"

#include <span>



namespace ml7 {



struct Matrix2x2
{

    static const Matrix2x2 ZERO;
    static const Matrix2x2 IDENTITY;



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
            /** Element in the 2nd row and 1st column. */
            float _21;
            /** Element in the 2nd row and 2nd column. */
            float _22;
            // NOLINTEND(*-use-default-member-init)
        }; // struct

        struct
        {
            /** Element in the 1st row and 1st column. */
            float a;
            /** Element in the 1st row and 2nd column. */
            float b;
            /** Element in the 2nd row and 1st column. */
            float c;
            /** Element in the 2nd row and 2nd column. */
            float d;
        };

        /** Array of the elements. */
        float data[4];

        /** 2D array of the elements. */
        float m[2][2];
    }; // union



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes an identity matrix representing a neutral
     * transformation.
     */
    constexpr Matrix2x2() noexcept
        : _11(1.0f), _12(0.0f)
        , _21(0.0f), _22(1.0f)
    {
    }

    /**
     * Explicit constructor with parameters for each element.
     */
    constexpr Matrix2x2(
        float _11, float _12,
        float _21, float _22) noexcept
        : _11(_11), _12(_12)
        , _21(_21), _22(_22)
    {
    }

    /**
     * Swap operation.
     */
    void swap(Matrix2x2& other) noexcept;



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
    float determinant() const { return _11*_22 - _12*_21; }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this matrix transposed.
     */
    Matrix2x2 transposed() const
    {
        return {
            _11, _21,
            _12, _22,
        };
    }

    /**
     * Returns a copy of this matrix inverted (if possible).
     */
    Matrix2x2 inverted() const;

    /**
     * Returns the i-th (0-indexed) row vector of this matrix.
     */
    Vector2 get_row(unsigned i) const
    {
        assert(i < 2);
        return {
            m[i][0],
            m[i][1],
        };
    }

    /**
     * Returns the j-th (0-indexed) column vector of this matrix.
     */
    Vector2 get_column(unsigned j) const
    {
        assert(j < 2);
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
     * Tries to extract the scaling vector and the counter-clockwise rotation angle
     * (in the range [-pi;+pi]) this matrix is composed of.
     * This only works if the matrix actually consists of rotations and (positive)
     * scalings in the "common" order (no shears, negative scalings, etc.).
     */
    bool decompose(ml7::Vector2& scaling, float& angle) const;

    /**
     * Returns a copy of the given (column) vector transformed by this matrix.
     */
    constexpr Vector2 transform(const Vector2& v) const
    {
        return {
            _11*v.x + _12*v.y,
            _21*v.x + _22*v.y,
        };
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix
     * inverted (if possible).
     */
    Vector2 transform_inverted(const Vector2& v) const;



    // #############################################################################
    // Manipulations
    // #############################################################################

    /**
     * Transposes this matrix.
     */
    Matrix2x2& transpose()
    {
        std::swap(_12, _21);
        return *this;
    }

    /**
     * Inverts this matrix (if possible).
     */
    Matrix2x2& invert()
    {
        *this = inverted();
        return *this;
    }

    /**
     * Sets the i-th (0-indexed) row vector of this matrix.
     */
    Matrix2x2& set_row(unsigned i, Vector2 v)
    {
        assert(i < 2);
        m[i][0] = v[0];
        m[i][1] = v[1];
        return *this;
    }

    /**
     * Sets the j-th (0-indexed) column vector of this matrix.
     */
    Matrix2x2& set_column(unsigned j, Vector2 v)
    {
        assert(j < 2);
        m[0][j] = v[0];
        m[1][j] = v[1];
        return *this;
    }



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const Matrix2x2& m) const { for (unsigned k = 0; k < 4; ++k) if (data[k] != m.data[k]) return false; return true; }
    bool operator!=(const Matrix2x2& m) const { for (unsigned k = 0; k < 4; ++k) if (data[k] != m.data[k]) return true; return false; }



    // #############################################################################
    // Arithmetic Operators
    // #############################################################################

    /** Returns a copy of this matrix unmodified. */
    constexpr Matrix2x2 operator+() const { return *this; }
    /** Returns a copy of this matrix with the signs of the elements flipped. */
    constexpr Matrix2x2 operator-() const { return {-a, -b, -c, -d}; }

    /** Returns the (element-wise) matrix sum of two matrices. */
    constexpr Matrix2x2 operator+(const Matrix2x2& m) const { return {a + m.a, b + m.b, c + m.c, d + m.d}; }
    /** Returns the (element-wise) matrix difference of two matrices. */
    constexpr Matrix2x2 operator-(const Matrix2x2& m) const { return {a - m.a, b - m.b, c - m.c, d - m.d}; }

    /** Returns a copy of this matrix "scaled" by the specified factor (scalar multiplication). */
    constexpr Matrix2x2 operator*(float s) const { return {a * s, b * s, c * s, d * s}; }
    /** Returns a copy of this matrix inversely "scaled" by the specified factor (scalar division). */
    constexpr Matrix2x2 operator/(float s) const { return {a / s, b / s, c / s, d / s}; }

    /** Returns the matrix product of two matrices (matrix multiplication). */
    constexpr Matrix2x2 operator*(const Matrix2x2& m) const
    {
        return {
            _11*m._11 + _12*m._21,   _11*m._12 + _12*m._22,
            _21*m._11 + _22*m._21,   _21*m._12 + _22*m._22,
        };
    }

    /** Returns a copy of the given (column) vector transformed by this matrix. */
    constexpr Vector2 operator*(const Vector2& v) const { return transform(v); }



    // #############################################################################
    // Arithmetic Assignment Operators
    // #############################################################################

    /** Adds the given matrix to this one, resulting in the (element-wise) matrix sum. */
    constexpr Matrix2x2& operator+=(const Matrix2x2& m) { for (unsigned k = 0; k < 4; ++k) data[k] += m.data[k]; return *this; }
    /** Subtracts the given matrix from this one, resulting in the (element-wise) matrix difference. */
    constexpr Matrix2x2& operator-=(const Matrix2x2& m) { for (unsigned k = 0; k < 4; ++k) data[k] -= m.data[k]; return *this; }

    /** "Scales" this matrix by the specified factor (scalar multiplication). */
    constexpr Matrix2x2& operator*=(float s) { for (float& k : data) k *= s; return *this; }
    /** Inversely "scales" this matrix by the specified factor (scalar division). */
    constexpr Matrix2x2& operator/=(float s) { for (float& k : data) k /= s; return *this; }



    // #############################################################################
    // Access Operators
    // #############################################################################

    std::span<const float, 2> operator[](unsigned i) const { assert(i < 2); return std::span<const float, 2>{m[i], 2}; }
    std::span<float, 2> operator[](unsigned i) { assert(i < 2); return std::span<float, 2>{m[i], 2}; }



    // #############################################################################
    // Static Functions
    // #############################################################################

    /**
     * Initializes a transformation matrix from the specified basis vectors that
     * define the transformed coordinate system (as the column vectors of the matrix).
     */
    static constexpr Matrix2x2 from_axes(const ml7::Vector2& x, const ml7::Vector2& y)
    {
        return {
            x.x, y.x,
            x.y, y.y,
        };
    }

    /**
     * Initializes a scaling matrix with scaling factor s.
     */
    static constexpr Matrix2x2 scaling(float s)
    {
        return {
            s, 0.0f,
            0.0f, s,
        };
    }

    /**
     * Initializes a scaling matrix with scaling vector s.
     */
    static constexpr Matrix2x2 scaling(const ml7::Vector2& s)
    {
        return {
            s.x, 0.0f,
            0.0f, s.y,
        };
    }

    /**
     * Initializes a rotation matrix representing a counter-clockwise rotation by
     * a certain angle (in radians).
     */
    static Matrix2x2 rotation(float angle)
    {
        const float cs = ::cosf(angle);
        const float sn = ::sinf(angle);
        return {
            cs, -sn,
            sn, cs,
        };
    }

    /**
     * Initializes a transformation matrix from the specified scaling factor and a
     * counter-clockwise rotation angle (in radians).
     */
    static Matrix2x2 compose(float scaling, float angle)
    {
        return compose({scaling, scaling}, angle);
    }

    /**
     * Initializes a transformation matrix from the specified scaling vector and a
     * counter-clockwise rotation angle (in radians).
     */
    static Matrix2x2 compose(const ml7::Vector2& scaling, float angle);

}; // struct Matrix2x2



    // #############################################################################
    // Additional Operators
    // #############################################################################

    /** "Scales" a matrix by the specified factor (scalar multiplication). */
    constexpr Matrix2x2 operator*(float s, const Matrix2x2& m) { return m * s; }



} // namespace ml7

#endif // ML7_MATRIX2X2_H
