#include "Angle.h"

#include <MathLabs/constants.h>
#include <MathLabs/utilities.h>



namespace ml7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Initializes an angle in degrees.
     */
    Angle Angle::from_degrees(float degrees)
    {
        return Angle( ml7::utilities::deg2rad( degrees ) );
    }

    /**
     * Initializes an angle from a value with the specified half-cycle.
     */
    Angle Angle::from_half_cycle(float value, float half_cycle)
    {
        return Angle( value / half_cycle * ml7::constants::pi );
    }

    /**
     * Initializes an angle from a value with the specified cycle.
     */
    Angle Angle::from_cycle(float value, float cycle)
    {
        return Angle( value / cycle * ml7::constants::pi2 );
    }



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the angle value transformed to degrees.
     */
    float Angle::to_degrees() const
    {
        return ml7::utilities::rad2deg( radians );
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
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this angle normalized to the range [-pi;+pi].
     */
    Angle Angle::normalized_symmetric() const
    {
        if ( radians < -ml7::constants::pi )
            return ml7::constants::pi - ::fmodf( -radians - ml7::constants::pi, ml7::constants::pi2 );
        if ( radians > ml7::constants::pi )
            return ::fmodf( radians + ml7::constants::pi, ml7::constants::pi2 ) - ml7::constants::pi;
        return radians;
    }

    /**
     * Returns a copy of this angle normalized to the range [0;2pi].
     */
    Angle Angle::normalized_asymmetric() const
    {
        if ( radians < 0.0f )
            return ml7::constants::pi2 - ::fmodf( -radians, ml7::constants::pi2 );
        if ( radians > ml7::constants::pi2 )
            return ::fmodf( radians, ml7::constants::pi2 );
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
        if ( radians < -ml7::constants::pi )
            radians = ml7::constants::pi - ::fmodf( -radians - ml7::constants::pi, ml7::constants::pi2 );
        else if ( radians > ml7::constants::pi )
            radians = ::fmodf( radians + ml7::constants::pi, ml7::constants::pi2 ) - ml7::constants::pi;
        return *this;
    }

    /**
     * Normalizes this angle to the range [0;2pi].
     */
    Angle& Angle::normalize_asymmetric()
    {
        if ( radians < 0.0f )
            radians = ml7::constants::pi2 - ::fmodf( -radians, ml7::constants::pi2 );
        else if ( radians > ml7::constants::pi2 )
            radians = ::fmodf( radians, ml7::constants::pi2 );
        return *this;
    }



} // namespace ml7
