#include "Vector3.h"

#include <MathLabs/constants.h>
#include <MathLabs/utilities.h>



namespace ml7 {



    const Vector3 Vector3::ZERO =   { 0.0f, 0.0f, 0.0f };
    const Vector3 Vector3::X =      { 1.0f, 0.0f, 0.0f };
    const Vector3 Vector3::Y =      { 0.0f, 1.0f, 0.0f };
    const Vector3 Vector3::Z =      { 0.0f, 0.0f, 1.0f };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Swap operation.
     */
    void Vector3::swap(Vector3& rhs)
    {
        std::swap( x, rhs.x );
        std::swap( y, rhs.y );
        std::swap( z, rhs.z );
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the azimuth angle of this vector in the signed range [-pi;+pi], with
     * the positive x-axis pointing east and the positive z-axis pointing north.
     */
    float Vector3::azimuth_symmetric() const
    {
        return ::atan2f( x, z );
    }

    /**
     * Returns the azimuth angle of this vector in the positive range [0;2pi], with
     * the positive x-axis pointing east and the positive z-axis pointing north.
     */
    float Vector3::azimuth_asymmetric() const
    {
        float a = ::atan2f( x, z );
        if ( x < 0.0f )
            a += ml7::constants::pi2;
        return a;
    }

    /**
     * Returns the elevation angle of this vector in the range [-pi/2;+pi/2], with
     * the positive y-axis pointing up.
     */
    float Vector3::elevation() const
    {
        return ::atan2f( y, ::sqrtf( x*x + z*z ) );
    }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns this vector refracted across a plane defined by a given normal
     * vector n with respect to the specified refractive index.
     */
    Vector3 Vector3::refracted(const Vector3& n, float index) const
    {
        const float t = dot(n);
        const float r = 1-index*index*(1-t*t);

        if ( r < 0 )
            return Vector3();

        const float s = index*t + ::sqrtf(r);
        return index * *this - s * n;
    }



    // #############################################################################
    // Static Methods
    // #############################################################################

    /** Returns a vector having the minimum components of two given vectors. */
    Vector3 Vector3::min2(const Vector3& a, const Vector3& b)
    {
        return Vector3( utilities::min2(a.x, b.x), utilities::min2(a.y, b.y), utilities::min2(a.z, b.z) );
    }

    /** Returns a vector having the maximum components of two given vectors. */
    Vector3 Vector3::max2(const Vector3& a, const Vector3& b)
    {
        return Vector3( utilities::max2(a.x, b.x), utilities::max2(a.y, b.y), utilities::max2(a.z, b.z) );
    }

    /**
     * Performs a linear interpolation between a and b, with y = a + (b-a)x.
     */
    Vector3 Vector3::lerp(const Vector3& a, const Vector3& b, float x)
    {
        return a + (b-a)*x;
    }

    /**
     * Performs a cosine interpolation between a and b, with y = a + (b-a)z and
     * z = (1-cos(wx))/2, w = pi.
     */
    Vector3 Vector3::terp(const Vector3& a, const Vector3& b, float x)
    {
        x = (1.0f-::cosf( constants::pi*x ))*0.5f;
        return a + (b-a)*x;
    }

    /**
     * Performs a cubic interpolation between a and b (affected by a0, the point
     * "before" a, and b1, the point "after" b), with y = Px^3 + Qx^2 + Rx + S and
     * P = (b1-b)-(a0-a), Q = (a0-a)-P, R = b-a0, S = a.
     */
    Vector3 Vector3::cerp(const Vector3& a0, const Vector3& a, const Vector3& b, const Vector3& b1, float x)
    {
        const Vector3 P = (b1-b)-(a0-a);
        const Vector3 Q = (a0-a)-P;
        const Vector3 R = b-a0;
        const Vector3 S = a;
        const float x2 = x*x;
        return P*x2*x + Q*x2 + R*x + S;
    }



} // namespace ml7
