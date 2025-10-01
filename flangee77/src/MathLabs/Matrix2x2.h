#ifndef ML7_MATRIX2X2_H
#define ML7_MATRIX2X2_H

#include "./Vector2.h"

#include <span>



namespace ml7 {



template <std::floating_point T>
struct Matrix2x2
{

    using scalar_type = T;

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
            T _11;
            /** Element in the 1st row and 2nd column. */
            T _12;
            /** Element in the 2nd row and 1st column. */
            T _21;
            /** Element in the 2nd row and 2nd column. */
            T _22;
            // NOLINTEND(*-use-default-member-init)
        }; // struct

        struct
        {
            /** Element in the 1st row and 1st column. */
            T a;
            /** Element in the 1st row and 2nd column. */
            T b;
            /** Element in the 2nd row and 1st column. */
            T c;
            /** Element in the 2nd row and 2nd column. */
            T d;
        };

        /** Array of the elements. */
        T data[4];

        /** 2D array of the elements. */
        T m[2][2];
    }; // union



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes an identity matrix representing a neutral
     * transformation.
     */
    constexpr Matrix2x2() noexcept
        : _11(T{1}), _12(T{0})
        , _21(T{0}), _22(T{1})
    {
    }

    /**
     * Explicit constructor with parameters for each element.
     */
    constexpr Matrix2x2(
        T _11, T _12,
        T _21, T _22) noexcept
        : _11(_11), _12(_12)
        , _21(_21), _22(_22)
    {
    }

    /**
     * Swap operation.
     */
    void swap(Matrix2x2& other) noexcept
    {
        Matrix2x2 tmp(other);
        other = *this;
        *this = tmp;
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Tells whether this matrix is invertible (i.e., whether its determinant is
     * non-zero).
     */
    bool is_invertible() const { return determinant() != T{0}; }

    /**
     * Returns the determinant of the matrix.
     */
    T determinant() const { return _11*_22 - _12*_21; }



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
    Matrix2x2 inverted() const
    {
        const T det = determinant();
        if (det == T{0})
            return ZERO;
        const T i = T{1} / det;
        return {
            _22 * i, -_12 * i,
            -_21 * i, _11 * i,
        };
    }

    /**
     * Returns the i-th (0-indexed) row vector of this matrix.
     */
    Vector2<T> get_row(unsigned i) const
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
    Vector2<T> get_column(unsigned j) const
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
    void to_axes(Vector2<T>& x, Vector2<T>& y) const
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
    bool decompose(Vector2<T>& scaling, T& angle) const
    {
        const T sx = Vector2<T>{_11, _21}.length();
        const T sy = Vector2<T>{_12, _22}.length();
        if (sx == T{0}) return false;
        if (sy == T{0}) return false;
        scaling.x = sx;
        scaling.y = sy;
        angle = std::atan2(-_12 / sy, _11 / sx);
        return true;
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix.
     */
    constexpr Vector2<T> transform(const Vector2<T>& v) const
    {
        return {
            _11 * v.x + _12 * v.y,
            _21 * v.x + _22 * v.y,
        };
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix
     * inverted (if possible).
     */
    Vector2<T> transform_inverted(const Vector2<T>& v) const
    {
        const T det = determinant();
        if (det == T{0})
            return v;
        const T i = T{1} / det;
        return {
            (_22 * v.x + -_12 * v.y) * i,
            (-_21 * v.x + _11 * v.y) * i,
        };
    }



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
    Matrix2x2& set_row(unsigned i, Vector2<T> v)
    {
        assert(i < 2);
        m[i][0] = v[0];
        m[i][1] = v[1];
        return *this;
    }

    /**
     * Sets the j-th (0-indexed) column vector of this matrix.
     */
    Matrix2x2& set_column(unsigned j, Vector2<T> v)
    {
        assert(j < 2);
        m[0][j] = v[0];
        m[1][j] = v[1];
        return *this;
    }



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const Matrix2x2& m) const noexcept { for (unsigned k = 0; k < 4; ++k) if (data[k] != m.data[k]) return false; return true; }
    bool operator!=(const Matrix2x2& m) const noexcept { for (unsigned k = 0; k < 4; ++k) if (data[k] != m.data[k]) return true; return false; }



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
    constexpr Matrix2x2 operator*(T s) const { return {a * s, b * s, c * s, d * s}; }
    /** Returns a copy of this matrix inversely "scaled" by the specified factor (scalar division). */
    constexpr Matrix2x2 operator/(T s) const { return {a / s, b / s, c / s, d / s}; }

    /** Returns the matrix product of two matrices (matrix multiplication). */
    constexpr Matrix2x2 operator*(const Matrix2x2& m) const
    {
        return {
            _11*m._11 + _12*m._21,   _11*m._12 + _12*m._22,
            _21*m._11 + _22*m._21,   _21*m._12 + _22*m._22,
        };
    }

    /** Returns a copy of the given (column) vector transformed by this matrix. */
    constexpr Vector2<T> operator*(const Vector2<T>& v) const { return transform(v); }

    /** "Scales" a matrix by the specified factor (scalar multiplication). */
    friend constexpr Matrix2x2 operator*(T s, const Matrix2x2& m) { return m * s; }



    // #############################################################################
    // Arithmetic Assignment Operators
    // #############################################################################

    /** Adds the given matrix to this one, resulting in the (element-wise) matrix sum. */
    constexpr Matrix2x2& operator+=(const Matrix2x2& m) { for (unsigned k = 0; k < 4; ++k) data[k] += m.data[k]; return *this; }
    /** Subtracts the given matrix from this one, resulting in the (element-wise) matrix difference. */
    constexpr Matrix2x2& operator-=(const Matrix2x2& m) { for (unsigned k = 0; k < 4; ++k) data[k] -= m.data[k]; return *this; }

    /** "Scales" this matrix by the specified factor (scalar multiplication). */
    constexpr Matrix2x2& operator*=(T s) { for (T& k : data) k *= s; return *this; }
    /** Inversely "scales" this matrix by the specified factor (scalar division). */
    constexpr Matrix2x2& operator/=(T s) { for (T& k : data) k /= s; return *this; }



    // #############################################################################
    // Access Operators
    // #############################################################################

    std::span<const T, 2> operator[](unsigned i) const { assert(i < 2); return std::span<const T, 2>{m[i], 2}; }
    std::span<T, 2> operator[](unsigned i) { assert(i < 2); return std::span<T, 2>{m[i], 2}; }



    // #############################################################################
    // Static Functions
    // #############################################################################

    /**
     * Initializes a transformation matrix from the specified basis vectors that
     * define the transformed coordinate system (as the column vectors of the matrix).
     */
    static constexpr Matrix2x2 from_axes(const Vector2<T>& x, const Vector2<T>& y)
    {
        return {
            x.x, y.x,
            x.y, y.y,
        };
    }

    /**
     * Initializes a scaling matrix with scaling factor s.
     */
    static constexpr Matrix2x2 scaling(T s)
    {
        return {
            s, T{0},
            T{0}, s,
        };
    }

    /**
     * Initializes a scaling matrix with scaling vector s.
     */
    static constexpr Matrix2x2 scaling(const Vector2<T>& s)
    {
        return {
            s.x, T{0},
            T{0}, s.y,
        };
    }

    /**
     * Initializes a rotation matrix representing a counter-clockwise rotation by
     * a certain angle (in radians).
     */
    static Matrix2x2 rotation(T angle)
    {
        const T cs = std::cos(angle);
        const T sn = std::sin(angle);
        return {
            cs, -sn,
            sn, cs,
        };
    }

    /**
     * Initializes a transformation matrix from the specified scaling factor and a
     * counter-clockwise rotation angle (in radians).
     */
    static Matrix2x2 compose(T scaling, T angle)
    {
        return compose({scaling, scaling}, angle);
    }

    /**
     * Initializes a transformation matrix from the specified scaling vector and a
     * counter-clockwise rotation angle (in radians).
     */
    static Matrix2x2 compose(const Vector2<T>& scaling, T angle)
    {
        const T sx = scaling.x;
        const T sy = scaling.y;
        const T cs = std::cos(angle);
        const T sn = std::sin(angle);
        return {
            cs * sx,    -sn * sy,
            sn * sx,    cs * sy,
        };
    }

}; // struct Matrix2x2



    template <std::floating_point T>
    const Matrix2x2<T> Matrix2x2<T>::ZERO =       {T{0}, T{0}, T{0}, T{0}};
    template <std::floating_point T>
    const Matrix2x2<T> Matrix2x2<T>::IDENTITY =   {T{1}, T{0}, T{0}, T{1}};



using Matrix2x2f = Matrix2x2<float>;
using Matrix2x2d = Matrix2x2<double>;
using Matrix2x2ld = Matrix2x2<long double>;



} // namespace ml7

#endif // ML7_MATRIX2X2_H
