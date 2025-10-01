#ifndef ML7_ANGLE_H
#define ML7_ANGLE_H

#include <MathLabs/math.h>



namespace ml7 {



template <std::floating_point T>
struct Angle
{

    using scalar_type = T;



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The angle in radians.
     */
    T radians;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    constexpr Angle(T radians = T{0}) noexcept
        : radians(radians)
    {
    }
    
    void swap(Angle& a) noexcept
    {
        std::swap(radians, a.radians);
    }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this angle normalized to the range [-pi;+pi].
     */
    Angle normalized_symmetric() const
    {
        if (radians < -constants<T>::pi)
            return constants<T>::pi - std::fmod(-radians - constants<T>::pi, constants<T>::pi2);
        if (radians > constants<T>::pi)
            return std::fmod(radians + constants<T>::pi, constants<T>::pi2) - constants<T>::pi;
        return radians;
    }

    /**
     * Returns a copy of this angle normalized to the range [0;2pi].
     */
    Angle normalized_asymmetric() const
    {
        if (radians < T{0})
            return constants<T>::pi2 - std::fmod(-radians, constants<T>::pi2);
        if (radians > constants<T>::pi2)
            return std::fmod(radians, constants<T>::pi2);
        return radians;
    }



    // #############################################################################
    // Manipulations
    // #############################################################################

    /**
     * Normalizes this angle to the range [-pi;+pi].
     */
    Angle& normalize_symmetric()
    {
        if (radians < -constants<T>::pi)
            radians = constants<T>::pi - std::fmod(-radians - constants<T>::pi, constants<T>::pi2);
        else if (radians > constants<T>::pi)
            radians = std::fmod(radians + constants<T>::pi, constants<T>::pi2) - constants<T>::pi;
        return *this;
    }

    /**
     * Normalizes this angle to the range [0;2pi].
     */
    Angle& normalize_asymmetric()
    {
        if (radians < T{0})
            radians = constants<T>::pi2 - std::fmod(-radians, constants<T>::pi2);
        else if (radians > constants<T>::pi2)
            radians = std::fmod(radians, constants<T>::pi2);
        return *this;
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the angle value in radians.
     */
    constexpr T to_radians() const
    {
        return radians;
    }

    /**
     * Returns the angle value transformed to degrees.
     */
    T to_degrees() const
    {
        return ml7::rad_to_deg(radians);
    }

    /**
     * Returns the angle value transformed to the specified half-cycle.
     */
    T to_half_cycle(T half_cycle) const
    {
        return radians / constants<T>::pi * half_cycle;
    }

    /**
     * Returns the angle value transformed to the specified cycle.
     */
    T to_cycle(T cycle) const
    {
        return radians / constants<T>::pi2 * cycle;
    }

    /**
     * Returns the angle value transformed to the specified half-cycle.
     */
    template <T half_cycle>
    T to_half_cycle() const
    {
        return radians / constants<T>::pi * half_cycle;
    }

    /**
     * Returns the angle value transformed to the specified cycle.
     */
    template <T cycle>
    T to_cycle() const
    {
        return radians / constants<T>::pi2 * cycle;
    }



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(Angle a) const noexcept { return radians == a.radians; }
    bool operator!=(Angle a) const noexcept { return radians != a.radians; }

    bool operator<(Angle a) const noexcept { return radians < a.radians; }
    bool operator>(Angle a) const noexcept { return radians > a.radians; }

    bool operator<=(Angle a) const noexcept { return radians <= a.radians; }
    bool operator>=(Angle a) const noexcept { return radians >= a.radians; }



    // #############################################################################
    // Arithmetic Operators
    // #############################################################################

    /** Returns a copy of this angle unmodified. */
    constexpr Angle operator+() const { return *this; }
    /** Returns a copy of this angle negated. */
    constexpr Angle operator-() const { return {-radians}; }

    /** Returns the sum of two angles. */
    constexpr Angle operator+(Angle a) const { return {radians + a.radians}; }
    /** Returns the difference of two angles. */
    constexpr Angle operator-(Angle a) const { return {radians - a.radians}; }

    /** Returns a copy of this angle multiplied by the specified scalar. */
    constexpr Angle operator*(T s) const { return {radians * s}; }
    /** Returns a copy of this angle divided by the given scalar. */
    constexpr Angle operator/(T s) const { return {radians / s}; }

    /** Scales a given angle by the specified scalar. */
    friend constexpr Angle operator*(T s, Angle a) { return a * s; }



    // #############################################################################
    // Arithmetic Assignment Operators
    // #############################################################################

    /** Adds the given angle to this one. */
    constexpr Angle& operator+=(Angle a) { radians += a.radians; return *this; }
    /** Subtracts the given angle from this one. */
    constexpr Angle& operator-=(Angle a) { radians -= a.radians; return *this; }

    /** Multiplies this angle by the specified scalar. */
    constexpr Angle& operator*=(T s) { radians *= s; return *this; }
    /** Divides this angle by the specified scalar. */
    constexpr Angle& operator/=(T s) { radians /= s; return *this; }



    // #############################################################################
    // Conversion Operators
    // #############################################################################

    operator T() const { return radians; }



    // #############################################################################
    // Static Functions
    // #############################################################################

    /**
     * Initializes an angle in radians.
     */
    static constexpr Angle from_radians(T radians)
    {
        return {radians};
    }

    /**
     * Initializes an angle in degrees.
     */
    static Angle from_degrees(T degrees)
    {
        return {ml7::deg_to_rad(degrees)};
    }

    /**
     * Initializes an angle from a value with the specified half-cycle.
     */
    static Angle from_half_cycle(T value, T half_cycle)
    {
        return {value / half_cycle * constantsf::pi};
    }

    /**
     * Initializes an angle from a value with the specified cycle.
     */
    static Angle from_cycle(T value, T cycle)
    {
        return {value / cycle * constantsf::pi2};
    }

    /**
     * Initializes an angle from a value with the specified half-cycle.
     */
    template <T half_cycle>
    static Angle from_half_cycle(T value)
    {
        return {value / half_cycle * constantsf::pi};
    }

    /**
     * Initializes an angle from a value with the specified cycle.
     */
    template <T cycle>
    static Angle from_cycle(T value)
    {
        return {value / cycle * constantsf::pi2};
    }

}; // struct Angle



using AngleF = Angle<float>;
using AngleD = Angle<double>;
using AngleLD = Angle<long double>;



} // namespace ml7

#endif // ML7_ANGLE_H
