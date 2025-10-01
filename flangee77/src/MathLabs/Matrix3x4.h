#ifndef ML7_MATRIX3X4_H
#define ML7_MATRIX3X4_H

#include "./Matrix3x3.h"
#include "./Vector3.h"

#include <span>



namespace ml7 {



template <std::floating_point T>
struct Matrix3x4
{

    using scalar_type = T;

    static const Matrix3x4 ZERO;
    static const Matrix3x4 IDENTITY;



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
            /** Element in the 1st row and 4th column. */
            T _14;
            /** Element in the 2nd row and 1st column. */
            T _21;
            /** Element in the 2nd row and 2nd column. */
            T _22;
            /** Element in the 2nd row and 3rd column. */
            T _23;
            /** Element in the 2nd row and 4th column. */
            T _24;
            /** Element in the 3rd row and 1st column. */
            T _31;
            /** Element in the 3rd row and 2nd column. */
            T _32;
            /** Element in the 3rd row and 3rd column. */
            T _33;
            /** Element in the 3rd row and 4th column. */
            T _34;
            // NOLINTEND(*-use-default-member-init)
        }; // struct

        /** Array of the elements. */
        T data[12];

        /** 2D array of the elements. */
        T m[3][4];
    }; // union



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes an identity matrix representing a neutral
     * transformation.
     */
    constexpr Matrix3x4() noexcept
        : _11(T{1}), _12(T{0}), _13(T{0}), _14(T{0})
        , _21(T{0}), _22(T{1}), _23(T{0}), _24(T{0})
        , _31(T{0}), _32(T{0}), _33(T{1}), _34(T{0})
    {
    }

    /**
     * Explicit constructor with parameters for each element.
     */
    constexpr Matrix3x4(
        T _11, T _12, T _13, T _14,
        T _21, T _22, T _23, T _24,
        T _31, T _32, T _33, T _34) noexcept
        : _11(_11), _12(_12), _13(_13), _14(_14)
        , _21(_21), _22(_22), _23(_23), _24(_24)
        , _31(_31), _32(_32), _33(_33), _34(_34)
    {
    }

    /**
     * Explicit constructor from a 3x3 matrix.
     */
    constexpr explicit Matrix3x4(const Matrix3x3<T>& m3x3) noexcept
        : _11(m3x3._11), _12(m3x3._12), _13(m3x3._13), _14(T{0})
        , _21(m3x3._21), _22(m3x3._22), _23(m3x3._23), _24(T{0})
        , _31(m3x3._31), _32(m3x3._32), _33(m3x3._33), _34(T{0})
    {
    }

    /**
     * Explicit constructor from a 3x3 matrix and a translation vector.
     */
    constexpr Matrix3x4(const Matrix3x3<T>& m3x3, const Vector3<T>& translation) noexcept
        : _11(m3x3._11), _12(m3x3._12), _13(m3x3._13), _14(translation.x)
        , _21(m3x3._21), _22(m3x3._22), _23(m3x3._23), _24(translation.y)
        , _31(m3x3._31), _32(m3x3._32), _33(m3x3._33), _34(translation.z)
    {
    }


    /**
     * Swap operation.
     */
    void swap(Matrix3x4& other) noexcept
    {
        Matrix3x4 tmp(other);
        other = *this;
        *this = tmp;
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Tells whether this matrix is invertible (i.e., whether the determinant of its
     * 3x3 part is non-zero).
     */
    bool is_invertible() const { return determinant() != T{0}; }

    /**
     * Returns the determinant of the 3x3 part of the matrix.
     */
    T determinant() const
    {
        return
            + _11*_22*_33 + _12*_23*_31 + _13*_21*_32
            - _31*_22*_13 - _32*_23*_11 - _33*_21*_12;
    }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this matrix inverted (if possible).
     */
    Matrix3x4 inverted() const
    {
        const T det = determinant();
        if (det == T{0})
            return ZERO;
        const T i = T{1} / det;
        Matrix3x3<T> m3x3{
            (_22*_33 - _23*_32) * i,   (_13*_32 - _12*_33) * i,   (_12*_23 - _13*_22) * i,
            (_23*_31 - _21*_33) * i,   (_11*_33 - _13*_31) * i,   (_13*_21 - _11*_23) * i,
            (_21*_32 - _22*_31) * i,   (_12*_31 - _11*_32) * i,   (_11*_22 - _12*_21) * i,
        };
        Vector3<T> translation{
            -(m3x3._11*_14 + m3x3._12*_24 + m3x3._13*_34),
            -(m3x3._21*_14 + m3x3._22*_24 + m3x3._23*_34),
            -(m3x3._31*_14 + m3x3._32*_24 + m3x3._33*_34),
        };
        return {m3x3, translation};
    }

    /**
     * Returns the j-th (0-indexed) column vector of this matrix.
     */
    Vector3<T> get_column(unsigned j) const
    {
        assert(j < 4);
        return {
            m[0][j],
            m[1][j],
            m[2][j],
        };
    }

    /**
     * Extracts the basis vectors that define the transformed coordinate system
     * (i.e., the column vectors of the 3x3 part of the matrix).
     */
    void to_axes(Vector3<T>& x, Vector3<T>& y, Vector3<T>& z) const
    {
        x.x = _11;   y.x = _12;   z.x = _13;
        x.y = _21;   y.y = _22;   z.y = _23;
        x.z = _31;   y.z = _32;   z.z = _33;
    }

    /**
     * Extracts the basis vectors and origin/translation vector that define the
     * transformed coordinate system (i.e., the column vectors of the matrix).
     */
    void to_axes(Vector3<T>& x, Vector3<T>& y, Vector3<T>& z, Vector3<T>& origin) const
    {
        x.x = _11;   y.x = _12;   z.x = _13;   origin.x = _14;
        x.y = _21;   y.y = _22;   z.y = _23;   origin.y = _24;
        x.z = _31;   y.z = _32;   z.z = _33;   origin.z = _34;
    }

    /**
     * Tries to extract the rotation axis and the rotation angle (in the range
     * [0;pi]) the 3x3 part of this matrix is composed of.
     * This only works if the 3x3 part actually consists of rotations only (no
     * scalings, shears, etc.).
     */
    bool to_axis_angle(Vector3<T>& axis, T& angle) const
    {
        return to_matrix3x3().to_axis_angle(axis, angle);
    }

    /**
     * Returns the 3x3 part of this matrix.
     */
    Matrix3x3<T> to_matrix3x3() const
    {
        return {
            _11, _12, _13,
            _21, _22, _23,
            _31, _32, _33,
        };
    }

    /**
     * Decomposes this matrix into a 3x3 matrix and a translation vector.
     */
    void decompose(Matrix3x3<T>& m3x3, Vector3<T>& translation) const
    {
        m3x3._11 = _11;   m3x3._12 = _12;   m3x3._13 = _13;   translation.x = _14;
        m3x3._21 = _21;   m3x3._22 = _22;   m3x3._23 = _23;   translation.y = _24;
        m3x3._31 = _31;   m3x3._32 = _32;   m3x3._33 = _33;   translation.z = _34;
    }

    /**
     * Tries to extract the scaling vector, the rotation axis, the rotation angle
     * (in the range [0;pi]), and the translation vector this matrix is composed of.
     * This only works if the matrix actually consists of translations, rotations and
     * (positive) scalings in the "common" order (no shears, negative scalings, etc.).
     */
    bool decompose(Vector3<T>& scaling, Vector3<T>& axis, T& angle, Vector3<T>& translation) const
    {
        translation.x = _14;
        translation.y = _24;
        translation.z = _34;
        return to_matrix3x3().decompose(scaling, axis, angle);
    }

    /**
     * Assumes that this matrix is a view transformation matrix for a left-handed
     * coordinate system and tries to extract the camera position, the camera "look"
     * direction, and the "up" direction.
     */
    bool is_look_lh(Vector3<T>& position, Vector3<T>& look, Vector3<T>& up) const
    {
        // Since we assume that the 3x3 part is a rotation matrix,
        // we can simply transpose it (instead of "costly" inverting it).
        position = {
            -(_11*_14 + _21*_24 + _31*_34),
            -(_12*_14 + _22*_24 + _32*_34),
            -(_13*_14 + _23*_24 + _33*_34),
        };
        look = {_31, _32, _33};
        up = {_21, _22, _23};
        return look.length_squared() != T{0} && up.length_squared() != T{0}; // Just some bare minimum check.
    }

    /**
     * Assumes that this matrix is a view transformation matrix for a right-handed
     * coordinate system and tries to extract the camera position, the camera "look"
     * direction, and the "up" direction.
     */
    bool is_look_rh(Vector3<T>& position, Vector3<T>& look, Vector3<T>& up) const
    {
        // Since we assume that the 3x3 part is a rotation matrix,
        // we can simply transpose it (instead of "costly" inverting it).
        position = {
            -(_11*_14 + _21*_24 + _31*_34),
            -(_12*_14 + _22*_24 + _32*_34),
            -(_13*_14 + _23*_24 + _33*_34),
        };
        look = {-_31, -_32, -_33};
        up = {_21, _22, _23};
        return look.length_squared() != T{0} && up.length_squared() != T{0}; // Just some bare minimum check.
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix.
     * Used to transform position vectors rather than direction vectors.
     */
    constexpr Vector3<T> transform(const Vector3<T>& v) const
    {
        return {
            _11 * v.x + _12 * v.y + _13 * v.z + _14,
            _21 * v.x + _22 * v.y + _23 * v.z + _24,
            _31 * v.x + _32 * v.y + _33 * v.z + _34,
        };
    }

    /**
     * Returns a copy of the given (column) vector transformed by the 3x3 part of
     * this matrix.
     * Used to transform direction vectors rather than position vectors.
     */
    constexpr Vector3<T> transform3x3(const Vector3<T>& v) const
    {
        return {
            _11 * v.x + _12 * v.y + _13 * v.z,
            _21 * v.x + _22 * v.y + _23 * v.z,
            _31 * v.x + _32 * v.y + _33 * v.z,
        };
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix
     * inverted (if possible).
     * Used to transform position vectors rather than direction vectors.
     */
    Vector3<T> transform_inverted(const Vector3<T>& v) const
    {
        return transform3x3_inverted(v - Vector3<T>{_14, _24, _34});
    }

    /**
     * Returns a copy of the given (column) vector transformed by the 3x3 part of
     * this matrix inverted (if possible).
     * Used to transform direction vectors rather than position vectors.
     */
    Vector3<T> transform3x3_inverted(const Vector3<T>& v) const
    {
        const T det = determinant();
        if (det == T{0})
            return v;
        const T i = T{1} / det;
        return {
            ((_22*_33 - _23*_32) * v.x + (_13*_32 - _12*_33) * v.y + (_12*_23 - _13*_22) * v.z) * i,
            ((_23*_31 - _21*_33) * v.x + (_11*_33 - _13*_31) * v.y + (_13*_21 - _11*_23) * v.z) * i,
            ((_21*_32 - _22*_31) * v.x + (_12*_31 - _11*_32) * v.y + (_11*_22 - _12*_21) * v.z) * i,
        };
    }



    // #############################################################################
    // Manipulations
    // #############################################################################

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
    Matrix3x4& set_column(unsigned j, Vector3<T> v)
    {
        assert(j < 4);
        m[0][j] = v[0];
        m[1][j] = v[1];
        m[2][j] = v[2];
        return *this;
    }



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const Matrix3x4& m) const noexcept { for (unsigned k = 0; k < 12; ++k) if (data[k] != m.data[k]) return false; return true; }
    bool operator!=(const Matrix3x4& m) const noexcept { for (unsigned k = 0; k < 12; ++k) if (data[k] != m.data[k]) return true; return false; }



    // #############################################################################
    // Arithmetic Operators
    // #############################################################################

    /** Returns a copy of this matrix unmodified. */
    constexpr Matrix3x4 operator+() const { return *this; }
    /** Returns a copy of this matrix with the signs of the elements flipped. */
    constexpr Matrix3x4 operator-() const { return {-_11, -_12, -_13, -_14, -_21, -_22, -_23, -_24, -_31, -_32, -_33, -_34}; }

    /** Returns the (element-wise) matrix sum of two matrices. */
    constexpr Matrix3x4 operator+(const Matrix3x4& m) const { return {_11 + m._11, _12 + m._12, _13 + m._13, _14 + m._14, _21 + m._21, _22 + m._22, _23 + m._23, _24 + m._24, _31 + m._31, _32 + m._32, _33 + m._33, _34 + m._34}; }
    /** Returns the (element-wise) matrix difference of two matrices. */
    constexpr Matrix3x4 operator-(const Matrix3x4& m) const { return {_11 - m._11, _12 - m._12, _13 - m._13, _14 - m._14, _21 - m._21, _22 - m._22, _23 - m._23, _24 - m._24, _31 - m._31, _32 - m._32, _33 - m._33, _34 - m._34}; }

    /** Returns a copy of this matrix "scaled" by the specified factor (scalar multiplication). */
    constexpr Matrix3x4 operator*(T s) const { return {_11 * s, _12 * s, _13 * s, _14 * s, _21 * s, _22 * s, _23 * s, _24 * s, _31 * s, _32 * s, _33 * s, _34 * s}; }
    /** Returns a copy of this matrix inversely "scaled" by the specified factor (scalar division). */
    constexpr Matrix3x4 operator/(T s) const { return {_11 / s, _12 / s, _13 / s, _14 / s, _21 / s, _22 / s, _23 / s, _24 / s, _31 / s, _32 / s, _33 / s, _34 / s}; }

    /** Returns the matrix product of two matrices (matrix multiplication). */
    constexpr Matrix3x4 operator*(const Matrix3x4& m) const
    {
        return {
            _11*m._11 + _12*m._21 + _13*m._31,   _11*m._12 + _12*m._22 + _13*m._32,   _11*m._13 + _12*m._23 + _13*m._33,   _11*m._14 + _12*m._24 + _13*m._34 + _14,
            _21*m._11 + _22*m._21 + _23*m._31,   _21*m._12 + _22*m._22 + _23*m._32,   _21*m._13 + _22*m._23 + _23*m._33,   _21*m._14 + _22*m._24 + _23*m._34 + _24,
            _31*m._11 + _32*m._21 + _33*m._31,   _31*m._12 + _32*m._22 + _33*m._32,   _31*m._13 + _32*m._23 + _33*m._33,   _31*m._14 + _32*m._24 + _33*m._34 + _34,
        };
    }

    /** Returns a copy of the given (column vector) transformed by this matrix. */
    constexpr Vector3<T> operator*(const Vector3<T>& v) const { return transform(v); }

    /** "Scales" a matrix by the specified factor (scalar multiplication). */
    friend constexpr Matrix3x4 operator*(T s, const Matrix3x4& m) { return m * s; }



    // #############################################################################
    // Arithmetic Assignment Operators
    // #############################################################################

    /** Adds the given matrix to this one, resulting in the (element-wise) matrix sum. */
    constexpr Matrix3x4& operator+=(const Matrix3x4& m) { for (unsigned k = 0; k < 12; ++k) data[k] += m.data[k]; return *this; }
    /** Subtracts the given matrix from this one, resulting in the (element-wise) matrix difference. */
    constexpr Matrix3x4& operator-=(const Matrix3x4& m) { for (unsigned k = 0; k < 12; ++k) data[k] -= m.data[k]; return *this; }

    /** "Scales" this matrix by the specified factor (scalar multiplication). */
    constexpr Matrix3x4& operator*=(T s) { for (T& k : data) k *= s; return *this; }
    /** Inversely "scales" this matrix by the specified factor (scalar division). */
    constexpr Matrix3x4& operator/=(T s) { for (T& k : data) k /= s; return *this; }



    // #############################################################################
    // Access Operators
    // #############################################################################

    std::span<const T, 4> operator[](unsigned i) const { assert(i < 3); return std::span<const T, 4>{m[i], 4}; }
    std::span<T, 4> operator[](unsigned i) { assert(i < 3); return std::span<T, 4>{m[i], 4}; }



    // #############################################################################
    // Static Functions
    // #############################################################################

    /**
     * Initializes a transformation matrix from the specified basis vectors that
     * define the transformed coordinate system (as the column vectors of the matrix).
     */
    static constexpr Matrix3x4 from_axes(const Vector3<T>& x, const Vector3<T>& y, const Vector3<T>& z)
    {
        return {
            x.x, y.x, z.x, T{0},
            x.y, y.y, z.y, T{0},
            x.z, y.z, z.z, T{0},
        };
    }

    /**
     * Initializes a transformation matrix from the specified basis vectors and
     * origin/translation vector that define the transformed coordinate system (as
     * the column vectors of the matrix).
     */
    static constexpr Matrix3x4 from_axes(const Vector3<T>& x, const Vector3<T>& y, const Vector3<T>& z, const Vector3<T>& origin)
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
    static constexpr Matrix3x4 scaling(T s)
    {
        return {
            s, T{0}, T{0}, T{0},
            T{0}, s, T{0}, T{0},
            T{0}, T{0}, s, T{0},
        };
    }

    /**
     * Initializes a scaling matrix with scaling vector s.
     */
    static constexpr Matrix3x4 scaling(const Vector3<T>& s)
    {
        return {
            s.x, T{0}, T{0}, T{0},
            T{0}, s.y, T{0}, T{0},
            T{0}, T{0}, s.z, T{0},
        };
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around the x-axis.
     */
    static Matrix3x4 rotx(T angle)
    {
        return rotx(std::sin(angle), std::cos(angle));
    }

    /**
     * Initializes a rotation matrix representing a rotation around the x-axis based
     * on ready-made sine and cosine values.
     */
    static Matrix3x4 rotx(T sin, T cos)
    {
        return {
            T{1}, T{0}, T{0}, T{0},
            T{0},  cos, -sin, T{0},
            T{0},  sin,  cos, T{0},
        };
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around the y-axis.
     */
    static Matrix3x4 roty(T angle)
    {
        return roty(std::sin(angle), std::cos(angle));
    }

    /**
     * Initializes a rotation matrix representing a rotation around the y-axis based
     * on ready-made sine and cosine values.
     */
    static Matrix3x4 roty(T sin, T cos)
    {
        return {
             cos, T{0},  sin, T{0},
            T{0}, T{1}, T{0}, T{0},
            -sin, T{0},  cos, T{0},
        };
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around the z-axis.
     */
    static Matrix3x4 rotz(T angle)
    {
        return rotz(std::sin(angle), std::cos(angle));
    }

    /**
     * Initializes a rotation matrix representing a rotation around the z-axis based
     * on ready-made sine and cosine values.
     */
    static Matrix3x4 rotz(T sin, T cos)
    {
        return {
             cos, -sin, T{0}, T{0},
             sin,  cos, T{0}, T{0},
            T{0}, T{0}, T{1}, T{0},
        };
    }

    /**
     * Initializes a rotation matrix representing a rotation around the specified
     * axis, with an angle equal to the magnitude of the specified vector (in
     * radians).
     */
    static Matrix3x4 rotation(const Vector3<T>& axis_angle)
    {
        const T angle = axis_angle.length();
        if (angle == T{0})
            return IDENTITY;
        return rotation_normalized(axis_angle / angle, angle);
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around the specified axis.
     */
    static Matrix3x4 rotation(const Vector3<T>& axis, T angle)
    {
        if (angle == T{0})
            return IDENTITY;
        return rotation_normalized(axis.normalized(), angle);
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around an axis specified as a normalized unit vector.
     */
    static Matrix3x4 rotation_normalized(const Vector3<T>& unit_axis, T angle)
    {
        return Matrix3x4{Matrix3x3<T>::rotation_normalized(unit_axis, angle)};
    }

    /**
     * Initializes a translation matrix from the specified translation vector.
     */
    static Matrix3x4 translation(const Vector3<T>& translation)
    {
        return {
            T{1}, T{0}, T{0}, translation.x,
            T{0}, T{1}, T{0}, translation.y,
            T{0}, T{0}, T{1}, translation.z,
        };
    }

    /**
     * Initializes a transformation matrix from the specified scaling factor, a
     * rotation axis, a rotation angle (in radians), and a translation vector.
     */
    static Matrix3x4 compose(T scaling, const Vector3<T>& axis, T angle, const Vector3<T>& translation)
    {
        return compose({scaling, scaling, scaling}, axis, angle, translation);
    }

    /**
     * Initializes a transformation matrix from the specified scaling vector, a
     * rotation axis, a rotation angle (in radians), and a translation vector.
     */
    static Matrix3x4 compose(const Vector3<T>& scaling, const Vector3<T>& axis, T angle, const Vector3<T>& translation)
    {
        return {Matrix3x3<T>::compose(scaling, axis, angle), translation};
    }

    /**
     * Initializes a view transformation matrix for a left-handed coordinate system
     * using a camera position, a focal point, and an "up" direction.
     */
    static Matrix3x4 look_at_lh(const Vector3<T>& position, const Vector3<T>& focus, const Vector3<T>& up)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatlh
        return look_to_lh(position, focus - position, up);
    }

    /**
     * Initializes a view transformation matrix for a right-handed coordinate system
     * using a camera position, a focal point, and an "up" direction.
     */
    static Matrix3x4 look_at_rh(const Vector3<T>& position, const Vector3<T>& focus, const Vector3<T>& up)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatrh
        return look_to_rh(position, focus - position, up);
    }

    /**
     * Initializes a view transformation matrix for a left-handed coordinate system
     * using a camera position, a camera "look" direction, and an "up" direction.
     */
    static Matrix3x4 look_to_lh(const Vector3<T>& position, const Vector3<T>& look, const Vector3<T>& up)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatlh
        const Vector3<T> z{look.normalized()};
        const Vector3<T> x{up.cross(z).normalized()};
        const Vector3<T> y{z.cross(x)};
        return {
            x.x,  x.y,  x.z,  -x.dot(position),
            y.x,  y.y,  y.z,  -y.dot(position),
            z.x,  z.y,  z.z,  -z.dot(position),
        };
    }

    /**
     * Initializes a view transformation matrix for a right-handed coordinate system
     * using a camera position, a camera "look" direction, and an "up" direction.
     */
    static Matrix3x4 look_to_rh(const Vector3<T>& position, const Vector3<T>& look, const Vector3<T>& up)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatrh
        return look_to_lh(position, -look, up);
    }

}; // struct Matrix3x4



    template <std::floating_point T>
    const Matrix3x4<T> Matrix3x4<T>::ZERO =       {T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}};
    template <std::floating_point T>
    const Matrix3x4<T> Matrix3x4<T>::IDENTITY =   {T{1}, T{0}, T{0}, T{0}, T{0}, T{1}, T{0}, T{0}, T{0}, T{0}, T{1}, T{0}};



using Matrix3x4f = Matrix3x4<float>;
using Matrix3x4d = Matrix3x4<double>;
using Matrix3x4ld = Matrix3x4<long double>;

//extern template class Matrix3x4<float>;
//extern template class Matrix3x4<double>;



} // namespace ml7

#endif // ML7_MATRIX3X4_H
