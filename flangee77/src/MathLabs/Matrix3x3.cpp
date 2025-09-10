#include "Matrix3x3.h"



namespace ml7 {



    const Matrix3x3 Matrix3x3::ZERO =       {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    const Matrix3x3 Matrix3x3::IDENTITY =   {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Swap operation.
     */
    void Matrix3x3::swap(Matrix3x3& other) noexcept
    {
        Matrix3x3 tmp(other);
        other = *this;
        *this = tmp;
    }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this matrix inverted (if possible).
     */
    Matrix3x3 Matrix3x3::inverted() const
    {
        const float det = determinant();
        if (det == 0.0f)
            return ZERO;
        const float i = 1.0f / det;
        return {
            (_22*_33 - _23*_32)*i,   (_13*_32 - _12*_33)*i,   (_12*_23 - _13*_22)*i,
            (_23*_31 - _21*_33)*i,   (_11*_33 - _13*_31)*i,   (_13*_21 - _11*_23)*i,
            (_21*_32 - _22*_31)*i,   (_12*_31 - _11*_32)*i,   (_11*_22 - _12*_21)*i,
        };
    }

    /**
     * Tries to extract the rotation axis and the rotation angle (in the range
     * [0;pi]) this matrix is composed of.
     * This only works if the matrix actually consists of rotations only (no
     * scalings, shears, etc.).
     */
    bool Matrix3x3::to_axis_angle(ml7::Vector3& axis, float& angle) const
    {
        float t = _11 + _22 + _33;
        float cs = 0.5f * (t - 1.0f);
        angle = std::acos(cs); // [0;pi]

        if (!(angle > 0.0f))
        {
            axis = ml7::Vector3::X; // Any axis would be "correct" here.
            return angle == 0.0f;
        }

        if (angle < ml7::constants::pi)
        {
            axis.x = _32 - _23;
            axis.y = _13 - _31;
            axis.z = _21 - _12;
            axis.normalize();
            return true;
        }

        float _05i;
        if (_11 >= _22)
        {
            if (_11 >= _33) // => _11 is maximum
            {
                axis.x = 0.5f * std::sqrt(_11 - _22 - _33 + 1.0f);
                _05i = 0.5f / axis.x;
                axis.y = _12 * _05i;
                axis.z = _13 * _05i;
            }
            else // => _33 > _11 => _33 is maximum
            {
                axis.z = 0.5f * std::sqrt(_33 - _11 - _22 + 1.0f);
                _05i = 0.5f / axis.z;
                axis.x = _13 * _05i;
                axis.y = _23 * _05i;
            }
        }
        else // => _22 >= _11
        {
            if (_22 >= _33) // => _22 is maximum
            {
                axis.y = 0.5f * std::sqrt(_22 - _11 - _33 + 1.0f);
                _05i = 0.5f / axis.y;
                axis.x = _12 * _05i;
                axis.z = _23 * _05i;
            }
            else // => _33 > _22 => => _33 is maximum
            {
                axis.z = 0.5f * std::sqrt(_33 - _11 - _22 + 1.0f);
                _05i = 0.5f / axis.z;
                axis.x = _13 * _05i;
                axis.y = _23 * _05i;
            }
        }

        return true;
    }

    /**
     * Tries to extract the scaling vector, the rotation axis, and the rotation angle
     * (in the range [0;pi]) this matrix is composed of.
     * This only works if the matrix actually consists of rotations and (positive)
     * scalings in the "common" order (no shears, negative scalings, etc.).
     */
    bool Matrix3x3::decompose(ml7::Vector3& scaling, ml7::Vector3& axis, float& angle) const
    {
        const float sx = Vector3(_11, _21, _31).length();
        const float sy = Vector3(_12, _22, _32).length();
        const float sz = Vector3(_13, _23, _33).length();
        if (sx == 0.0f) return false;
        if (sy == 0.0f) return false;
        if (sz == 0.0f) return false;
        scaling.x = sx;
        scaling.y = sy;
        scaling.z = sz;

        const float sxi = 1.0f / sx;
        const float syi = 1.0f / sy;
        const float szi = 1.0f / sz;
        const ml7::Matrix3x3 m{
            _11*sxi, _12*syi, _13*szi,
            _21*sxi, _22*syi, _23*szi,
            _31*sxi, _32*syi, _33*szi,
        };

        return m.to_axis_angle(axis, angle);
    }

    /**
     * Assumes that this matrix is a view rotation matrix for a left-handed
     * coordinate system and tries to extract the camera "look" direction and the
     * "up" direction.
     */
    bool Matrix3x3::is_look_lh(ml7::Vector3& look, ml7::Vector3& up) const
    {
        look = {_31, _32, _33};
        up = {_21, _22, _23};
        return look.length_squared() != 0.0f && up.length_squared() != 0.0f; // Just some bare minimum check.
    }

    /**
     * Assumes that this matrix is a view rotation matrix for a right-handed
     * coordinate system and tries to extract the camera "look" direction and the
     * "up" direction.
     */
    bool Matrix3x3::is_look_rh(ml7::Vector3& look, ml7::Vector3& up) const
    {
        look = {-_31, -_32, -_33};
        up = {_21, _22, _23};
        return look.length_squared() != 0.0f && up.length_squared() != 0.0f; // Just some bare minimum check.
    }

    /**
     * Returns a copy of the given (column) vector transformed by this matrix
     * inverted (if possible).
     */
    Vector3 Matrix3x3::transform_inverted(const Vector3& v) const
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
    Matrix3x3 Matrix3x3::rotation_normalized(const ml7::Vector3& unit_axis, float angle)
    {
        const ml7::Vector3& u = unit_axis;
        if (angle == 0.0f)
            return IDENTITY;
        const float cs = std::cos(angle);
        const float sn = std::sin(angle);
        const float C = 1.0f - cs;
        const float xsn = u.x*sn;
        const float ysn = u.y*sn;
        const float zsn = u.z*sn;
        const float xC = u.x*C;
        const float yC = u.y*C;
        const float zC = u.z*C;
        const float xyC = u.x*yC;
        const float xzC = u.x*zC;
        const float yzC = u.y*zC;
        return {
            u.x*xC + cs,   xyC - zsn,     xzC + ysn,
            xyC + zsn,     u.y*yC + cs,   yzC - xsn,
            xzC - ysn,     yzC + xsn,     u.z*zC + cs,
        };
    }

    /**
     * Initializes a transformation matrix from the specified scaling vector, a
     * rotation axis, and a rotation angle (in radians).
     */
    Matrix3x3 Matrix3x3::compose(const ml7::Vector3& scaling, const ml7::Vector3& axis, float angle)
    {
        if (angle == 0.0f)
            return Matrix3x3::scaling(scaling);
        const float sx = scaling.x;
        const float sy = scaling.y;
        const float sz = scaling.z;
        const ml7::Vector3 u = axis.normalized();
        const float cs = std::cos(angle);
        const float sn = std::sin(angle);
        const float C = 1.0f - cs;
        const float xsn = u.x*sn;
        const float ysn = u.y*sn;
        const float zsn = u.z*sn;
        const float xC = u.x*C;
        const float yC = u.y*C;
        const float zC = u.z*C;
        const float xyC = u.x*yC;
        const float xzC = u.x*zC;
        const float yzC = u.y*zC;
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
    Matrix3x3 Matrix3x3::look_lh(const Vector3& look, const Vector3& up)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatlh
        const Vector3 z(look.normalized());
        const Vector3 x(up.cross(z).normalized());
        const Vector3 y(z.cross(x));
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
    Matrix3x3 Matrix3x3::look_rh(const Vector3& look, const Vector3& up)
    {
        // https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatrh
        return look_lh(-look, up);
    }



} // namespace ml7
