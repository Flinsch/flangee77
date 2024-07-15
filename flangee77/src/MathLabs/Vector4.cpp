#include "Vector4.h"

#include <MathLabs/constants.h>
#include <MathLabs/utilities.h>



namespace ml7 {



    const Vector4 Vector4::ZERO =   { 0.0f, 0.0f, 0.0f };
    const Vector4 Vector4::X =      { 1.0f, 0.0f, 0.0f };
    const Vector4 Vector4::Y =      { 0.0f, 1.0f, 0.0f };
    const Vector4 Vector4::Z =      { 0.0f, 0.0f, 1.0f };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Swap operation.
     */
    void Vector4::swap(Vector4& rhs)
    {
        std::swap( x, rhs.x );
        std::swap( y, rhs.y );
        std::swap( z, rhs.z );
        std::swap( w, rhs.w );
    }



    // #############################################################################
    // Static Methods
    // #############################################################################

    /** Returns a vector having the minimum components of two given vectors. */
    Vector4 Vector4::min2(const Vector4& a, const Vector4& b)
    {
        return Vector4( utilities::min2(a.x, b.x), utilities::min2(a.y, b.y), utilities::min2(a.z, b.z), utilities::min2(a.w, b.w) );
    }

    /** Returns a vector having the maximum components of two given vectors. */
    Vector4 Vector4::max2(const Vector4& a, const Vector4& b)
    {
        return Vector4( utilities::max2(a.x, b.x), utilities::max2(a.y, b.y), utilities::max2(a.z, b.z), utilities::max2(a.w, b.w) );
    }

    /**
     * Performs a linear interpolation between a and b, with y = a + (b-a)x.
     */
    Vector4 Vector4::lerp(const Vector4& a, const Vector4& b, float x)
    {
        return a + (b-a)*x;
    }

    /**
     * Performs a cosine interpolation between a and b, with y = a + (b-a)z and
     * z = (1-cos(wx))/2, w = pi.
     */
    Vector4 Vector4::terp(const Vector4& a, const Vector4& b, float x)
    {
        x = (1.0f-::cosf( constants::pi*x ))*0.5f;
        return a + (b-a)*x;
    }

    /**
     * Performs a cubic interpolation between a and b (affected by a0, the point
     * "before" a, and b1, the point "after" b), with y = Px^3 + Qx^2 + Rx + S and
     * P = (b1-b)-(a0-a), Q = (a0-a)-P, R = b-a0, S = a.
     */
    Vector4 Vector4::cerp(const Vector4& a0, const Vector4& a, const Vector4& b, const Vector4& b1, float x)
    {
        const Vector4 P = (b1-b)-(a0-a);
        const Vector4 Q = (a0-a)-P;
        const Vector4 R = b-a0;
        const Vector4 S = a;
        const float x2 = x*x;
        return P*x2*x + Q*x2 + R*x + S;
    }



} // namespace ml7
