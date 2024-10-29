#pragma once
#ifndef ML7_ANGLE_H
#define ML7_ANGLE_H

#include <CoreLabs/root.h>



namespace ml7 {



class Angle
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default/explicit constructor.
     */
    constexpr Angle(float radians = 0.0f)
        : radians( radians )
    {
    }

    /**
     * Initializes an angle in radians.
     */
    static constexpr Angle from_radians(float radians) { return Angle( radians ); }

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
    
    /**
     * Swap operation.
     */
    void swap(Angle& a)
    {
        std::swap( radians, a.radians );
    }



    // #############################################################################
    // Attributes
    // #############################################################################
public:
    /**
     * The angle in radians.
     */
    float radians;



    // #############################################################################
    // Properties
    // #############################################################################
public:
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
    // Transformations
    // #############################################################################
public:
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
public:
    /**
     * Normalizes this angle to the range [-pi;+pi].
     */
    Angle& normalize_symmetric();

    /**
     * Normalizes this angle to the range [0;2pi].
     */
    Angle& normalize_asymmetric();



    // #############################################################################
    // Arithmetic Operators
    // #############################################################################
public:
    /** Returns a copy of this angle unmodified. */
    constexpr Angle operator + () const { return *this; }
    /** Returns a copy of this angle negated. */
    constexpr Angle operator - () const { return Angle( -radians ); }

    /** Returns the sum of two angles. */
    constexpr Angle operator + (Angle a) const { return Angle( radians + a.radians ); }
    /** Returns the difference of two angles. */
    constexpr Angle operator - (Angle a) const { return Angle( radians - a.radians ); }

    /** Returns a copy of this angle multiplied by the specified scalar. */
    constexpr Angle operator * (float s) const { return Angle( radians * s ); }
    /** Returns a copy of this angle divided by the given scalar. */
    constexpr Angle operator / (float s) const { return Angle( radians / s ); }



    // #############################################################################
    // Arithmetic Assignment Operators
    // #############################################################################
public:
    /** Adds the given angle to this one. */
    constexpr Angle& operator += (Angle a) { radians += a.radians; return *this; }
    /** Subtracts the given angle from this one. */
    constexpr Angle& operator -= (Angle a) { radians -= a.radians; return *this; }

    /** Multiplies this angle by the specified scalar. */
    constexpr Angle& operator *= (float s) { radians *= s; return *this; }
    /** Divides this angle by the specified scalar. */
    constexpr Angle& operator /= (float s) { radians /= s; return *this; }



    // #############################################################################
    // Comparison Operators
    // #############################################################################
public:
    bool operator == (Angle a) const { return radians == a.radians; }
    bool operator != (Angle a) const { return radians != a.radians; }

    bool operator < (Angle a) const { return radians < a.radians; }
    bool operator > (Angle a) const { return radians > a.radians; }

    bool operator <= (Angle a) const { return radians <= a.radians; }
    bool operator >= (Angle a) const { return radians >= a.radians; }



    // #############################################################################
    // Conversion Operators
    // #############################################################################
public:
    operator float () const { return radians; }

}; // class Angle



    // #############################################################################
    // Additional Operators
    // #############################################################################

    /**
     * Scales a given angle by the specified scalar.
     */
    constexpr Angle operator * (float s, Angle a) { return a * s; }



} // namespace ml7

#endif // ML7_ANGLE_H
