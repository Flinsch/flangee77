#include "Vector2.h"



namespace ml7 {



    const Vector2 Vector2::ZERO =   {0.0f, 0.0f};
    const Vector2 Vector2::X =      {1.0f, 0.0f};
    const Vector2 Vector2::Y =      {0.0f, 1.0f};



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Swap operation.
     */
    void Vector2::swap(Vector2& other) noexcept
    {
        std::swap(x, other.x);
        std::swap(y, other.y);
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the azimuth angle of this vector in the signed range [-pi;+pi], with
     * the positive x-axis pointing east and the positive y-axis pointing north.
     */
    float Vector2::azimuth_symmetric() const
    {
        return ::atan2f(x, y);
    }

    /**
     * Returns the azimuth angle of this vector in the positive range [0;2pi], with
     * the positive x-axis pointing east and the positive y-axis pointing north.
     */
    float Vector2::azimuth_asymmetric() const
    {
        float a = ::atan2f(x, y);
        if (x < 0.0f)
            a += ml7::constants::pi2;
        return a;
    }

    /**
     * Returns the elevation angle of this vector in the range [-pi/2;+pi/2], with
     * the positive y-axis pointing up.
     */
    float Vector2::elevation() const
    {
        return ::atan2f(y, ::abs(x));
    }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns this vector refracted across a plane defined by a given normal
     * vector n with respect to the specified refractive index.
     */
    Vector2 Vector2::refracted(const Vector2& n, float index) const
    {
        const float t = dot(n);
        const float r = 1-index*index*(1-t*t);

        if (r < 0)
            return {};

        const float s = index*t + ::sqrtf(r);
        return index * *this - s * n;
    }



    // #############################################################################
    // Static Methods
    // #############################################################################

    /** Returns a vector having the minimum components of two given vectors. */
    Vector2 Vector2::min2(const Vector2& a, const Vector2& b)
    {
        return {ml7::min2(a.x, b.x), ml7::min2(a.y, b.y)};
    }

    /** Returns a vector having the maximum components of two given vectors. */
    Vector2 Vector2::max2(const Vector2& a, const Vector2& b)
    {
        return {ml7::max2(a.x, b.x), ml7::max2(a.y, b.y)};
    }

    /**
     * Performs a linear interpolation between a and b, with y = a + (b-a)x.
     */
    Vector2 Vector2::lerp(const Vector2& a, const Vector2& b, float x)
    {
        return a + (b-a)*x;
    }

    /**
     * Performs a cosine interpolation between a and b, with y = a + (b-a)z and
     * z = (1-cos(wx))/2, w = pi.
     */
    Vector2 Vector2::terp(const Vector2& a, const Vector2& b, float x)
    {
        x = (1.0f-::cosf(constants::pi*x))*0.5f;
        return a + (b-a)*x;
    }

    /**
     * Performs a cubic interpolation between a and b (affected by a0, the point
     * "before" a, and b1, the point "after" b), with y = Px^3 + Qx^2 + Rx + S and
     * P = (b1-b)-(a0-a), Q = (a0-a)-P, R = b-a0, S = a.
     */
    Vector2 Vector2::cerp(const Vector2& a0, const Vector2& a, const Vector2& b, const Vector2& b1, float x)
    {
        const Vector2 P = (b1-b)-(a0-a);
        const Vector2 Q = (a0-a)-P;
        const Vector2 R = b-a0;
        const Vector2 S = a;
        const float x2 = x*x;
        return P*x2*x + Q*x2 + R*x + S;
    }

    /**
     * Checks whether the three given points are a counter-clockwise turn. In that
     * case, the function returns a value greater than 0. A value less than 0
     * indicates a clockwise turn. 0 indicates that the points are collinear.
     */
    float Vector2::ccw(const ml7::Vector2& a, const ml7::Vector2& b, const ml7::Vector2& c)
    {
        return (b.x - a.x)*(c.y - b.y) - (c.x - b.x)*(b.y - a.y);
    }



} // namespace ml7
