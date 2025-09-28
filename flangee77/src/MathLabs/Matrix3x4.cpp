#include "Matrix3x4.h"



namespace ml7 {



    const Matrix3x4 Matrix3x4::ZERO =       {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    const Matrix3x4 Matrix3x4::IDENTITY =   {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f};



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Swap operation.
     */
    void Matrix3x4::swap(Matrix3x4& other) noexcept
    {
        Matrix3x4 tmp(other);
        other = *this;
        *this = tmp;
    }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this matrix inverted (if possible).
     */
    Matrix3x4 Matrix3x4::inverted() const
    {
        const float det = determinant();
        if (det == 0.0f)
            return ZERO;
        const float i = 1.0f / det;
        Matrix3x3 m3x3{
            (_22*_33 - _23*_32)*i,   (_13*_32 - _12*_33)*i,   (_12*_23 - _13*_22)*i,
            (_23*_31 - _21*_33)*i,   (_11*_33 - _13*_31)*i,   (_13*_21 - _11*_23)*i,
            (_21*_32 - _22*_31)*i,   (_12*_31 - _11*_32)*i,   (_11*_22 - _12*_21)*i,
        };
        Vector3 translation{
            -(m3x3._11*_14 + m3x3._12*_24 + m3x3._13*_34),
            -(m3x3._21*_14 + m3x3._22*_24 + m3x3._23*_34),
            -(m3x3._31*_14 + m3x3._32*_24 + m3x3._33*_34),
        };
        return {m3x3, translation};
    }

    /**
     * Tries to extract the rotation axis and the rotation angle (in the range
     * [0;pi]) the 3x3 part of this matrix is composed of.
     * This only works if the 3x3 part actually consists of rotations only (no
     * scalings, shears, etc.).
     */
    bool Matrix3x4::to_axis_angle(ml7::Vector3& axis, float& angle) const
    {
        return to_matrix3x3().to_axis_angle(axis, angle);
    }

    /**
     * Tries to extract the scaling vector, the rotation axis, the rotation angle
     * (in the range [0;pi]), and the translation vector this matrix is composed of.
     * This only works if the matrix actually consists of translations, rotations and
     * (positive) scalings in the "common" order (no shears, negative scalings, etc.).
     */
    bool Matrix3x4::decompose(ml7::Vector3& scaling, ml7::Vector3& axis, float& angle, ml7::Vector3& translation) const
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
    bool Matrix3x4::is_look_lh(ml7::Vector3& position, ml7::Vector3& look, ml7::Vector3& up) const
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
        return look.length_squared() != 0.0f && up.length_squared() != 0.0f; // Just some bare minimum check.
    }

    /**
     * Assumes that this matrix is a view transformation matrix for a right-handed
     * coordinate system and tries to extract the camera position, the camera "look"
     * direction, and the "up" direction.
     */
    bool Matrix3x4::is_look_rh(ml7::Vector3& position, ml7::Vector3& look, ml7::Vector3& up) const
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
        return look.length_squared() != 0.0f && up.length_squared() != 0.0f; // Just some bare minimum check.
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix
     * inverted (if possible).
     * Used to transform position vectors rather than direction vectors.
     */
    Vector3 Matrix3x4::transform_inverted(const Vector3& v) const
    {
        return transform3x3_inverted(v - ml7::Vector3(_14, _24, _34));
    }

    /**
     * Returns a copy of the given (column) vector transformed by the 3x3 part of
     * this matrix inverted (if possible).
     * Used to transform direction vectors rather than position vectors.
     */
    Vector3 Matrix3x4::transform3x3_inverted(const Vector3& v) const
    {
        const float det = determinant();
        if (det == 0.0f)
            return v;
        const float i = 1.0f / det;
        return {
            ((_22*_33 - _23*_32)*v.x + (_13*_32 - _12*_33)*v.y + (_12*_23 - _13*_22)*v.z)*i,
            ((_23*_31 - _21*_33)*v.x + (_11*_33 - _13*_31)*v.y + (_13*_21 - _11*_23)*v.z)*i,
            ((_21*_32 - _22*_31)*v.x + (_12*_31 - _11*_32)*v.y + (_11*_22 - _12*_21)*v.z)*i,
        };
    }



    // #############################################################################
    // Static Functions
    // #############################################################################

    /**
     * Initializes a rotation matrix representing a rotation by a certain angle (in
     * radians) around an axis specified as a unit vector.
     */
    Matrix3x4 Matrix3x4::rotation_normalized(const ml7::Vector3& unit_axis, float angle)
    {
        return Matrix3x4(Matrix3x3::rotation_normalized(unit_axis, angle));
    }

    /**
     * Initializes a transformation matrix from the specified scaling vector, a
     * rotation axis, a rotation angle (in radians), and a translation vector.
     */
    Matrix3x4 Matrix3x4::compose(const ml7::Vector3& scaling, const ml7::Vector3& axis, float angle, const ml7::Vector3& translation)
    {
        return {Matrix3x3::compose(scaling, axis, angle), translation};
    }

    /**
     * Initializes a view transformation matrix for a left-handed coordinate system
     * using a camera position, a focal point, and an "up" direction.
     */
    Matrix3x4 Matrix3x4::look_at_lh(const Vector3& position, const Vector3& focus, const Vector3& up)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatlh
        return look_to_lh(position, focus - position, up);
    }

    /**
     * Initializes a view transformation matrix for a right-handed coordinate system
     * using a camera position, a focal point, and an "up" direction.
     */
    Matrix3x4 Matrix3x4::look_at_rh(const Vector3& position, const Vector3& focus, const Vector3& up)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatrh
        return look_to_rh(position, focus - position, up);
    }

    /**
     * Initializes a view transformation matrix for a left-handed coordinate system
     * using a camera position, a camera "look" direction, and an "up" direction.
     */
    Matrix3x4 Matrix3x4::look_to_lh(const Vector3& position, const Vector3& look, const Vector3& up)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatlh
        const Vector3 z(look.normalized());
        const Vector3 x(up.cross(z).normalized());
        const Vector3 y(z.cross(x));
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
    Matrix3x4 Matrix3x4::look_to_rh(const Vector3& position, const Vector3& look, const Vector3& up)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatrh
        return look_to_lh(position, -look, up);
    }



} // namespace ml7
