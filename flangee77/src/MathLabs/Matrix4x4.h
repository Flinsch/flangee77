#ifndef ML7_MATRIX4X4_H
#define ML7_MATRIX4X4_H

#include "./Matrix3x4.h"
#include "./Matrix3x3.h"
#include "./Vector4.h"
#include "./Vector3.h"

#include <span>



namespace ml7 {



struct Matrix4x4
{

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
            /** Element in the 4th row and 1st column. */
            float _41;
            /** Element in the 4th row and 2nd column. */
            float _42;
            /** Element in the 4th row and 3rd column. */
            float _43;
            /** Element in the 4th row and 4th column. */
            float _44;
            // NOLINTEND(*-use-default-member-init)
        }; // struct

        /** Array of the elements. */
        float data[16];

        /** 2D array of the elements. */
        float m[4][4];
    }; // union



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes an identity matrix representing a neutral
     * transformation.
     */
    constexpr Matrix4x4() noexcept
        : _11(1.0f), _12(0.0f), _13(0.0f), _14(0.0f)
        , _21(0.0f), _22(1.0f), _23(0.0f), _24(0.0f)
        , _31(0.0f), _32(0.0f), _33(1.0f), _34(0.0f)
        , _41(0.0f), _42(0.0f), _43(0.0f), _44(1.0f)
    {
    }

    /**
     * Explicit constructor with parameters for each element.
     */
    constexpr Matrix4x4(
        float _11, float _12, float _13, float _14,
        float _21, float _22, float _23, float _24,
        float _31, float _32, float _33, float _34,
        float _41, float _42, float _43, float _44) noexcept
        : _11(_11), _12(_12), _13(_13), _14(_14)
        , _21(_21), _22(_22), _23(_23), _24(_24)
        , _31(_31), _32(_32), _33(_33), _34(_34)
        , _41(_41), _42(_42), _43(_43), _44(_44)
    {
    }

    /**
     * Explicit constructor from a 3x3 matrix.
     */
    constexpr explicit Matrix4x4(const Matrix3x3& m3x3) noexcept
        : _11(m3x3._11), _12(m3x3._12), _13(m3x3._13), _14(0.0f)
        , _21(m3x3._21), _22(m3x3._22), _23(m3x3._23), _24(0.0f)
        , _31(m3x3._31), _32(m3x3._32), _33(m3x3._33), _34(0.0f)
        , _41(0.0f),     _42(0.0f),     _43(0.0f),     _44(1.0f)
    {
    }

    /**
     * Explicit constructor from a 3x3 matrix and a translation vector.
     */
    constexpr Matrix4x4(const Matrix3x3& m3x3, const ml7::Vector3& translation) noexcept
        : _11(m3x3._11), _12(m3x3._12), _13(m3x3._13), _14(translation.x)
        , _21(m3x3._21), _22(m3x3._22), _23(m3x3._23), _24(translation.y)
        , _31(m3x3._31), _32(m3x3._32), _33(m3x3._33), _34(translation.z)
        , _41(0.0f),     _42(0.0f),     _43(0.0f),     _44(1.0f)
    {
    }

    /**
     * Explicit constructor from a 3x4 matrix.
     */
    constexpr explicit Matrix4x4(const Matrix3x4& m3x4) noexcept
        : _11(m3x4._11), _12(m3x4._12), _13(m3x4._13), _14(m3x4._14)
        , _21(m3x4._21), _22(m3x4._22), _23(m3x4._23), _24(m3x4._24)
        , _31(m3x4._31), _32(m3x4._32), _33(m3x4._33), _34(m3x4._34)
        , _41(0.0f),     _42(0.0f),     _43(0.0f),     _44(1.0f)
    {
    }

    /**
     * Swap operation.
     */
    void swap(Matrix4x4& other) noexcept;



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
            _41 == 0.0f &&
            _42 == 0.0f &&
            _43 == 0.0f &&
            _44 == 1.0f;
    }

    /**
     * Tells whether this matrix is invertible (i.e., whether its determinant is
     * non-zero).
     */
    bool is_invertible() const { return determinant() != 0.0f; }

    /**
     * Returns the determinant of the matrix.
     */
    float determinant() const;



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
    Matrix4x4 inverted() const;

    /**
     * Returns the i-th (0-indexed) row vector of this matrix.
     */
    Vector4 get_row(unsigned i) const
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
    Vector4 get_column(unsigned j) const
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
    void to_axes(ml7::Vector3& x, ml7::Vector3& y, ml7::Vector3& z) const
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
    void to_axes(ml7::Vector3& x, ml7::Vector3& y, ml7::Vector3& z, ml7::Vector3& origin) const
    {
        x.x = _11;   y.x = _12;   z.x = _13;   origin.x = _14;
        x.y = _21;   y.y = _22;   z.y = _23;   origin.y = _24;
        x.z = _31;   y.z = _32;   z.z = _33;   origin.z = _34;
    }

    /**
     * Extracts the (column) vectors of this matrix.
     */
    void to_axes(ml7::Vector4& x, ml7::Vector4& y, ml7::Vector4& z, ml7::Vector4& w) const
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
    bool to_axis_angle(ml7::Vector3& axis, float& angle) const;

    /**
     * Returns the 3x3 part of this matrix.
     */
    Matrix3x3 to_matrix3x3() const
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
    Matrix3x4 to_matrix3x4() const
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
    void decompose(ml7::Matrix3x3& m3x3, ml7::Vector3& translation) const
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
    bool decompose(ml7::Vector3& scaling, ml7::Vector3& axis, float& angle, ml7::Vector3& translation) const;

    /**
     * Assumes that this matrix is a view transformation matrix for a left-handed
     * coordinate system and tries to extract the camera position, the camera "look"
     * direction, and the "up" direction.
     */
    bool is_look_lh(ml7::Vector3& position, ml7::Vector3& look, ml7::Vector3& up) const;

    /**
     * Assumes that this matrix is a view transformation matrix for a right-handed
     * coordinate system and tries to extract the camera position, the camera "look"
     * direction, and the "up" direction.
     */
    bool is_look_rh(ml7::Vector3& position, ml7::Vector3& look, ml7::Vector3& up) const;

    /**
     * Assumes that this matrix is an orthogonal projection matrix for a left-handed
     * coordinate system and tries to extract the width and the height of the view
     * volume as well as the distances to the near and far clipping planes.
     */
    bool is_orthographic_lh(float& width, float& height, float& znear, float& zfar) const;

    /**
     * Assumes that this matrix is an orthogonal projection matrix for a right-handed
     * coordinate system and tries to extract the width and the height of the view
     * volume as well as the distances to the near and far clipping planes.
     */
    bool is_orthographic_rh(float& width, float& height, float& znear, float& zfar) const;

    /**
     * Assumes that this matrix in an orthogonal projection matrix (regardless of
     * whether it is left- or right-handed) and tries to extract the width and the
     * height of the view volume as well as the distances to the near and far
     * clipping planes.
     */
    bool is_orthographic(float& width, float& height, float& znear, float& zfar) const;

    /**
     * Assumes that this matrix is a perspective projection matrix for a left-handed
     * coordinate system and tries to extract the vertical field-of-view angle (in
     * radians), the aspect ratio of the frustum's width to its height, and the
     * distances to the near and far clipping planes.
     */
    bool is_perspective_lh(float& fov, float& aspect, float& znear, float& zfar) const;

    /**
     * Assumes that this matrix is a perspective projection matrix for a right-handed
     * coordinate system and tries to extract the vertical field-of-view angle (in
     * radians), the aspect ratio of the frustum's width to its height, and the
     * distances to the near and far clipping planes.
     */
    bool is_perspective_rh(float& fov, float& aspect, float& znear, float& zfar) const;

    /**
     * Assumes that this matrix is a perspective projection matrix (regardless of
     * whether it is left- or right-handed) and tries to extract the vertical
     * field-of-view angle (in radians), the aspect ratio of the frustum's width to
     * its height, and the distances to the near and far clipping planes.
     */
    bool is_perspective(float& fov, float& aspect, float& znear, float& zfar) const;

    /**
     * Assumes that this matrix is a projection matrix (whether orthogonal or
     * perspective, left- or right-handed) and tries to extract the distances to the
     * near and far clipping planes.
     */
    bool is_projection(float& znear, float& zfar) const;

    /**
     * Returns a copy of the given (column) vector transformed by this matrix.
     * Used to transform position vectors rather than direction vectors.
     */
    constexpr Vector4 transform(const Vector4& v) const
    {
        return {
            _11*v.x + _12*v.y + _13*v.z + _14*v.w,
            _21*v.x + _22*v.y + _23*v.z + _24*v.w,
            _31*v.x + _32*v.y + _33*v.z + _34*v.w,
            _41*v.x + _42*v.y + _43*v.z + _44*v.w,
        };
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix.
     * Used to transform position vectors rather than direction vectors.
     */
    constexpr Vector3 transform(const Vector3& v) const
    {
        const float w = _41*v.x + _42*v.y + _43*v.z + _44;
        const float i = w != 0.0f ? 1.0f / w : 1.0f;
        return {
            (_11*v.x + _12*v.y + _13*v.z + _14)*i,
            (_21*v.x + _22*v.y + _23*v.z + _24)*i,
            (_31*v.x + _32*v.y + _33*v.z + _34)*i,
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
     * Returns a copy of the given (column) vector transformed by this matrix
     * inverted (if possible).
     * Used to transform position vectors rather than direction vectors.
     */
    Vector4 transform_inverted(const Vector4& v) const;

    /**
     * Returns a copy of the given (column) vector transformed by this matrix
     * inverted (if possible).
     * Used to transform position vectors rather than direction vectors.
     */
    Vector3 transform_inverted(const Vector3& v) const;

    /**
     * Returns a copy of the given (column) vector transformed by the 3x3 part of
     * this matrix inverted (if possible).
     * Used to transform direction vectors rather than position vectors.
     */
    Vector3 transform3x3_inverted(const Vector3& v) const;



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
    Matrix4x4& set_row(unsigned i, Vector4 v)
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
    Matrix4x4& set_column(unsigned j, Vector4 v)
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
    constexpr Matrix4x4 operator*(float s) const { return {_11 * s, _12 * s, _13 * s, _14 * s, _21 * s, _22 * s, _23 * s, _24 * s, _31 * s, _32 * s, _33 * s, _34 * s, _41 * s, _42 * s, _43 * s, _44 * s}; }
    /** Returns a copy of this matrix inversely "scaled" by the specified factor (scalar division). */
    constexpr Matrix4x4 operator/(float s) const { return {_11 / s, _12 / s, _13 / s, _14 / s, _21 / s, _22 / s, _23 / s, _24 / s, _31 / s, _32 / s, _33 / s, _34 / s, _41 / s, _42 / s, _43 / s, _44 / s}; }

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
    constexpr Vector4 operator*(const Vector4& v) const { return transform(v); }

    /** Returns a copy of the given (column vector) transformed by this matrix. */
    constexpr Vector3 operator*(const Vector3& v) const { return transform(v); }

    /** "Scales" a matrix by the specified factor (scalar multiplication). */
    friend constexpr Matrix4x4 operator*(float s, const Matrix4x4& m) { return m * s; }



    // #############################################################################
    // Arithmetic Assignment Operators
    // #############################################################################

    /** Adds the given matrix to this one, resulting in the (element-wise) matrix sum. */
    constexpr Matrix4x4& operator+=(const Matrix4x4& m) { for (unsigned k = 0; k < 16; ++k) data[k] += m.data[k]; return *this; }
    /** Subtracts the given matrix from this one, resulting in the (element-wise) matrix difference. */
    constexpr Matrix4x4& operator-=(const Matrix4x4& m) { for (unsigned k = 0; k < 16; ++k) data[k] -= m.data[k]; return *this; }

    /** "Scales" this matrix by the specified factor (scalar multiplication). */
    constexpr Matrix4x4& operator*=(float s) { for (float& k : data) k *= s; return *this; }
    /** Inversely "scales" this matrix by the specified factor (scalar division). */
    constexpr Matrix4x4& operator/=(float s) { for (float& k : data) k /= s; return *this; }



    // #############################################################################
    // Access Operators
    // #############################################################################

    std::span<const float, 4> operator[](unsigned i) const { assert(i < 4); return std::span<const float, 4>{m[i], 4}; }
    std::span<float, 4> operator[](unsigned i) { assert(i < 4); return std::span<float, 4>{m[i], 4}; }



    // #############################################################################
    // Static Functions
    // #############################################################################

    /**
     * Initializes an affine transformation matrix from the specified basis vectors
     * that define the transformed coordinate system (as the column vectors of the
     * 3x4 part of the matrix).
     */
    static constexpr Matrix4x4 from_axes(const ml7::Vector3& x, const ml7::Vector3& y, const ml7::Vector3& z)
    {
        return {
            x.x,  y.x,  z.x,  0.0f,
            x.y,  y.y,  z.y,  0.0f,
            x.z,  y.z,  z.z,  0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
    }

    /**
     * Initializes an affine transformation matrix from the specified basis vectors
     * and origin/translation vector that define the transformed coordinate system
     * (as the column vectors of the 3x4 part of the matrix).
     */
    static constexpr Matrix4x4 from_axes(const ml7::Vector3& x, const ml7::Vector3& y, const ml7::Vector3& z, const ml7::Vector3& origin)
    {
        return {
            x.x,  y.x,  z.x,  origin.x,
            x.y,  y.y,  z.y,  origin.y,
            x.z,  y.z,  z.z,  origin.z,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
    }

    /**
     * Initializes a transformation matrix from the specified (column) vectors.
     */
    static constexpr Matrix4x4 from_axes(const ml7::Vector4& x, const ml7::Vector4& y, const ml7::Vector4& z, const ml7::Vector4& w)
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
    static constexpr Matrix4x4 scaling(float s)
    {
        return {
            s,    0.0f, 0.0f, 0.0f,
            0.0f, s,    0.0f, 0.0f,
            0.0f, 0.0f, s,    0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
    }

    /**
     * Initializes a scaling matrix with scaling vector s.
     */
    static constexpr Matrix4x4 scaling(const ml7::Vector3& s)
    {
        return {
            s.x,  0.0f, 0.0f, 0.0f,
            0.0f, s.y,  0.0f, 0.0f,
            0.0f, 0.0f, s.z,  0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around the x-axis.
     */
    static Matrix4x4 rotx(float angle) { return rotx(::sinf(angle), ::cosf(angle)); }

    /**
     * Initializes a rotation matrix representing a rotation around the x-axis based
     * on ready-made sine and cosine values.
     */
    static Matrix4x4 rotx(float sin, float cos)
    {
        return {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f,  cos, -sin, 0.0f,
            0.0f,  sin,  cos, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around the y-axis.
     */
    static Matrix4x4 roty(float angle) { return roty(::sinf(angle), ::cosf(angle)); }

    /**
     * Initializes a rotation matrix representing a rotation around the y-axis based
     * on ready-made sine and cosine values.
     */
    static Matrix4x4 roty(float sin, float cos)
    {
        return {
             cos, 0.0f,  sin, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -sin, 0.0f,  cos, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around the z-axis.
     */
    static Matrix4x4 rotz(float angle) { return rotz(::sinf(angle), ::cosf(angle)); }

    /**
     * Initializes a rotation matrix representing a rotation around the z-axis based
     * on ready-made sine and cosine values.
     */
    static Matrix4x4 rotz(float sin, float cos)
    {
        return {
             cos, -sin, 0.0f, 0.0f,
             sin,  cos, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
    }

    /**
     * Initializes a rotation matrix representing a rotation around the specified
     * axis, with an angle equal to the magnitude of the specified vector (in
     * radians).
     */
    static Matrix4x4 rotation(const ml7::Vector3& axis_angle)
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
    static Matrix4x4 rotation(const ml7::Vector3& axis, float angle)
    {
        if (angle == 0.0f)
            return IDENTITY;
        return rotation_normalized(axis.normalized(), angle);
    }

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around an axis specified as a normalized unit vector.
     */
    static Matrix4x4 rotation_normalized(const ml7::Vector3& unit_axis, float angle);

    /**
     * Initializes a translation matrix from the specified translation vector.
     */
    static Matrix4x4 translation(const ml7::Vector3& translation)
    {
        return {
            1.0f, 0.0f, 0.0f, translation.x,
            0.0f, 1.0f, 0.0f, translation.y,
            0.0f, 0.0f, 1.0f, translation.z,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
    }

    /**
     * Initializes a transformation matrix from the specified scaling factor, a
     * rotation axis, a rotation angle (in radians), and a translation vector.
     */
    static Matrix4x4 compose(float scaling, const ml7::Vector3& axis, float angle, const ml7::Vector3& translation)
    {
        return compose({scaling, scaling, scaling}, axis, angle, translation);
    }

    /**
     * Initializes a transformation matrix from the specified scaling vector, a
     * rotation axis, a rotation angle (in radians), and a translation vector.
     */
    static Matrix4x4 compose(const ml7::Vector3& scaling, const ml7::Vector3& axis, float angle, const ml7::Vector3& translation);

    /**
     * Initializes a view transformation matrix for a left-handed coordinate system
     * using a camera position, a focal point, and an "up" direction.
     */
    static Matrix4x4 look_at_lh(const Vector3& position, const Vector3& focus, const Vector3& up);

    /**
     * Initializes a view transformation matrix for a right-handed coordinate system
     * using a camera position, a focal point, and an "up" direction.
     */
    static Matrix4x4 look_at_rh(const Vector3& position, const Vector3& focus, const Vector3& up);

    /**
     * Initializes a view transformation matrix for a left-handed coordinate system
     * using a camera position, a camera "look" direction, and an "up" direction.
     */
    static Matrix4x4 look_to_lh(const Vector3& position, const Vector3& look, const Vector3& up);

    /**
     * Initializes a view transformation matrix for a right-handed coordinate system
     * using a camera position, a camera "look" direction, and an "up" direction.
     */
    static Matrix4x4 look_to_rh(const Vector3& position, const Vector3& look, const Vector3& up);

    /**
     * Initializes an orthogonal projection matrix for a left-handed coordinate
     * system using a width and a height of the view volume as well as distances to
     * the near and far clipping planes.
     */
    static Matrix4x4 orthographic_lh(float width, float height, float znear, float zfar);

    /**
     * Initializes an orthogonal projection matrix for a right-handed coordinate
     * system using a width and a height of the view volume as well as distances to
     * the near and far clipping planes.
     */
    static Matrix4x4 orthographic_rh(float width, float height, float znear, float zfar);

    /**
     * Initializes a perspective projection matrix for a left-handed coordinate
     * system using a vertical field-of-view angle (in radians), an aspect ratio of
     * the frustum's width to its height, and distances to the near and far clipping
     * planes.
     */
    static Matrix4x4 perspective_lh(float fov, float aspect, float znear, float zfar);

    /**
     * Initializes a perspective projection matrix for a right-handed coordinate
     * system using a vertical field-of-view angle (in radians), an aspect ratio of
     * the frustum's width to its height, and distances to the near and far clipping
     * planes.
     */
    static Matrix4x4 perspective_rh(float fov, float aspect, float znear, float zfar);

    /**
     * Initializes a color transformation matrix using a luminance vector that
     * specifies the weighted sum of the r, g, and b color components, allowing
     * to customize how each component contributes to the (grayscale) result.
     */
    static Matrix4x4 luminance(const ml7::Vector3& l);

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
    static Matrix4x4 saturation(const ml7::Vector3& s);

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
    static Matrix4x4 saturation(const ml7::Vector3& s, const ml7::Vector3& l);

    /**
     * Initializes a color transformation matrix using a contrast vector that
     * specifies the respective contrast for the r, g, and b color components.
     * Interesting: This can also be used to alter the color balance.
     * Note: When c is set to 0, the resulting "color" is a constant gray with all
     * three color components set to 0.5. When c is set to 1, the matrix becomes the
     * identity matrix leaving the color unchanged.
     */
    static Matrix4x4 contrast(const ml7::Vector3& c);

    /**
     * Initializes a color transformation matrix using a rotation angle (in radians)
     * that specifies how much to rotate the r, g, and b color components.
     * The grayscale weights for the r, g, and b color components are given by the
     * luminance vector l = (0.3086, 0.6094, 0.0820) to maintain the brightness.
     * These values are better for linear RGB colors. The NTSC weights of 0.299,
     * 0.587, and 0.114 are only applicable to RGB colors in a gamma 2.2 color space.
     */
    static Matrix4x4 hue(float r);

    /**
     * Initializes a color transformation matrix using a rotation angle (in radians)
     * that specifies how much to rotate the r, g, and b color components.
     * The grayscale weights for the r, g, and b color components are given by the
     * specified luminance vector l to maintain the perceived brightness.
     */
    static Matrix4x4 hue(float r, const ml7::Vector3& l);

}; // struct Matrix4x4



} // namespace ml7

#endif // ML7_MATRIX4X4_H
