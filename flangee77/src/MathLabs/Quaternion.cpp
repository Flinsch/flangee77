#include "Quaternion.h"



namespace ml7 {



    const Quaternion Quaternion::ZERO =     { 0.0f, 0.0f, 0.0f, 0.0f };
    const Quaternion Quaternion::IDENTITY = { 0.0f, 0.0f, 0.0f, 1.0f };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Swap operation.
     */
    void Quaternion::swap(Quaternion& rhs)
    {
        std::swap( x, rhs.x );
        std::swap( y, rhs.y );
        std::swap( z, rhs.z );
        std::swap( w, rhs.w );
    }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this quaternion normalized.
     */
    Quaternion Quaternion::normalized() const
    {
        float d = norm();
        if ( d == 0.0f )
            return ZERO; // x = y = z = w = 0
        d = 1.0f / d;
        return Quaternion( x*d, y*d, z*d, w*d );
    }

    /**
     * Returns a copy of this quaternion inverted (if possible).
     */
    Quaternion Quaternion::inverted() const
    {
        float d = norm();
        if ( d == 0.0f )
            return ZERO; // x = y = z = w = 0
        d = -1.0f / d;
        return Quaternion( x*d, y*d, z*d, -w*d );
    }

    /**
     * Returns the rotation matrix corresponding to this object quaternion.
     */
    Matrix3x3 Quaternion::to_matrix3x3() const
    {
        const float d = norm();
        const float r = d == 0.0f ? 0.0f : 2.0f / d;
        const float xs = x*r;
        const float ys = y*r;
        const float zs = z*r;
        const float xw = xs*w;
        const float yw = ys*w;
        const float zw = zs*w;
        const float xx = x*xs;
        const float xy = x*ys;
        const float xz = x*zs;
        const float yy = y*ys;
        const float yz = y*zs;
        const float zz = z*zs;
        return {
            1.0f - (yy + zz),   xy - zw,   xz + yw,
            xy + zw,   1.0f - (xx + zz),   yz - xw,
            xz - yw,   yz + xw,   1.0f - (xx + yy),
        };
    }

    /**
     * Extracts the basis vectors that define the transformed coordinate system.
     */
    void Quaternion::to_axes(ml7::Vector3& x, ml7::Vector3& y, ml7::Vector3& z) const
    {
        to_matrix3x3().to_axes( x, y, z );
    }

    /**
     * Tries to extract the rotation axis and the rotation angle (in the range
     * [0;pi]) this quaternion is composed of.
     */
    bool Quaternion::to_axis_angle(ml7::Vector3& axis, float& angle) const
    {
        const float lensqr = x*x + y*y + z*z;
        if ( lensqr > 0.0f )
        {
            const float invlen = 1.0f / ::sqrtf( lensqr );
            axis.x = x * invlen;
            axis.y = y * invlen;
            axis.z = z * invlen;
            angle = 2.0f * ::acosf( w );
            return true;
        }
        else
        {
            axis = ml7::Vector3::X; // Any axis would be "correct" here.
            angle = 0.0f;
            return false;
        }
    }

    /**
     * Assumes that this quaternion represents a "view" rotation for a left-handed
     * coordinate system and tries to extract the camera "look" direction and the
     * "up" direction.
     */
    bool Quaternion::is_look_lh(ml7::Vector3& look, ml7::Vector3& up) const
    {
        return to_matrix3x3().is_look_lh( look, up );
    }

    /**
     * Assumes that this quaternion represents a "view" rotation for a right-handed
     * coordinate system and tries to extract the camera "look" direction and the
     * "up" direction.
     */
    bool Quaternion::is_look_rh(ml7::Vector3& look, ml7::Vector3& up) const
    {
        return to_matrix3x3().is_look_rh( look, up );
    }

    /**
     * Returns a copy of the given vector transformed by this quaternion.
     */
    Vector3 Quaternion::transform(const Vector3& v) const
    {
        ml7::Vector3 u{ x, y, z };
        ml7::Vector3 uv = u.cross( v );
        ml7::Vector3 uuv = u.cross( uv );
        uv *= 2.0f * w;
        uuv *= 2.0f;
        return v + uv + uuv;
    }

    /**
     * Returns a copy of the given vector transformed by this quaternion inverted
     * (if possible).
     */
    Vector3 Quaternion::transform_inverted(const Vector3& v) const
    {
        float d = norm();
        if ( d == 0.0f )
            return v;
        d = -1.0f / d;
        ml7::Vector3 u{ x*d, y*d, z*d };
        ml7::Vector3 uv = u.cross( v );
        ml7::Vector3 uuv = u.cross( uv );
        uv *= 2.0f * -w*d;
        uuv *= 2.0f;
        return v + uv + uuv;
    }



} // namespace ml7
