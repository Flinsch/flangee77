#include "Angle.h"



namespace ml7 {



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this angle normalized to the range [-pi;+pi].
     */
    Angle Angle::normalized_symmetric() const
    {
        if (radians < -ml7::constants::pi)
            return ml7::constants::pi - std::fmod(-radians - ml7::constants::pi, ml7::constants::pi2);
        if (radians > ml7::constants::pi)
            return std::fmod(radians + ml7::constants::pi, ml7::constants::pi2) - ml7::constants::pi;
        return radians;
    }

    /**
     * Returns a copy of this angle normalized to the range [0;2pi].
     */
    Angle Angle::normalized_asymmetric() const
    {
        if (radians < 0.0f)
            return ml7::constants::pi2 - std::fmod(-radians, ml7::constants::pi2);
        if (radians > ml7::constants::pi2)
            return std::fmod(radians, ml7::constants::pi2);
        return radians;
    }



    // #############################################################################
    // Manipulations
    // #############################################################################

    /**
     * Normalizes this angle to the range [-pi;+pi].
     */
    Angle& Angle::normalize_symmetric()
    {
        if (radians < -ml7::constants::pi)
            radians = ml7::constants::pi - std::fmod(-radians - ml7::constants::pi, ml7::constants::pi2);
        else if (radians > ml7::constants::pi)
            radians = std::fmod(radians + ml7::constants::pi, ml7::constants::pi2) - ml7::constants::pi;
        return *this;
    }

    /**
     * Normalizes this angle to the range [0;2pi].
     */
    Angle& Angle::normalize_asymmetric()
    {
        if (radians < 0.0f)
            radians = ml7::constants::pi2 - std::fmod(-radians, ml7::constants::pi2);
        else if (radians > ml7::constants::pi2)
            radians = std::fmod(radians, ml7::constants::pi2);
        return *this;
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the angle value transformed to degrees.
     */
    float Angle::to_degrees() const
    {
        return ml7::rad_to_deg(radians);
    }

    /**
     * Returns the angle value transformed to the specified half-cycle.
     */
    float Angle::to_half_cycle(float half_cycle) const
    {
        return radians / ml7::constants::pi * half_cycle;
    }

    /**
     * Returns the angle value transformed to the specified cycle.
     */
    float Angle::to_cycle(float cycle) const
    {
        return radians / ml7::constants::pi2 * cycle;
    }



    // #############################################################################
    // Static Functions
    // #############################################################################

    /**
     * Initializes an angle in degrees.
     */
    Angle Angle::from_degrees(float degrees)
    {
        return {ml7::deg_to_rad(degrees)};
    }

    /**
     * Initializes an angle from a value with the specified half-cycle.
     */
    Angle Angle::from_half_cycle(float value, float half_cycle)
    {
        return {value / half_cycle * ml7::constants::pi};
    }

    /**
     * Initializes an angle from a value with the specified cycle.
     */
    Angle Angle::from_cycle(float value, float cycle)
    {
        return {value / cycle * ml7::constants::pi2};
    }



} // namespace ml7
