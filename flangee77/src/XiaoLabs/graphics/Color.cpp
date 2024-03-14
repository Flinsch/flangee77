#include "Color.h"

#include <MathLabs/utilities.h>



namespace xl7 {
namespace graphics {



    const Color Color::ZERO =   { 0.0f, 0.0f, 0.0f, 0.0f };
    const Color Color::BLACK =  { 0.0f, 0.0f, 0.0f, 1.0f };
    const Color Color::WHITE =  { 1.0f, 1.0f, 1.0f, 1.0f };
    const Color Color::RED =    { 1.0f, 0.0f, 0.0f, 1.0f };
    const Color Color::GREEN =  { 0.0f, 1.0f, 0.0f, 1.0f };
    const Color Color::BLUE =   { 0.0f, 0.0f, 1.0f, 1.0f };
    const Color Color::YELLOW = { 1.0f, 1.0f, 0.0f, 1.0f };
    const Color Color::MAGENTA ={ 1.0f, 0.0f, 1.0f, 1.0f };
    const Color Color::CYAN =   { 0.0f, 1.0f, 1.0f, 1.0f };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Factory method from hue, saturation, and "value" (brightness),
     * with 0 <= H < 360, 0 <= S <= 1, and 0 <= V <= 1.
     */
    Color Color::from_hsv(float hue, float saturation, float brightness)
    {
        float H = hue < 0.0f ? 360.0f - ::fmodf( -hue, 360.0f ) : ::fmodf( hue, 360.0f );
        float S = ml7::utilities::clamp( saturation, 0.0f, 1.0f );
        float V = ml7::utilities::clamp( brightness, 0.0f, 1.0f );

        // https://www.rapidtables.com/convert/color/hsv-to-rgb.html

        float C = V * S;
        float X = C * (1.0f - ::abs(::fmodf(H / 60.0f, 2.0f) - 1.0f)); // https://www.codespeedy.com/hsv-to-rgb-in-cpp/
        float m = V - C;

        Color v;
        if ( H < 60.0f ) {
            v.r = C;
            v.g = X;
        } else if ( H < 120.0f ) {
            v.r = X;
            v.g = C;
        } else if ( H < 180.0f ) {
            v.g = C;
            v.b = X;
        } else if ( H < 240.0f ) {
            v.g = X;
            v.b = C;
        } else if ( H < 300.0f ) {
            v.r = X;
            v.b = C;
        } else { // => H < 360.0f
            v.r = C;
            v.b = X;
        }

        return v + Color( m, m, m, 1.0f );
    }

    /**
     * Factory method from hue, saturation, and lightness,
     * with 0 <= H < 360, 0 <= S <= 1, and 0 <= L <= 1.
     */
    Color Color::from_hsl(float hue, float saturation, float lightness)
    {
        float H = hue < 0.0f ? 360.0f - ::fmodf( -hue, 360.0f ) : ::fmodf( hue, 360.0f );
        float S = ml7::utilities::clamp( saturation, 0.0f, 1.0f );
        float L = ml7::utilities::clamp( lightness, 0.0f, 1.0f );

        // https://www.rapidtables.com/convert/color/hsl-to-rgb.html

        float C = (1.0f - ::abs(2.0f * L - 1.0f)) * S;
        float X = C * (1.0f - ::abs(::fmodf(H / 60.0f, 2.0f) - 1.0f)); // https://www.codespeedy.com/hsv-to-rgb-in-cpp/
        float m = L - C * 0.5f;

        Color v;
        if ( H < 60.0f ) {
            v.r = C;
            v.g = X;
        } else if ( H < 120.0f ) {
            v.r = X;
            v.g = C;
        } else if ( H < 180.0f ) {
            v.g = C;
            v.b = X;
        } else if ( H < 240.0f ) {
            v.g = X;
            v.b = C;
        } else if ( H < 300.0f ) {
            v.r = X;
            v.b = C;
        } else { // => H < 360.0f
            v.r = C;
            v.b = X;
        }

        return v + Color( m, m, m, 1.0f );
    }

    /**
     * Swap operation.
     */
    void Color::swap(Color& rhs)
    {
        std::swap( r, rhs.r );
        std::swap( g, rhs.g );
        std::swap( b, rhs.b );
        std::swap( a, rhs.a );
    }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns the hue of this color, with 0 <= H < 360.
     */
    float Color::get_hue() const
    {
        // https://www.rapidtables.com/convert/color/rgb-to-hsl.html
        // https://www.rapidtables.com/convert/color/rgb-to-hsv.html

        float Cmax = ml7::utilities::max3( r, g, b );
        float Cmin = ml7::utilities::min3( r, g, b );
        float delta = Cmax - Cmin;
        if ( !delta ) return 0;
        float t = 1.0f / delta;
             if ( Cmax == b ) t *= r - g, t += 4.0f;
        else if ( Cmax == g ) t *= b - r, t += 2.0f;
        else/* => Cmax == r */t *= g - b, t += g < b ? 6.0f : 0.0f;
        return t * 60.0f;
    }

    /**
     * Returns the saturation of this color, with 0 <= S <= 1.
     */
    float Color::get_saturation() const
    {
        // https://www.rapidtables.com/convert/color/rgb-to-hsl.html
        // https://www.rapidtables.com/convert/color/rgb-to-hsv.html

        float Cmax = ml7::utilities::max3( r, g, b );
        float Cmin = ml7::utilities::min3( r, g, b );
        float delta = Cmax - Cmin;
        return Cmax ? delta / Cmax : 0.0f;
    }

    /**
     * Returns the lightness of this color, with 0 <= L <= 1.
     */
    float Color::get_lightness() const
    {
        // https://www.rapidtables.com/convert/color/rgb-to-hsl.html

        float Cmax = ml7::utilities::max3( r, g, b );
        float Cmin = ml7::utilities::min3( r, g, b );
        return (Cmax + Cmin) * 0.5f;
    }

    /**
     * Returns the "value" (brightness) of this color, with 0 <= V <= 1.
     */
    float Color::get_brightness() const
    {
        // https://www.rapidtables.com/convert/color/rgb-to-hsv.html

        float Cmax = ml7::utilities::max3( r, g, b );
        return Cmax;
    }



    // #############################################################################
    // Manipulations
    // #############################################################################

    /**
     * Returns a copy of this color with the components clamped to the range [0;1].
     */
    Color Color::saturated() const
    {
        return Color( r, g, b, a ).saturate();
    }

    /**
     * Clamps the components to the range [0;1].
     */
    Color& Color::saturate()
    {
        r = ml7::utilities::clamp( r, 0.0f, 1.0f );
        g = ml7::utilities::clamp( g, 0.0f, 1.0f );
        b = ml7::utilities::clamp( b, 0.0f, 1.0f );
        a = ml7::utilities::clamp( a, 0.0f, 1.0f );
        return *this;
    }



} // namespace graphics
} // namespace xl7
