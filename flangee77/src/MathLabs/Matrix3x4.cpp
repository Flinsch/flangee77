#include "Matrix3x4.h"



namespace ml7 {



    const Matrix3x4 Matrix3x4::ZERO =       { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    const Matrix3x4 Matrix3x4::IDENTITY =   { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Initializes a rotation matrix representing a clockwise rotation by angle theta
     * (in radians) around an axis specified as a unit vector.
     */
    Matrix3x4 Matrix3x4::rotation_normalized(const ml7::Vector3& u, float theta)
    {
        return Matrix3x4( Matrix3x3::rotation_normalized( u, theta ) );
    }

    /**
     * Initializes a transformation matrix from the specified scaling vector, a
     * clockwise rotation angle theta (in radians) around the specified axis, and a
     * translation vector.
     */
    Matrix3x4 Matrix3x4::compose(const ml7::Vector3& scaling, const ml7::Vector3& axis, float theta, const ml7::Vector3& translation)
    {
        return Matrix3x4( Matrix3x3::compose( scaling, axis, theta ), translation );
    }

    /**
     * Swap operation.
     */
    void Matrix3x4::swap(Matrix3x4& rhs)
    {
        Matrix3x4 tmp( rhs );
        rhs = *this;
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
        if ( det == 0.0f )
            return ZERO;
        const float i = 1.0f / det;
        Matrix3x3 m3x3 {
            (_22*_33 - _23*_32)*i,   (_13*_32 - _12*_33)*i,   (_12*_23 - _13*_22)*i,
            (_23*_31 - _21*_33)*i,   (_11*_33 - _13*_31)*i,   (_13*_21 - _11*_23)*i,
            (_21*_32 - _22*_31)*i,   (_12*_31 - _11*_32)*i,   (_11*_22 - _12*_21)*i,
        };
        Vector3 translation {
            -(m3x3._11*_14 + m3x3._12*_24 + m3x3._13*_34),
            -(m3x3._21*_14 + m3x3._22*_24 + m3x3._23*_34),
            -(m3x3._31*_14 + m3x3._32*_24 + m3x3._33*_34),
        };
        return Matrix3x4( m3x3, translation );
    }

    /**
     * Tries to extract the rotation axis and the clockwise rotation angle theta (in
     * the range [0;pi]) the 3x3 part of this matrix is composed of.
     * This only works if the 3x3 part actually consists of rotations only (no
     * scalings, shears, etc.).
     */
    bool Matrix3x4::to_axis_angle(ml7::Vector3& axis, float& theta) const
    {
        return to_matrix3x3().to_axis_angle( axis, theta );
    }

    /**
     * Tries to extract the scaling vector, the rotation axis, the clockwise rotation
     * angle theta (in the range [0;pi]), and the translation vector this matrix is
     * composed of.
     * This only works if the matrix actually consists of translations, rotations and
     * (positive) scalings in the "common" order (no shears, negative scalings, etc.).
     */
    bool Matrix3x4::decompose(ml7::Vector3& scaling, ml7::Vector3& axis, float& theta, ml7::Vector3& translation) const
    {
        translation.x = _14;
        translation.y = _24;
        translation.z = _34;
        return to_matrix3x3().decompose( scaling, axis, theta );
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix.
     * Used to transform position vectors rather than direction vectors.
     */
    Vector3 Matrix3x4::transform_inverted(const Vector3& v) const
    {
        return transform3x3_inverted( v - ml7::Vector3( _14, _24, _34 ) );
    }

    /**
     * Returns a copy of the given (column) vector transformed by the 3x3 part of
     * this matrix.
     * Used to transform direction vectors rather than position vectors.
     */
    Vector3 Matrix3x4::transform3x3_inverted(const Vector3& v) const
    {
        const float det = determinant();
        if ( det == 0.0f )
            return v;
        const float i = 1.0f / det;
        return {
            ((_22*_33 - _23*_32)*v.x + (_13*_32 - _12*_33)*v.y + (_12*_23 - _13*_22)*v.z)*i,
            ((_23*_31 - _21*_33)*v.x + (_11*_33 - _13*_31)*v.y + (_13*_21 - _11*_23)*v.z)*i,
            ((_21*_32 - _22*_31)*v.x + (_12*_31 - _11*_32)*v.y + (_11*_22 - _12*_21)*v.z)*i,
        };
    }



} // namespace ml7
