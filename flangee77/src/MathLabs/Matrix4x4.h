#ifndef ML7_MATRIX4X4_H
#define ML7_MATRIX4X4_H

#include "./Matrix3x4.h"
#include "./Matrix3x3.h"
#include "./Vector4.h"
#include "./Vector3.h"

#include <span>



namespace ml7 {



template <std::floating_point T>
struct Matrix4x4
{

    using scalar_type = T;

    static const Matrix4x4 ZERO;
    static const Matrix4x4 IDENTITY;



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
            /** Element in the 4th row and 1st column. */
            T _41;
            /** Element in the 4th row and 2nd column. */
            T _42;
            /** Element in the 4th row and 3rd column. */
            T _43;
            /** Element in the 4th row and 4th column. */
            T _44;
            // NOLINTEND(*-use-default-member-init)
        }; // struct

        /** Array of the elements. */
        T data[16];

        /** 2D array of the elements. */
        T m[4][4];
    }; // union



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes an identity matrix representing a neutral
     * transformation.
     */
    constexpr Matrix4x4() noexcept
        : _11(T{1}), _12(T{0}), _13(T{0}), _14(T{0})
        , _21(T{0}), _22(T{1}), _23(T{0}), _24(T{0})
        , _31(T{0}), _32(T{0}), _33(T{1}), _34(T{0})
        , _41(T{0}), _42(T{0}), _43(T{0}), _44(T{1})
    {
    }

    /**
     * Explicit constructor with parameters for each element.
     */
    constexpr Matrix4x4(
        T _11, T _12, T _13, T _14,
        T _21, T _22, T _23, T _24,
        T _31, T _32, T _33, T _34,
        T _41, T _42, T _43, T _44) noexcept
        : _11(_11), _12(_12), _13(_13), _14(_14)
        , _21(_21), _22(_22), _23(_23), _24(_24)
        , _31(_31), _32(_32), _33(_33), _34(_34)
        , _41(_41), _42(_42), _43(_43), _44(_44)
    {
    }

    /**
     * Explicit constructor from a 3x3 matrix.
     */
    constexpr explicit Matrix4x4(const Matrix3x3<T>& m3x3) noexcept
        : _11(m3x3._11), _12(m3x3._12), _13(m3x3._13), _14(T{0})
        , _21(m3x3._21), _22(m3x3._22), _23(m3x3._23), _24(T{0})
        , _31(m3x3._31), _32(m3x3._32), _33(m3x3._33), _34(T{0})
        , _41(T{0}),     _42(T{0}),     _43(T{0}),     _44(T{1})
    {
    }

    /**
     * Explicit constructor from a 3x3 matrix and a translation vector.
     */
    constexpr Matrix4x4(const Matrix3x3<T>& m3x3, const Vector3<T>& translation) noexcept
        : _11(m3x3._11), _12(m3x3._12), _13(m3x3._13), _14(translation.x)
        , _21(m3x3._21), _22(m3x3._22), _23(m3x3._23), _24(translation.y)
        , _31(m3x3._31), _32(m3x3._32), _33(m3x3._33), _34(translation.z)
        , _41(T{0}),     _42(T{0}),     _43(T{0}),     _44(T{1})
    {
    }

    /**
     * Explicit constructor from a 3x4 matrix.
     */
    constexpr explicit Matrix4x4(const Matrix3x4<T>& m3x4) noexcept
        : _11(m3x4._11), _12(m3x4._12), _13(m3x4._13), _14(m3x4._14)
        , _21(m3x4._21), _22(m3x4._22), _23(m3x4._23), _24(m3x4._24)
        , _31(m3x4._31), _32(m3x4._32), _33(m3x4._33), _34(m3x4._34)
        , _41(T{0}),     _42(T{0}),     _43(T{0}),     _44(T{1})
    {
    }

    /**
     * Swap operation.
     */
    void swap(Matrix4x4& other) noexcept
    {
        Matrix4x4 tmp(other);
        other = *this;
        *this = tmp;
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Tells whether this matrix represents an affine transformation, i.e.,
     * preserving points, straight lines, and planes. When used "correctly", only
     * (perspective) projection matrices should behave non-affinely (with the fourth
     * row altering the homogeneity of the coordinates).
     */
    bool is_affine() const
    {
        return
            _41 == T{0} &&
            _42 == T{0} &&
            _43 == T{0} &&
            _44 == T{1};
    }

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
        if (is_affine())
        {
            return
                + _11*_22*_33 + _12*_23*_31 + _13*_21*_32
                - _31*_22*_13 - _32*_23*_11 - _33*_21*_12;
        }

        // For the following calculations see:
        // http://www.geometrictools.com/Documentation/LaplaceExpansionTheorem.pdf
        const T s1 = _11*_22 - _12*_21;
        const T c1 = _33*_44 - _34*_43;
        const T s2 = _11*_23 - _13*_21;
        const T c2 = _32*_44 - _34*_42;
        const T s3 = _11*_24 - _14*_21;
        const T c3 = _32*_43 - _33*_42;
        const T s4 = _12*_23 - _13*_22;
        const T c4 = _31*_44 - _34*_41;
        const T s5 = _12*_24 - _14*_22;
        const T c5 = _31*_43 - _33*_41;
        const T s6 = _13*_24 - _14*_23;
        const T c6 = _31*_42 - _32*_41;

        return s1*c1 - s2*c2 + s3*c3 + s4*c4 - s5*c5 + s6*c6;
    }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this matrix transposed.
     */
    Matrix4x4 transposed() const
    {
        return {
            _11, _21, _31, _41,
            _12, _22, _32, _42,
            _13, _23, _33, _43,
            _14, _24, _34, _44,
        };
    }

    /**
     * Returns a copy of this matrix inverted (if possible).
     */
    Matrix4x4 inverted() const
    {
        if (is_affine())
        {
            return Matrix4x4{to_matrix3x4().inverted()};
        }

        // For the following calculations see:
        // http://www.geometrictools.com/Documentation/LaplaceExpansionTheorem.pdf
        const T s1 = _11*_22 - _12*_21;
        const T c1 = _33*_44 - _34*_43;
        const T s2 = _11*_23 - _13*_21;
        const T c2 = _32*_44 - _34*_42;
        const T s3 = _11*_24 - _14*_21;
        const T c3 = _32*_43 - _33*_42;
        const T s4 = _12*_23 - _13*_22;
        const T c4 = _31*_44 - _34*_41;
        const T s5 = _12*_24 - _14*_22;
        const T c5 = _31*_43 - _33*_41;
        const T s6 = _13*_24 - _14*_23;
        const T c6 = _31*_42 - _32*_41;

        const T det = s1*c1 - s2*c2 + s3*c3 + s4*c4 - s5*c5 + s6*c6;
        if (det == T{0})
            return ZERO;
        const T i = T{1} / det;
        return Matrix4x4{
            +_22*c1 - _23*c2 + _24*c3,   -_12*c1 + _13*c2 - _14*c3,   +_42*s6 - _43*s5 + _44*s4,   -_32*s6 + _33*s5 - _34*s4,
            -_21*c1 + _23*c4 - _24*c5,   +_11*c1 - _13*c4 + _14*c5,   -_41*s6 + _43*s3 - _44*s2,   +_31*s6 - _33*s3 + _34*s2,
            +_21*c2 - _22*c4 + _24*c6,   -_11*c2 + _12*c4 - _14*c6,   +_41*s5 - _42*s3 + _44*s1,   -_31*s5 + _32*s3 - _34*s1,
            -_21*c3 + _22*c5 - _23*c6,   +_11*c3 - _12*c5 + _13*c6,   -_41*s4 + _42*s2 - _43*s1,   +_31*s4 - _32*s2 + _33*s1,
        } * i;
    }

    /**
     * Returns the i-th (0-indexed) row vector of this matrix.
     */
    Vector4<T> get_row(unsigned i) const
    {
        assert(i < 4);
        return {
            m[i][0],
            m[i][1],
            m[i][2],
            m[i][3],
        };
    }

    /**
     * Returns the j-th (0-indexed) column vector of this matrix.
     */
    Vector4<T> get_column(unsigned j) const
    {
        assert(j < 4);
        return {
            m[0][j],
            m[1][j],
            m[2][j],
            m[3][j],
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
     * transformed coordinate system (i.e., the column vectors of the 3x4 part of
     * the matrix).
     */
    void to_axes(Vector3<T>& x, Vector3<T>& y, Vector3<T>& z, Vector3<T>& origin) const
    {
        x.x = _11;   y.x = _12;   z.x = _13;   origin.x = _14;
        x.y = _21;   y.y = _22;   z.y = _23;   origin.y = _24;
        x.z = _31;   y.z = _32;   z.z = _33;   origin.z = _34;
    }

    /**
     * Extracts the (column) vectors of this matrix.
     */
    void to_axes(Vector4<T>& x, Vector4<T>& y, Vector4<T>& z, Vector4<T>& w) const
    {
        x.x = _11;   y.x = _12;   z.x = _13;   w.x = _14;
        x.y = _21;   y.y = _22;   z.y = _23;   w.y = _24;
        x.z = _31;   y.z = _32;   z.z = _33;   w.z = _34;
        x.w = _41;   y.w = _42;   z.w = _43;   w.w = _44;
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
     * Returns the 3x4 part of this matrix.
     */
    Matrix3x4<T> to_matrix3x4() const
    {
        return {
            _11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
        };
    }

    /**
     * Decomposes the 3x4 part of this matrix into a 3x3 matrix and a translation
     * vector.
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
     * (positive) scalings in the "common" order (no projections, shears, negative
     * scalings, etc.).
     */
    bool decompose(Vector3<T>& scaling, Vector3<T>& axis, T& angle, Vector3<T>& translation) const
    {
        if (!is_affine())
            return false;

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
        if (!is_affine())
            return false;

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
        if (!is_affine())
            return false;

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
     * Assumes that this matrix is an orthogonal projection matrix for a left-handed
     * coordinate system and tries to extract the width and the height of the view
     * volume as well as the distances to the near and far clipping planes.
     */
    bool is_orthographic_lh(T& width, T& height, T& znear, T& zfar) const
    {
        if (_41 != T{0} || _42 != T{0} || _43 != T{0} || _44 != T{1})
            return false;

        if (_12 != T{0} || _13 != T{0} || _21 != T{0} || _23 != T{0} || _31 != T{0} || _32 != T{0})
            return false;
        if (_14 != T{0} || _24 != T{0})
            return false;

        if (_11 == T{0} || _22 == T{0} || _33 == T{0} || _34 == T{0})
            return false;

        width = T{2} / _11;
        height = T{2} / _22;
        znear = -_34 / _33;
        zfar = (T{1} - _34) / _33;
        return true;
    }

    /**
     * Assumes that this matrix is an orthogonal projection matrix for a right-handed
     * coordinate system and tries to extract the width and the height of the view
     * volume as well as the distances to the near and far clipping planes.
     */
    bool is_orthographic_rh(T& width, T& height, T& znear, T& zfar) const
    {
        if (_41 != T{0} || _42 != T{0} || _43 != T{0} || _44 != T{1})
            return false;

        if (_12 != T{0} || _13 != T{0} || _21 != T{0} || _23 != T{0} || _31 != T{0} || _32 != T{0})
            return false;
        if (_14 != T{0} || _24 != T{0})
            return false;

        if (_11 == T{0} || _22 == T{0} || _33 == T{0} || _34 == T{0})
            return false;

        width = T{2} / _11;
        height = T{2} / _22;
        znear = _34 / _33;
        zfar = (_34 - T{1}) / _33;
        return true;
    }

    /**
     * Assumes that this matrix in an orthogonal projection matrix (regardless of
     * whether it is left- or right-handed) and tries to extract the width and the
     * height of the view volume as well as the distances to the near and far
     * clipping planes.
     */
    bool is_orthographic(T& width, T& height, T& znear, T& zfar) const
    {
        if (_33 > T{0}) return is_orthographic_lh(width, height, znear, zfar);
        if (_33 < T{0}) return is_orthographic_rh(width, height, znear, zfar);
        return false;
    }

    /**
     * Assumes that this matrix is a perspective projection matrix for a left-handed
     * coordinate system and tries to extract the vertical field-of-view angle (in
     * radians), the aspect ratio of the frustum's width to its height, and the
     * distances to the near and far clipping planes.
     */
    bool is_perspective_lh(T& fov, T& aspect, T& znear, T& zfar) const
    {
        if (_41 != T{0} || _42 != T{0} || _43 != T{1} || _44 != T{0})
            return false;

        if (_12 != T{0} || _13 != T{0} || _21 != T{0} || _23 != T{0} || _31 != T{0} || _32 != T{0})
            return false;
        if (_14 != T{0} || _24 != T{0})
            return false;

        if (_11 == T{0} || _22 == T{0} || _33 == T{0} || _34 == T{0})
            return false;

        fov = T{2} * std::atan(T{1} / _22);
        aspect = _22 / _11;
        znear = -_34 / _33;
        zfar = _34 / (T{1} - _33);
        return true;
    }

    /**
     * Assumes that this matrix is a perspective projection matrix for a right-handed
     * coordinate system and tries to extract the vertical field-of-view angle (in
     * radians), the aspect ratio of the frustum's width to its height, and the
     * distances to the near and far clipping planes.
     */
    bool is_perspective_rh(T& fov, T& aspect, T& znear, T& zfar) const
    {
        if (_41 != T{0} || _42 != T{0} || _43 != T{-1} || _44 != T{0})
            return false;

        if (_12 != T{0} || _13 != T{0} || _21 != T{0} || _23 != T{0} || _31 != T{0} || _32 != T{0})
            return false;
        if (_14 != T{0} || _24 != T{0})
            return false;

        if (_11 == T{0} || _22 == T{0} || _33 == T{0} || _34 == T{0})
            return false;

        fov = T{2} * std::atan(T{1} / _22);
        aspect = _22 / _11;
        znear = _34 / _33;
        zfar = _34 / (T{1} + _33);
        return true;
    }

    /**
     * Assumes that this matrix is a perspective projection matrix (regardless of
     * whether it is left- or right-handed) and tries to extract the vertical
     * field-of-view angle (in radians), the aspect ratio of the frustum's width to
     * its height, and the distances to the near and far clipping planes.
     */
    bool is_perspective(T& fov, T& aspect, T& znear, T& zfar) const
    {
        if (_33 > T{0}) return is_perspective_lh(fov, aspect, znear, zfar);
        if (_33 < T{0}) return is_perspective_rh(fov, aspect, znear, zfar);
        return false;
    }

    /**
     * Assumes that this matrix is a projection matrix (whether orthogonal or
     * perspective, left- or right-handed) and tries to extract the distances to the
     * near and far clipping planes.
     */
    bool is_projection(T& znear, T& zfar) const
    {
        T foo;
        T bar;
        if (_43 == T{0} && _44 == T{1}) return is_orthographic(foo, bar, znear, zfar);
        if (_43 != T{0} && _44 == T{0}) return is_perspective(foo, bar, znear, zfar);
        return false;
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix.
     * Used to transform position vectors rather than direction vectors.
     */
    constexpr Vector4<T> transform(const Vector4<T>& v) const
    {
        return {
            _11 * v.x + _12 * v.y + _13 * v.z + _14 * v.w,
            _21 * v.x + _22 * v.y + _23 * v.z + _24 * v.w,
            _31 * v.x + _32 * v.y + _33 * v.z + _34 * v.w,
            _41 * v.x + _42 * v.y + _43 * v.z + _44 * v.w,
        };
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix.
     * Used to transform position vectors rather than direction vectors.
     */
    constexpr Vector3<T> transform(const Vector3<T>& v) const
    {
        const T w = _41 * v.x + _42 * v.y + _43 * v.z + _44;
        const T i = w != T{0} ? T{1} / w : T{1};
        return {
            (_11 * v.x + _12 * v.y + _13 * v.z + _14) * i,
            (_21 * v.x + _22 * v.y + _23 * v.z + _24) * i,
            (_31 * v.x + _32 * v.y + _33 * v.z + _34) * i,
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
    Vector4<T> transform_inverted(const Vector4<T>& v) const
    {
        return inverted().transform(v);
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix
     * inverted (if possible).
     * Used to transform position vectors rather than direction vectors.
     */
    Vector3<T> transform_inverted(const Vector3<T>& v) const
    {
        return inverted().transform(v);
    }

    /**
     * Returns a copy of the given (column) vector transformed by the 3x3 part of
     * this matrix inverted (if possible).
     * Used to transform direction vectors rather than position vectors.
     */
    Vector3<T> transform3x3_inverted(const Vector3<T>& v) const
    {
        return to_matrix3x3().transform_inverted(v);
    }



    // #############################################################################
    // Manipulations
    // #############################################################################

    /**
     * Transposes this matrix.
     */
    Matrix4x4& transpose()
    {
        std::swap(_12, _21);
        std::swap(_13, _31);
        std::swap(_14, _41);
        std::swap(_23, _32);
        std::swap(_24, _42);
        std::swap(_34, _43);
        return *this;
    }

    /**
     * Inverts this matrix (if possible).
     */
    Matrix4x4& invert()
    {
        *this = inverted();
        return *this;
    }

    /**
     * Sets the i-th (0-indexed) row vector of this matrix.
     */
    Matrix4x4& set_row(unsigned i, Vector4<T> v)
    {
        assert(i < 4);
        m[i][0] = v[0];
        m[i][1] = v[1];
        m[i][2] = v[2];
        m[i][3] = v[3];
        return *this;
    }

    /**
     * Sets the j-th (0-indexed) column vector of this matrix.
     */
    Matrix4x4& set_column(unsigned j, Vector4<T> v)
    {
        assert(j < 4);
        m[0][j] = v[0];
        m[1][j] = v[1];
        m[2][j] = v[2];
        m[3][j] = v[3];
        return *this;
    }



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const Matrix4x4& m) const noexcept { for (unsigned k = 0; k < 16; ++k) if (data[k] != m.data[k]) return false; return true; }
    bool operator!=(const Matrix4x4& m) const noexcept { for (unsigned k = 0; k < 16; ++k) if (data[k] != m.data[k]) return true; return false; }



    // #############################################################################
    // Arithmetic Operators
    // #############################################################################

    /** Returns a copy of this matrix unmodified. */
    constexpr Matrix4x4 operator+() const { return *this; }
    /** Returns a copy of this matrix with the signs of the elements flipped. */
    constexpr Matrix4x4 operator-() const { return {-_11, -_12, -_13, -_14, -_21, -_22, -_23, -_24, -_31, -_32, -_33, -_34, -_41, -_42, -_43, -_44}; }

    /** Returns the (element-wise) matrix sum of two matrices. */
    constexpr Matrix4x4 operator+(const Matrix4x4& m) const { return {_11 + m._11, _12 + m._12, _13 + m._13, _14 + m._14, _21 + m._21, _22 + m._22, _23 + m._23, _24 + m._24, _31 + m._31, _32 + m._32, _33 + m._33, _34 + m._34, _41 + m._41, _42 + m._42, _43 + m._43, _44 + m._44}; }
    /** Returns the (element-wise) matrix difference of two matrices. */
    constexpr Matrix4x4 operator-(const Matrix4x4& m) const { return {_11 - m._11, _12 - m._12, _13 - m._13, _14 - m._14, _21 - m._21, _22 - m._22, _23 - m._23, _24 - m._24, _31 - m._31, _32 - m._32, _33 - m._33, _34 - m._34, _41 - m._41, _42 - m._42, _43 - m._43, _44 - m._44}; }

    /** Returns a copy of this matrix "scaled" by the specified factor (scalar multiplication). */
    constexpr Matrix4x4 operator*(T s) const { return {_11 * s, _12 * s, _13 * s, _14 * s, _21 * s, _22 * s, _23 * s, _24 * s, _31 * s, _32 * s, _33 * s, _34 * s, _41 * s, _42 * s, _43 * s, _44 * s}; }
    /** Returns a copy of this matrix inversely "scaled" by the specified factor (scalar division). */
    constexpr Matrix4x4 operator/(T s) const { return {_11 / s, _12 / s, _13 / s, _14 / s, _21 / s, _22 / s, _23 / s, _24 / s, _31 / s, _32 / s, _33 / s, _34 / s, _41 / s, _42 / s, _43 / s, _44 / s}; }

    /** Returns the matrix product of two matrices (matrix multiplication). */
    constexpr Matrix4x4 operator*(const Matrix4x4& m) const
    {
        return {
            _11*m._11 + _12*m._21 + _13*m._31 + _14*m._41,   _11*m._12 + _12*m._22 + _13*m._32 + _14*m._42,   _11*m._13 + _12*m._23 + _13*m._33 + _14*m._43,   _11*m._14 + _12*m._24 + _13*m._34 + _14*m._44,
            _21*m._11 + _22*m._21 + _23*m._31 + _24*m._41,   _21*m._12 + _22*m._22 + _23*m._32 + _24*m._42,   _21*m._13 + _22*m._23 + _23*m._33 + _24*m._43,   _21*m._14 + _22*m._24 + _23*m._34 + _24*m._44,
            _31*m._11 + _32*m._21 + _33*m._31 + _34*m._41,   _31*m._12 + _32*m._22 + _33*m._32 + _34*m._42,   _31*m._13 + _32*m._23 + _33*m._33 + _34*m._43,   _31*m._14 + _32*m._24 + _33*m._34 + _34*m._44,
            _41*m._11 + _42*m._21 + _43*m._31 + _44*m._41,   _41*m._12 + _42*m._22 + _43*m._32 + _44*m._42,   _41*m._13 + _42*m._23 + _43*m._33 + _44*m._43,   _41*m._14 + _42*m._24 + _43*m._34 + _44*m._44,
        };
    }

    /** Returns a copy of the given (column vector) transformed by this matrix. */
    constexpr Vector4<T> operator*(const Vector4<T>& v) const { return transform(v); }

    /** Returns a copy of the given (column vector) transformed by this matrix. */
    constexpr Vector3<T> operator*(const Vector3<T>& v) const { return transform(v); }

    /** "Scales" a matrix by the specified factor (scalar multiplication). */
    friend constexpr Matrix4x4 operator*(T s, const Matrix4x4& m) { return m * s; }



    // #############################################################################
    // Arithmetic Assignment Operators
    // #############################################################################

    /** Adds the given matrix to this one, resulting in the (element-wise) matrix sum. */
    constexpr Matrix4x4& operator+=(const Matrix4x4& m) { for (unsigned k = 0; k < 16; ++k) data[k] += m.data[k]; return *this; }
    /** Subtracts the given matrix from this one, resulting in the (element-wise) matrix difference. */
    constexpr Matrix4x4& operator-=(const Matrix4x4& m) { for (unsigned k = 0; k < 16; ++k) data[k] -= m.data[k]; return *this; }

    /** "Scales" this matrix by the specified factor (scalar multiplication). */
    constexpr Matrix4x4& operator*=(T s) { for (T& k : data) k *= s; return *this; }
    /** Inversely "scales" this matrix by the specified factor (scalar division). */
    constexpr Matrix4x4& operator/=(T s) { for (T& k : data) k /= s; return *this; }



    // #############################################################################
    // Access Operators
    // #############################################################################

    std::span<const T, 4> operator[](unsigned i) const { assert(i < 4); return std::span<const T, 4>{m[i], 4}; }
    std::span<T, 4> operator[](unsigned i) { assert(i < 4); return std::span<T, 4>{m[i], 4}; }



    // #############################################################################
    // Static Functions
    // #############################################################################

    /**
     * Initializes an affine transformation matrix from the specified basis vectors
     * that define the transformed coordinate system (as the column vectors of the
     * 3x4 part of the matrix).
     */
    static constexpr Matrix4x4 from_axes(const Vector3<T>& x, const Vector3<T>& y, const Vector3<T>& z)
    {
        return {
            x.x,  y.x,  z.x,  T{0},
            x.y,  y.y,  z.y,  T{0},
            x.z,  y.z,  z.z,  T{0},
            T{0}, T{0}, T{0}, T{1},
        };
    }

    /**
     * Initializes an affine transformation matrix from the specified basis vectors
     * and origin/translation vector that define the transformed coordinate system
     * (as the column vectors of the 3x4 part of the matrix).
     */
    static constexpr Matrix4x4 from_axes(const Vector3<T>& x, const Vector3<T>& y, const Vector3<T>& z, const Vector3<T>& origin)
    {
        return {
            x.x,  y.x,  z.x,  origin.x,
            x.y,  y.y,  z.y,  origin.y,
            x.z,  y.z,  z.z,  origin.z,
            T{0}, T{0}, T{0}, T{1},
        };
    }

    /**
     * Initializes a transformation matrix from the specified (column) vectors.
     */
    static constexpr Matrix4x4 from_axes(const Vector4<T>& x, const Vector4<T>& y, const Vector4<T>& z, const Vector4<T>& w)
    {
        return {
            x.x,  y.x,  z.x,  w.x,
            x.y,  y.y,  z.y,  w.y,
            x.z,  y.z,  z.z,  w.z,
            x.w,  y.w,  z.w,  w.w,
        };
    }

    /**
     * Initializes a scaling matrix with scaling factor s.
     */
    static constexpr Matrix4x4 scaling(T s)
    {
        return {
            s,    T{0}, T{0}, T{0},
            T{0}, s,    T{0}, T{0},
            T{0}, T{0}, s,    T{0},
            T{0}, T{0}, T{0}, T{1},
        };
    }

    /**
     * Initializes a scaling matrix with scaling vector s.
     */
    static constexpr Matrix4x4 scaling(const Vector3<T>& s)
    {
        return {
            s.x,  T{0}, T{0}, T{0},
            T{0}, s.y,  T{0}, T{0},
            T{0}, T{0}, s.z,  T{0},
            T{0}, T{0}, T{0}, T{1},
        };
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around the x-axis.
     */
    static Matrix4x4 rotx(T angle)
    {
        return rotx(std::sin(angle), std::cos(angle));
    }

    /**
     * Initializes a rotation matrix representing a rotation around the x-axis based
     * on ready-made sine and cosine values.
     */
    static Matrix4x4 rotx(T sin, T cos)
    {
        return {
            T{1}, T{0}, T{0}, T{0},
            T{0},  cos, -sin, T{0},
            T{0},  sin,  cos, T{0},
            T{0}, T{0}, T{0}, T{1},
        };
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around the y-axis.
     */
    static Matrix4x4 roty(T angle)
    {
        return roty(std::sin(angle), std::cos(angle));
    }

    /**
     * Initializes a rotation matrix representing a rotation around the y-axis based
     * on ready-made sine and cosine values.
     */
    static Matrix4x4 roty(T sin, T cos)
    {
        return {
             cos, T{0},  sin, T{0},
            T{0}, T{1}, T{0}, T{0},
            -sin, T{0},  cos, T{0},
            T{0}, T{0}, T{0}, T{1},
        };
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around the z-axis.
     */
    static Matrix4x4 rotz(T angle)
    {
        return rotz(std::sin(angle), std::cos(angle));
    }

    /**
     * Initializes a rotation matrix representing a rotation around the z-axis based
     * on ready-made sine and cosine values.
     */
    static Matrix4x4 rotz(T sin, T cos)
    {
        return {
             cos, -sin, T{0}, T{0},
             sin,  cos, T{0}, T{0},
            T{0}, T{0}, T{1}, T{0},
            T{0}, T{0}, T{0}, T{1},
        };
    }

    /**
     * Initializes a rotation matrix representing a rotation around the specified
     * axis, with an angle equal to the magnitude of the specified vector (in
     * radians).
     */
    static Matrix4x4 rotation(const Vector3<T>& axis_angle)
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
    static Matrix4x4 rotation(const Vector3<T>& axis, T angle)
    {
        if (angle == T{0})
            return IDENTITY;
        return rotation_normalized(axis.normalized(), angle);
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around an axis specified as a normalized unit vector.
     */
    static Matrix4x4 rotation_normalized(const Vector3<T>& unit_axis, T angle)
    {
        return Matrix4x4{Matrix3x3<T>::rotation_normalized(unit_axis, angle)};
    }

    /**
     * Initializes a translation matrix from the specified translation vector.
     */
    static Matrix4x4 translation(const Vector3<T>& translation)
    {
        return {
            T{1}, T{0}, T{0}, translation.x,
            T{0}, T{1}, T{0}, translation.y,
            T{0}, T{0}, T{1}, translation.z,
            T{0}, T{0}, T{0}, T{1},
        };
    }

    /**
     * Initializes a transformation matrix from the specified scaling factor, a
     * rotation axis, a rotation angle (in radians), and a translation vector.
     */
    static Matrix4x4 compose(T scaling, const Vector3<T>& axis, T angle, const Vector3<T>& translation)
    {
        return compose({scaling, scaling, scaling}, axis, angle, translation);
    }

    /**
     * Initializes a transformation matrix from the specified scaling vector, a
     * rotation axis, a rotation angle (in radians), and a translation vector.
     */
    static Matrix4x4 compose(const Vector3<T>& scaling, const Vector3<T>& axis, T angle, const Vector3<T>& translation)
    {
        return Matrix4x4{Matrix3x4<T>::compose(scaling, axis, angle, translation)};
    }

    /**
     * Initializes a view transformation matrix for a left-handed coordinate system
     * using a camera position, a focal point, and an "up" direction.
     */
    static Matrix4x4 look_at_lh(const Vector3<T>& position, const Vector3<T>& focus, const Vector3<T>& up)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatlh
        return look_to_lh(position, focus - position, up);
    }

    /**
     * Initializes a view transformation matrix for a right-handed coordinate system
     * using a camera position, a focal point, and an "up" direction.
     */
    static Matrix4x4 look_at_rh(const Vector3<T>& position, const Vector3<T>& focus, const Vector3<T>& up)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatrh
        return look_to_rh(position, focus - position, up);
    }

    /**
     * Initializes a view transformation matrix for a left-handed coordinate system
     * using a camera position, a camera "look" direction, and an "up" direction.
     */
    static Matrix4x4 look_to_lh(const Vector3<T>& position, const Vector3<T>& look, const Vector3<T>& up)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatlh
        const Vector3 z{look.normalized()};
        const Vector3 x{up.cross(z).normalized()};
        const Vector3 y{z.cross(x)};
        return {
            x.x,  x.y,  x.z,  -x.dot(position),
            y.x,  y.y,  y.z,  -y.dot(position),
            z.x,  z.y,  z.z,  -z.dot(position),
            T{0}, T{0}, T{0}, T{1},
        };
    }

    /**
     * Initializes a view transformation matrix for a right-handed coordinate system
     * using a camera position, a camera "look" direction, and an "up" direction.
     */
    static Matrix4x4 look_to_rh(const Vector3<T>& position, const Vector3<T>& look, const Vector3<T>& up)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatrh
        return look_to_lh(position, -look, up);
    }

    /**
     * Initializes an orthogonal projection matrix for a left-handed coordinate
     * system using a width and a height of the view volume as well as distances to
     * the near and far clipping planes.
     */
    static Matrix4x4 orthographic_lh(T width, T height, T znear, T zfar)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixortholh
        const T rd = T{1} / (zfar - znear);
        const T sy = T{2} / height;
        const T sx = T{2} / width;
        return {
            sx,    T{0},  T{0},  T{0},
            T{0},  sy,    T{0},  T{0},
            T{0},  T{0},  rd,    -znear * rd,
            T{0},  T{0},  T{0},  T{1},
        };
    }

    /**
     * Initializes an orthogonal projection matrix for a right-handed coordinate
     * system using a width and a height of the view volume as well as distances to
     * the near and far clipping planes.
     */
    static Matrix4x4 orthographic_rh(T width, T height, T znear, T zfar)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixorthorh
        return orthographic_lh(width, height, -znear, -zfar);
    }

    /**
     * Initializes a perspective projection matrix for a left-handed coordinate
     * system using a vertical field-of-view angle (in radians), an aspect ratio of
     * the frustum's width to its height, and distances to the near and far clipping
     * planes.
     */
    static Matrix4x4 perspective_lh(T fov, T aspect, T znear, T zfar)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovlh
        const T rd = zfar / (zfar - znear);
        const T sy = T{1} / std::tan(fov * T{0.5}); // cot = 1/tan
        const T sx = sy / aspect;
        return {
            sx,    T{0},  T{0},  T{0},
            T{0},  sy,    T{0},  T{0},
            T{0},  T{0},  rd,    -znear * rd,
            T{0},  T{0},  T{1},  T{0},
        };
    }

    /**
     * Initializes a perspective projection matrix for a right-handed coordinate
     * system using a vertical field-of-view angle (in radians), an aspect ratio of
     * the frustum's width to its height, and distances to the near and far clipping
     * planes.
     */
    static Matrix4x4 perspective_rh(T fov, T aspect, T znear, T zfar)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovrh
        const T rd = zfar / (znear - zfar);
        const T sy = T{1} / std::tan(fov * T{0.5}); // cot = 1/tan
        const T sx = sy / aspect;
        return {
            sx,    T{0},  T{0},  T{0},
            T{0},  sy,    T{0},  T{0},
            T{0},  T{0},  rd,    znear * rd,
            T{0},  T{0},  T{-1}, T{0},
        };
    }

    /**
     * Initializes a color transformation matrix using a luminance vector that
     * specifies the weighted sum of the r, g, and b color components, allowing
     * to customize how each component contributes to the (grayscale) result.
     */
    static Matrix4x4 luminance(const Vector3<T>& l)
    {
        // http://www.graficaobscura.com/matrix/index.html
        return {
            l.x,  l.y,  l.z,  T{0},
            l.x,  l.y,  l.z,  T{0},
            l.x,  l.y,  l.z,  T{0},
            T{0}, T{0}, T{0}, T{1},
        };
    }

    /**
     * Initializes a color transformation matrix using a saturation vector that
     * specifies the respective saturation for the r, g, and b color components.
     * The grayscale weights for the r, g, and b color components are given by the
     * luminance vector l = (0.3086, 0.6094, 0.0820) to maintain the brightness.
     * These values are better for linear RGB colors. The NTSC weights of 0.299,
     * 0.587, and 0.114 are only applicable to RGB colors in a gamma 2.2 color space.
     * Interesting: This can also be used to complement the color by specifying a
     * saturation of -1.
     * Note: When s is set to 0, the matrix becomes a "convert to luminance" matrix
     * transforming the color to grayscale according to the luminance vector.
     * When s is set to 1, the matrix becomes the identity matrix leaving the color
     * unchanged.
     */
    static Matrix4x4 saturation(const Vector3<T>& s)
    {
        return saturation(s, {T{0.3086}, T{0.6094}, T{0.0820}});
    }

    /**
     * Initializes a color transformation matrix using a saturation vector that
     * specifies the respective saturation for the r, g, and b color components.
     * The grayscale weights for the r, g, and b color components are given by the
     * specified luminance vector l to maintain the perceived brightness.
     * Interesting: This can also be used to complement the color by specifying a
     * saturation of -1.
     * Note: When s is set to 0, the matrix becomes a "convert to luminance" matrix
     * transforming the color to grayscale according to the luminance vector.
     * When s is set to 1, the matrix becomes the identity matrix leaving the color
     * unchanged.
     */
    static Matrix4x4 saturation(const Vector3<T>& s, const Vector3<T>& l)
    {
        // http://www.graficaobscura.com/matrix/index.html
        return {
            (T{1}-s.x)*l.x+s.x, (T{1}-s.x)*l.y,     (T{1}-s.x)*l.z,     T{0},
            (T{1}-s.y)*l.x,     (T{1}-s.y)*l.y+s.y, (T{1}-s.y)*l.z,     T{0},
            (T{1}-s.z)*l.x,     (T{1}-s.z)*l.y,     (T{1}-s.z)*l.z+s.z, T{0},
            T{0},               T{0},               T{0},               T{1},
        };
    }

    /**
     * Initializes a color transformation matrix using a contrast vector that
     * specifies the respective contrast for the r, g, and b color components.
     * Interesting: This can also be used to alter the color balance.
     * Note: When c is set to 0, the resulting "color" is a constant gray with all
     * three color components set to 0.5. When c is set to 1, the matrix becomes the
     * identity matrix leaving the color unchanged.
     */
    static Matrix4x4 contrast(const Vector3<T>& c)
    {
        // http://www.graficaobscura.com/matrix/index.html
        // https://docs.rainmeter.net/tips/colormatrix-guide
        return {
            c.x,   T{0},  T{0},  (T{1}-c.x)*T{0.5},
            T{0},  c.y,   T{0},  (T{1}-c.y)*T{0.5},
            T{0},  T{0},  c.z,   (T{1}-c.z)*T{0.5},
            T{0},  T{0},  T{0},  T{1},
        };
    }

    /**
     * Initializes a color transformation matrix using a rotation angle (in radians)
     * that specifies how much to rotate the r, g, and b color components.
     * The grayscale weights for the r, g, and b color components are given by the
     * luminance vector l = (0.3086, 0.6094, 0.0820) to maintain the brightness.
     * These values are better for linear RGB colors. The NTSC weights of 0.299,
     * 0.587, and 0.114 are only applicable to RGB colors in a gamma 2.2 color space.
     */
    static Matrix4x4 hue(T r)
    {
        return hue(r, {T{0.3086}, T{0.6094}, T{0.0820}});
    }

    /**
     * Initializes a color transformation matrix using a rotation angle (in radians)
     * that specifies how much to rotate the r, g, and b color components.
     * The grayscale weights for the r, g, and b color components are given by the
     * specified luminance vector l to maintain the perceived brightness.
     */
    static Matrix4x4 hue(T r, const Vector3<T>& l)
    {
        // http://www.graficaobscura.com/matrix/index.html
        // http://beesbuzz.biz/code/hsv_color_transforms.php
        // http://en.wikipedia.org/wiki/YIQ
        const Matrix4x4 rgb2yiq{
            l.x,          l.y,           l.z,           T{0},
            T{0.595716},  T{-0.274453},  T{-0.321263},  T{0},
            T{0.211456},  T{-0.522591},  T{0.311135},   T{0},
            T{0},         T{0},          T{0},          T{1},
        };

        const Matrix4x4 yiq2rgb = rgb2yiq.inverted();

        const Matrix4x4 hue_rot = Matrix4x4::rotx(r);

        return yiq2rgb * hue_rot * rgb2yiq;
    }

}; // struct Matrix4x4



    template <std::floating_point T>
    const Matrix4x4<T> Matrix4x4<T>::ZERO =       {T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}, T{0}};
    template <std::floating_point T>
    const Matrix4x4<T> Matrix4x4<T>::IDENTITY =   {T{1}, T{0}, T{0}, T{0}, T{0}, T{1}, T{0}, T{0}, T{0}, T{0}, T{1}, T{0}, T{0}, T{0}, T{0}, T{1}};



using Matrix4x4f = Matrix4x4<float>;
using Matrix4x4d = Matrix4x4<double>;
using Matrix4x4ld = Matrix4x4<long double>;

//extern template class Matrix4x4<float>;
//extern template class Matrix4x4<double>;



} // namespace ml7

#endif // ML7_MATRIX4X4_H
