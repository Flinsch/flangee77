#ifndef ML7_ANGLE_H
#define ML7_ANGLE_H

#include <MathLabs/math.h>



namespace ml7 {



struct Angle
{

    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The angle in radians.
     */
    float radians;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    constexpr Angle(float radians = 0.0f) noexcept
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
    Angle normalized_symmetric() const;

    /**
     * Returns a copy of this angle normalized to the range [0;2pi].
     */
    Angle normalized_asymmetric() const;



    // #############################################################################
    // Manipulations
    // #############################################################################

    /**
     * Normalizes this angle to the range [-pi;+pi].
     */
    Angle& normalize_symmetric();

    /**
     * Normalizes this angle to the range [0;2pi].
     */
    Angle& normalize_asymmetric();



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the angle value in radians.
     */
    constexpr float to_radians() const { return radians; }

    /**
     * Returns the angle value transformed to degrees.
     */
    float to_degrees() const;

    /**
     * Returns the angle value transformed to the specified half-cycle.
     */
    float to_half_cycle(float half_cycle) const;

    /**
     * Returns the angle value transformed to the specified cycle.
     */
    float to_cycle(float cycle) const;



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
    constexpr Angle operator*(float s) const { return {radians * s}; }
    /** Returns a copy of this angle divided by the given scalar. */
    constexpr Angle operator/(float s) const { return {radians / s}; }



    // #############################################################################
    // Arithmetic Assignment Operators
    // #############################################################################

    /** Adds the given angle to this one. */
    constexpr Angle& operator+=(Angle a) { radians += a.radians; return *this; }
    /** Subtracts the given angle from this one. */
    constexpr Angle& operator-=(Angle a) { radians -= a.radians; return *this; }

    /** Multiplies this angle by the specified scalar. */
    constexpr Angle& operator*=(float s) { radians *= s; return *this; }
    /** Divides this angle by the specified scalar. */
    constexpr Angle& operator/=(float s) { radians /= s; return *this; }



    // #############################################################################
    // Conversion Operators
    // #############################################################################

    operator float() const { return radians; }



    // #############################################################################
    // Static Functions
    // #############################################################################

    /**
     * Initializes an angle in radians.
     */
    static constexpr Angle from_radians(float radians) { return {radians}; }

    /**
     * Initializes an angle in degrees.
     */
    static Angle from_degrees(float degrees);

    /**
     * Initializes an angle from a value with the specified half-cycle.
     */
    static Angle from_half_cycle(float value, float half_cycle);

    /**
     * Initializes an angle from a value with the specified cycle.
     */
    static Angle from_cycle(float value, float cycle);

}; // struct Angle



    // #############################################################################
    // Additional Operators
    // #############################################################################

    /**
     * Scales a given angle by the specified scalar.
     */
    constexpr Angle operator*(float s, Angle a) { return a * s; }



} // namespace ml7

#endif // ML7_ANGLE_H
