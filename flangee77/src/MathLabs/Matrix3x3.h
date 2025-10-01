#ifndef ML7_MATRIX3X3_H
#define ML7_MATRIX3X3_H

#include "./Vector3.h"

#include <span>



namespace ml7 {



template <std::floating_point T>
struct Matrix3x3
{

    using scalar_type = T;

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
            /** Element in the 3rd row and 1st column. */
            T _31;
            /** Element in the 3rd row and 2nd column. */
            T _32;
            /** Element in the 3rd row and 3rd column. */
            T _33;
            // NOLINTEND(*-use-default-member-init)
        }; // struct

        struct
        {
            /** Element in the 1st row and 1st column. */
            T a;
            /** Element in the 1st row and 2nd column. */
            T b;
            /** Element in the 1st row and 3rd column. */
            T c;
            /** Element in the 2nd row and 1st column. */
            T d;
            /** Element in the 2nd row and 2nd column. */
            T e;
            /** Element in the 2nd row and 3rd column. */
            T f;
            /** Element in the 3rd row and 1st column. */
            T g;
            /** Element in the 3rd row and 2nd column. */
            T h;
            /** Element in the 3rd row and 3rd column. */
            T i;
        };

        /** Array of the elements. */
        T data[9];

        /** 2D array of the elements. */
        T m[3][3];
    }; // union



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes an identity matrix representing a neutral
     * transformation.
     */
    constexpr Matrix3x3() noexcept
        : _11(T{1}), _12(T{0}), _13(T{0})
        , _21(T{0}), _22(T{1}), _23(T{0})
        , _31(T{0}), _32(T{0}), _33(T{1})
    {
    }

    /**
     * Explicit constructor with parameters for each element.
     */
    constexpr Matrix3x3(
        T _11, T _12, T _13,
        T _21, T _22, T _23,
        T _31, T _32, T _33) noexcept
        : _11(_11), _12(_12), _13(_13)
        , _21(_21), _22(_22), _23(_23)
        , _31(_31), _32(_32), _33(_33)
    {
    }

    /**
     * Swap operation.
     */
    void swap(Matrix3x3& other) noexcept
    {
        Matrix3x3 tmp(other);
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
    Matrix3x3 inverted() const
    {
        const T det = determinant();
        if (det == T{0})
            return ZERO;
        const T i = T{1} / det;
        return {
            (_22*_33 - _23*_32) * i,   (_13*_32 - _12*_33) * i,   (_12*_23 - _13*_22) * i,
            (_23*_31 - _21*_33) * i,   (_11*_33 - _13*_31) * i,   (_13*_21 - _11*_23) * i,
            (_21*_32 - _22*_31) * i,   (_12*_31 - _11*_32) * i,   (_11*_22 - _12*_21) * i,
        };
    }

    /**
     * Returns the i-th (0-indexed) row vector of this matrix.
     */
    Vector3<T> get_row(unsigned i) const
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
    Vector3<T> get_column(unsigned j) const
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
    void to_axes(Vector3<T>& x, Vector3<T>& y, Vector3<T>& z) const
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
    bool to_axis_angle(Vector3<T>& axis, T& angle) const
    {
        T t = _11 + _22 + _33;
        T cs = T{0.5} * (t - T{1});
        angle = std::acos(cs); // [0;pi]

        if (!(angle > T{0}))
        {
            axis = Vector3<T>::X; // Any axis would be "correct" here.
            return angle == T{0};
        }

        if (angle < constants<T>::pi)
        {
            axis.x = _32 - _23;
            axis.y = _13 - _31;
            axis.z = _21 - _12;
            axis.normalize();
            return true;
        }

        T _05i;
        if (_11 >= _22)
        {
            if (_11 >= _33) // => _11 is maximum
            {
                axis.x = T{0.5} * std::sqrt(_11 - _22 - _33 + T{1});
                _05i = T{0.5} / axis.x;
                axis.y = _12 * _05i;
                axis.z = _13 * _05i;
            }
            else // => _33 > _11 => _33 is maximum
            {
                axis.z = T{0.5} * std::sqrt(_33 - _11 - _22 + T{1});
                _05i = T{0.5} / axis.z;
                axis.x = _13 * _05i;
                axis.y = _23 * _05i;
            }
        }
        else // => _22 >= _11
        {
            if (_22 >= _33) // => _22 is maximum
            {
                axis.y = T{0.5} * std::sqrt(_22 - _11 - _33 + T{1});
                _05i = T{0.5} / axis.y;
                axis.x = _12 * _05i;
                axis.z = _23 * _05i;
            }
            else // => _33 > _22 => => _33 is maximum
            {
                axis.z = T{0.5} * std::sqrt(_33 - _11 - _22 + T{1});
                _05i = T{0.5} / axis.z;
                axis.x = _13 * _05i;
                axis.y = _23 * _05i;
            }
        }

        return true;
    }

    /**
     * Tries to extract the scaling vector, the rotation axis, and the rotation
     * angle (in the range [0;pi]) this matrix is composed of.
     * This only works if the matrix actually consists of rotations and (positive)
     * scalings in the "common" order (no shears, negative scalings, etc.).
     */
    bool decompose(Vector3<T>& scaling, Vector3<T>& axis, T& angle) const
    {
        const T sx = Vector3<T>{_11, _21, _31}.length();
        const T sy = Vector3<T>{_12, _22, _32}.length();
        const T sz = Vector3<T>{_13, _23, _33}.length();
        if (sx == T{0}) return false;
        if (sy == T{0}) return false;
        if (sz == T{0}) return false;
        scaling.x = sx;
        scaling.y = sy;
        scaling.z = sz;

        const T sxi = T{1} / sx;
        const T syi = T{1} / sy;
        const T szi = T{1} / sz;
        const Matrix3x3 m{
            _11 * sxi, _12 * syi, _13 * szi,
            _21 * sxi, _22 * syi, _23 * szi,
            _31 * sxi, _32 * syi, _33 * szi,
        };

        return m.to_axis_angle(axis, angle);
    }

    /**
     * Assumes that this matrix is a view rotation matrix for a left-handed
     * coordinate system and tries to extract the camera "look" direction and the
     * "up" direction.
     */
    bool is_look_lh(Vector3<T>& look, Vector3<T>& up) const
    {
        look = {_31, _32, _33};
        up = {_21, _22, _23};
        return look.length_squared() != T{0} && up.length_squared() != T{0}; // Just some bare minimum check.
    }

    /**
     * Assumes that this matrix is a view rotation matrix for a right-handed
     * coordinate system and tries to extract the camera "look" direction and the
     * "up" direction.
     */
    bool is_look_rh(Vector3<T>& look, Vector3<T>& up) const
    {
        look = {-_31, -_32, -_33};
        up = {_21, _22, _23};
        return look.length_squared() != T{0} && up.length_squared() != T{0}; // Just some bare minimum check.
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix.
     */
    constexpr Vector3<T> transform(const Vector3<T>& v) const
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
     */
    Vector3<T> transform_inverted(const Vector3<T>& v) const
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
    Matrix3x3& set_row(unsigned i, Vector3<T> v)
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
    Matrix3x3& set_column(unsigned j, Vector3<T> v)
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
    constexpr Matrix3x3 operator*(T s) const { return {a * s, b * s, c * s, d * s, e * s, f * s, g * s, h * s, i * s}; }
    /** Returns a copy of this vector inversely "scaled" by the specified factor (scalar division). */
    constexpr Matrix3x3 operator/(T s) const { return {a / s, b / s, c / s, d / s, e / s, f / s, g / s, h / s, i / s}; }

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
    constexpr Vector3<T> operator*(const Vector3<T>& v) const { return transform(v); }

    /** "Scales" a matrix by the specified factor (scalar multiplication). */
    friend constexpr Matrix3x3 operator*(T s, const Matrix3x3& m) { return m * s; }



    // #############################################################################
    // Arithmetic Assignment Operators
    // #############################################################################

    /** Adds the given matrix to this one, resulting in the (element-wise) matrix sum. */
    constexpr Matrix3x3& operator+=(const Matrix3x3& m) { for (unsigned k = 0; k < 9; ++k) data[k] += m.data[k]; return *this; }
    /** Subtracts the given matrix from this one, resulting in the (element-wise) matrix difference. */
    constexpr Matrix3x3& operator-=(const Matrix3x3& m) { for (unsigned k = 0; k < 9; ++k) data[k] -= m.data[k]; return *this; }

    /** "Scales" this matrix by the specified factor (scalar multiplication). */
    constexpr Matrix3x3& operator*=(T s) { for (T& k : data) k *= s; return *this; }
    /** Inversely scales this matrix by the specified factor (scalar division). */
    constexpr Matrix3x3& operator/=(T s) { for (T& k : data) k /= s; return *this; }



    // #############################################################################
    // Access Operators
    // #############################################################################

    std::span<const T, 3> operator[](unsigned i) const { assert(i < 3); return std::span<const T, 3>{m[i], 3}; }
    std::span<T, 3> operator[](unsigned i) { assert(i < 3); return std::span<T, 3>{m[i], 3}; }



    // #############################################################################
    // Static Functions
    // #############################################################################

    /**
     * Initializes a transformation matrix from the specified basis vectors that
     * define the transformed coordinate system (as the column vectors of the matrix).
     */
    static constexpr Matrix3x3 from_axes(const Vector3<T>& x, const Vector3<T>& y, const Vector3<T>& z)
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
    static constexpr Matrix3x3 scaling(T s)
    {
        return {
            s, T{0}, T{0},
            T{0}, s, T{0},
            T{0}, T{0}, s,
        };
    }

    /**
     * Initializes a scaling matrix with scaling vector s.
     */
    static constexpr Matrix3x3 scaling(const Vector3<T>& s)
    {
        return {
            s.x, T{0}, T{0},
            T{0}, s.y, T{0},
            T{0}, T{0}, s.z,
        };
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around the x-axis.
     */
    static Matrix3x3 rotx(T angle)
    {
        return rotx(std::sin(angle), std::cos(angle));
    }

    /**
     * Initializes a rotation matrix representing a rotation around the x-axis based
     * on ready-made sine and cosine values.
     */
    static Matrix3x3 rotx(T sin, T cos)
    {
        return {
            T{1}, T{0}, T{0},
            T{0},  cos, -sin,
            T{0},  sin,  cos,
        };
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around the y-axis.
     */
    static Matrix3x3 roty(T angle)
    {
        return roty(std::sin(angle), std::cos(angle));
    }

    /**
     * Initializes a rotation matrix representing a rotation around the y-axis based
     * on ready-made sine and cosine values.
     */
    static Matrix3x3 roty(T sin, T cos)
    {
        return {
             cos, T{0},  sin,
            T{0}, T{1}, T{0},
            -sin, T{0},  cos,
        };
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around the z-axis.
     */
    static Matrix3x3 rotz(T angle)
    {
        return rotz(std::sin(angle), std::cos(angle));
    }

    /**
     * Initializes a rotation matrix representing a rotation around the z-axis based
     * on ready-made sine and cosine values.
     */
    static Matrix3x3 rotz(T sin, T cos)
    {
        return {
             cos, -sin, T{0},
             sin,  cos, T{0},
            T{0}, T{0}, T{1},
        };
    }

    /**
     * Initializes a rotation matrix representing a rotation around the specified
     * axis, with an angle equal to the magnitude of the specified vector (in
     * radians).
     */
    static Matrix3x3 rotation(const Vector3<T>& axis_angle)
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
    static Matrix3x3 rotation(const Vector3<T>& axis, T angle)
    {
        if (angle == T{0})
            return IDENTITY;
        return rotation_normalized(axis.normalized(), angle);
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around an axis specified as a normalized unit vector.
     */
    static Matrix3x3 rotation_normalized(const Vector3<T>& unit_axis, T angle)
    {
        const Vector3<T>& u = unit_axis;
        if (angle == T{0})
            return IDENTITY;
        const T cs = std::cos(angle);
        const T sn = std::sin(angle);
        const T C = T{1} - cs;
        const T xsn = u.x * sn;
        const T ysn = u.y * sn;
        const T zsn = u.z * sn;
        const T xC = u.x * C;
        const T yC = u.y * C;
        const T zC = u.z * C;
        const T xyC = u.x * yC;
        const T xzC = u.x * zC;
        const T yzC = u.y * zC;
        return {
            u.x*xC + cs,   xyC - zsn,     xzC + ysn,
            xyC + zsn,     u.y*yC + cs,   yzC - xsn,
            xzC - ysn,     yzC + xsn,     u.z*zC + cs,
        };
    }

    /**
     * Initializes a transformation matrix from the specified scaling factor, a
     * rotation axis, and a rotation angle (in radians).
     */
    static Matrix3x3 compose(T scaling, const Vector3<T>& axis, T angle)
    {
        return compose({scaling, scaling, scaling}, axis, angle);
    }

    /**
     * Initializes a transformation matrix from the specified scaling vector, a
     * rotation axis, and a rotation angle (in radians).
     */
    static Matrix3x3 compose(const Vector3<T>& scaling, const Vector3<T>& axis, T angle)
    {
        if (angle == T{0})
            return Matrix3x3::scaling(scaling);
        const T sx = scaling.x;
        const T sy = scaling.y;
        const T sz = scaling.z;
        const Vector3<T> u = axis.normalized();
        const T cs = std::cos(angle);
        const T sn = std::sin(angle);
        const T C = T{1} - cs;
        const T xsn = u.x * sn;
        const T ysn = u.y * sn;
        const T zsn = u.z * sn;
        const T xC = u.x * C;
        const T yC = u.y * C;
        const T zC = u.z * C;
        const T xyC = u.x * yC;
        const T xzC = u.x * zC;
        const T yzC = u.y * zC;
        return {
            (u.x*xC + cs) * sx,   (xyC - zsn) * sy,     (xzC + ysn) * sz,
            (xyC + zsn) * sx,     (u.y*yC + cs) * sy,   (yzC - xsn) * sz,
            (xzC - ysn) * sx,     (yzC + xsn) * sy,     (u.z*zC + cs) * sz,
        };
    }

    /**
     * Initializes a view rotation matrix for a left-handed coordinate system using
     * a camera "look" direction and an "up" direction.
     */
    static Matrix3x3 look_lh(const Vector3<T>& look, const Vector3<T>& up)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatlh
        const Vector3<T> z(look.normalized());
        const Vector3<T> x(up.cross(z).normalized());
        const Vector3<T> y(z.cross(x));
        return {
            x.x,  x.y,  x.z,
            y.x,  y.y,  y.z,
            z.x,  z.y,  z.z,
        };
    }

    /**
     * Initializes a view rotation matrix for a right-handed coordinate system using
     * a camera "look" direction and an "up" direction.
     */
    static Matrix3x3 look_rh(const Vector3<T>& look, const Vector3<T>& up)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatrh
        return look_lh(-look, up);
    }

}; // struct Matrix3x3



    template <std::floating_point T>
    const Matrix3x3<T> Matrix3x3<T>::ZERO =       {T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}};
    template <std::floating_point T>
    const Matrix3x3<T> Matrix3x3<T>::IDENTITY =   {T{1}, T{0}, T{0}, T{0}, T{1}, T{0}, T{0}, T{0}, T{1}};



using Matrix3x3f = Matrix3x3<float>;
using Matrix3x3d = Matrix3x3<double>;
using Matrix3x3ld = Matrix3x3<long double>;

//extern template class Matrix3x3<float>;
//extern template class Matrix3x3<double>;



} // namespace ml7

#endif // ML7_MATRIX3X3_H
