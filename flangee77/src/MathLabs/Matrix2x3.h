#ifndef ML7_MATRIX2X3_H
#define ML7_MATRIX2X3_H

#include "./Matrix2x2.h"
#include "./Vector2.h"

#include <span>



namespace ml7 {



template <std::floating_point T>
struct Matrix2x3
{

    using scalar_type = T;

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
            T _11;
            /** Element in the 1st row and 2nd column. */
            T _12;
            /** Element in the 1st row and 3rd column. */
            T _13;
            /** Element in the 2nd row and 1st column. */
            T _21;
            /** Element in the 2nd row and 2nd column. */
            T _22;
            /** Element in the 2nd row and 3rd column. */
            T _23;
            // NOLINTEND(*-use-default-member-init)
        }; // struct

        struct
        {
            /** Element in the 1st row and 1st column. */
            T a;
            /** Element in the 1st row and 2nd column. */
            T b;
            /** Element in the 1nd row and 3rd column. */
            T c;
            /** Element in the 2nd row and 1st column. */
            T d;
            /** Element in the 2nd row and 2nd column. */
            T e;
            /** Element in the 2nd row and 3rd column. */
            T f;
        };

        /** Array of the elements. */
        T data[6];

        /** 2D array of the elements. */
        T m[2][3];
    }; // union



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes an identity matrix representing a neutral
     * transformation.
     */
    constexpr Matrix2x3() noexcept
        : _11(T{1}), _12(T{0}), _13(T{0})
        , _21(T{0}), _22(T{1}), _23(T{0})
    {
    }

    /**
     * Explicit constructor with parameters for each element.
     */
    constexpr Matrix2x3(
        T _11, T _12, T _13,
        T _21, T _22, T _23) noexcept
        : _11(_11), _12(_12), _13(_13)
        , _21(_21), _22(_22), _23(_23)
    {
    }

    /**
     * Explicit constructor from a 2x2 matrix.
     */
    constexpr explicit Matrix2x3(const Matrix2x2<T>& m2x2) noexcept
        : _11(m2x2._11), _12(m2x2._12), _13(T{0})
        , _21(m2x2._21), _22(m2x2._22), _23(T{0})
    {
    }

    /**
     * Explicit constructor from a 2x2 matrix and a translation vector.
     */
    constexpr Matrix2x3(const Matrix2x2<T>& m2x2, const Vector2<T>& translation) noexcept
        : _11(m2x2._11), _12(m2x2._12), _13(translation.x)
        , _21(m2x2._21), _22(m2x2._22), _23(translation.y)
    {
    }

    /**
     * Swap operation.
     */
    void swap(Matrix2x3& other) noexcept
    {
        Matrix2x3 tmp(other);
        other = *this;
        *this = tmp;
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Tells whether this matrix is invertible (i.e., whether the determinant of its
     * 2x2 part is non-zero).
     */
    bool is_invertible() const { return determinant() != T{0}; }

    /**
     * Returns the determinant of the 2x2 part of the matrix.
     */
    T determinant() const { return _11*_22 - _12*_21; }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this matrix inverted (if possible).
     */
    Matrix2x3 inverted() const
    {
        const T det = determinant();
        if (det == T{0})
            return ZERO;
        const T i = T{1} / det;
        return {
            _22 * i, -_12 * i, (_12*_23 - _22*_13) * i,
            -_21 * i, _11 * i, (_21*_13 - _11*_23) * i,
        };
    }

    /**
     * Returns the j-th (0-indexed) column vector of this matrix.
     */
    Vector2<T> get_column(unsigned j) const
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
    void to_axes(Vector2<T>& x, Vector2<T>& y) const
    {
        x.x = _11;   y.x = _12;
        x.y = _21;   y.y = _22;
    }

    /**
     * Extracts the basis vectors and origin/translation vector that define the
     * transformed coordinate system (i.e., the column vectors of the matrix).
     */
    void to_axes(Vector2<T>& x, Vector2<T>& y, Vector2<T>& origin) const
    {
        x.x = _11;   y.x = _12;   origin.x = _13;
        x.y = _21;   y.y = _22;   origin.y = _23;
    }

    /**
     * Returns the 2x2 part of this matrix.
     */
    Matrix2x2<T> to_matrix2x2() const
    {
        return {
            _11, _12,
            _21, _22,
        };
    }

    /**
     * Decomposes this matrix into a 2x2 matrix and a translation vector.
     */
    void decompose(Matrix2x2<T>& m2x2, Vector2<T>& translation) const
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
    bool decompose(Vector2<T>& scaling, T& angle, Vector2<T>& translation) const
    {
        const T sx = Vector2<T>{_11, _21}.length();
        const T sy = Vector2<T>{_12, _22}.length();
        if (sx == T{0}) return false;
        if (sy == T{0}) return false;
        scaling.x = sx;
        scaling.y = sy;
        angle = std::atan2(-_12 / sy, _11 / sx);
        translation.x = _13;
        translation.y = _23;
        return true;
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix.
     * Used to transform position vectors rather than direction vectors.
     */
    constexpr Vector2<T> transform(const Vector2<T>& v) const
    {
        return {
            _11 * v.x + _12 * v.y + _13,
            _21 * v.x + _22 * v.y + _23,
        };
    }

    /**
     * Returns a copy of the given (column) vector transformed by the 2x2 part of
     * this matrix.
     * Used to transform direction vectors rather than position vectors.
     */
    constexpr Vector2<T> transform2x2(const Vector2<T>& v) const
    {
        return {
            _11 * v.x + _12 * v.y,
            _21 * v.x + _22 * v.y,
        };
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix
     * inverted (if possible).
     * Used to transform position vectors rather than direction vectors.
     */
    Vector2<T> transform_inverted(const Vector2<T>& v) const
    {
        return transform2x2_inverted(v - Vector2<T>{_13, _23});
    }

    /**
     * Returns a copy of the given (column) vector transformed by the 2x2 part of
     * this matrix inverted (if possible).
     * Used to transform direction vectors rather than position vectors.
     */
    Vector2<T> transform2x2_inverted(const Vector2<T>& v) const
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
    Matrix2x3& set_column(unsigned j, Vector2<T> v)
    {
        assert(j < 3);
        m[0][j] = v[0];
        m[1][j] = v[1];
        return *this;
    }



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const Matrix2x3& m) const noexcept { for (unsigned k = 0; k < 6; ++k) if (data[k] != m.data[k]) return false; return true; }
    bool operator!=(const Matrix2x3& m) const noexcept { for (unsigned k = 0; k < 6; ++k) if (data[k] != m.data[k]) return true; return false; }



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
    constexpr Matrix2x3 operator*(T s) const { return {a * s, b * s, c * s, d * s, e * s, f * s}; }
    /** Returns a copy of this matrix inversely "scaled" by the specified factor (scalar division). */
    constexpr Matrix2x3 operator/(T s) const { return {a / s, b / s, c / s, d / s, e / s, f / s}; }

    /** Returns the matrix product of two matrices (matrix multiplication). */
    constexpr Matrix2x3 operator*(const Matrix2x3& m) const
    {
        return {
            _11*m._11 + _12*m._21,   _11*m._12 + _12*m._22,   _11*m._13 + _12*m._23 + _13,
            _21*m._11 + _22*m._21,   _21*m._12 + _22*m._22,   _21*m._13 + _22*m._23 + _23,
        };
    }

    /** Returns a copy of the given (column vector) transformed by this matrix. */
    constexpr Vector2<T> operator*(const Vector2<T>& v) const { return transform(v); }

    /** "Scales" a matrix by the specified factor (scalar multiplication). */
    friend constexpr Matrix2x3 operator*(T s, const Matrix2x3& m) { return m * s; }



    // #############################################################################
    // Arithmetic Assignment Operators
    // #############################################################################

    /** Adds the given matrix to this one, resulting in the (element-wise) matrix sum. */
    constexpr Matrix2x3& operator+=(const Matrix2x3& m) { for (unsigned k = 0; k < 6; ++k) data[k] += m.data[k]; return *this; }
    /** Subtracts the given matrix from this one, resulting in the (element-wise) matrix difference. */
    constexpr Matrix2x3& operator-=(const Matrix2x3& m) { for (unsigned k = 0; k < 6; ++k) data[k] -= m.data[k]; return *this; }

    /** "Scales" this matrix by the specified factor (scalar multiplication). */
    constexpr Matrix2x3& operator*=(T s) { for (T& k : data) k *= s; return *this; }
    /** Inversely "scales" this matrix by the specified factor (scalar division). */
    constexpr Matrix2x3& operator/=(T s) { for (T& k : data) k /= s; return *this; }



    // #############################################################################
    // Access Operators
    // #############################################################################

    std::span<const T, 3> operator[](unsigned i) const { assert(i < 2); return std::span<const T, 3>{m[i], 3}; }
    std::span<T, 3> operator[](unsigned i) { assert(i < 2); return std::span<T, 3>{m[i], 3}; }



    // #############################################################################
    // Static Functions
    // #############################################################################

    /**
     * Initializes a transformation matrix from the specified basis vectors that
     * define the transformed coordinate system (as the column vectors of the matrix).
     */
    static constexpr Matrix2x3 from_axes(const Vector2<T>& x, const Vector2<T>& y)
    {
        return {
            x.x, y.x, T{0},
            x.y, y.y, T{0},
        };
    }

    /**
     * Initializes a transformation matrix from the specified basis vectors and
     * origin/translation vector that define the transformed coordinate system (as
     * the column vectors of the matrix).
     */
    static constexpr Matrix2x3 from_axes(const Vector2<T>& x, const Vector2<T>& y, const Vector2<T>& origin)
    {
        return {
            x.x, y.x, origin.x,
            x.y, y.y, origin.y,
        };
    }

    /**
     * Initializes a scaling matrix with scaling factor s.
     */
    static constexpr Matrix2x3 scaling(T s)
    {
        return {
            s, T{0}, T{0},
            T{0}, s, T{0},
        };
    }

    /**
     * Initializes a scaling matrix with scaling vector s.
     */
    static constexpr Matrix2x3 scaling(const Vector2<T>& s)
    {
        return {
            s.x, T{0}, T{0},
            T{0}, s.y, T{0},
        };
    }

    /**
     * Initializes a rotation matrix representing a counter-clockwise rotation by
     * a certain angle (in radians).
     */
    static Matrix2x3 rotation(T angle)
    {
        const T cs = std::cos(angle);
        const T sn = std::sin(angle);
        return {
            cs, -sn, T{0},
            sn, cs, T{0},
        };
    }

    /**
     * Initializes a translation matrix from the specified translation vector.
     */
    static Matrix2x3 translation(const Vector2<T>& translation)
    {
        return {
            T{1}, T{0}, translation.x,
            T{0}, T{1}, translation.y,
        };
    }

    /**
     * Initializes a transformation matrix from the specified scaling factor, a
     * counter-clockwise rotation angle (in radians), and a translation vector.
     */
    static Matrix2x3 compose(T scaling, T angle, const Vector2<T>& translation)
    {
        return compose({scaling, scaling}, angle, translation);
    }

    /**
     * Initializes a transformation matrix from the specified scaling vector, a
     * counter-clockwise rotation angle (in radians), and a translation vector.
     */
    static Matrix2x3 compose(const Vector2<T>& scaling, T angle, const Vector2<T>& translation)
    {
        const T sx = scaling.x;
        const T sy = scaling.y;
        const T cs = std::cos(angle);
        const T sn = std::sin(angle);
        return {
            cs * sx,    -sn * sy,   translation.x,
            sn * sx,    cs * sy,    translation.y,
        };
    }

}; // struct Matrix2x3



    template <std::floating_point T>
    const Matrix2x3<T> Matrix2x3<T>::ZERO =       {T{0}, T{0}, T{0}, T{0}, T{0}, T{0}};
    template <std::floating_point T>
    const Matrix2x3<T> Matrix2x3<T>::IDENTITY =   {T{1}, T{0}, T{0}, T{0}, T{1}, T{0}};



using Matrix2x3f = Matrix2x3<float>;
using Matrix2x3d = Matrix2x3<double>;
using Matrix2x3ld = Matrix2x3<long double>;



} // namespace ml7

#endif // ML7_MATRIX2X3_H
