#include "utilities.h"

#include <cmath>



namespace ml7 {
namespace utilities {



    /**
     * Rounds the given value to the closest "integer" value.
     */
    float round(float x)
    {
        return std::floorf( x + 0.5f );
    }

    /**
     * Rounds the given value to the closest "integer" value.
     */
    double round(double x)
    {
        return std::floor( x + 0.5 );
    }

    /**
     * Rounds the given value to the specified number of decimal places.
     */
    float round(float x, unsigned num_decimals)
    {
        const auto p = std::powf( 10.0f, static_cast<float>( num_decimals ) );
        return round( x * p ) / p;
    }

    /**
     * Rounds the given value to the specified number of decimal places.
     */
    double round(double x, unsigned num_decimals)
    {
        const auto p = std::pow( 10.0, static_cast<double>( num_decimals ) );
        return round( x * p ) / p;
    }



} // namespace utilities
} // namespace ml7
