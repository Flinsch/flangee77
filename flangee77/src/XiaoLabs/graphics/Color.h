#ifndef XL7_GRAPHICS_COLOR_H
#define XL7_GRAPHICS_COLOR_H

#include "./ChannelOrder.h"



namespace xl7::graphics {



struct Color
{

    static const Color ZERO;
    static const Color BLACK;
    static const Color WHITE;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color YELLOW;
    static const Color MAGENTA;
    static const Color CYAN;

    static_assert(static_cast<unsigned>(ChannelOrder::RGBA) == 0);
    static_assert(static_cast<unsigned>(ChannelOrder::ARGB) == 1);
    static_assert(static_cast<unsigned>(ChannelOrder::ABGR) == 2);
    static_assert(static_cast<unsigned>(ChannelOrder::BGRA) == 3);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmultichar"
#if ('RGBA') == 0x52474241 // => little endian
    static constexpr unsigned RGBA_INDICES_BY_CHANNEL_ORDER[4][4] = {
        { 0, 1, 2, 3, }, // RGBA
        { 1, 2, 3, 0, }, // ARGB
        { 3, 2, 1, 0, }, // ABGR
        { 2, 1, 0, 3, }, // BGRA
    };
#endif
#if ('RGBA') == 0x41424752 // => big endian
    static constexpr unsigned RGBA_INDICES_BY_CHANNEL_ORDER[4][4] = {
        { 3, 2, 1, 0, }, // RGBA
        { 2, 1, 0, 3, }, // ARGB
        { 0, 1, 2, 3, }, // ABGR
        { 1, 2, 3, 0, }, // BGRA
    };
#endif
#pragma GCC diagnostic pop



    // #############################################################################
    // Attributes
    // #############################################################################

    float r;
    float g;
    float b;
    float a;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Initializes R, G, B, and A all to 0.
     */
    constexpr Color() noexcept
        : r(0.0f)
        , g(0.0f)
        , b(0.0f)
        , a(0.0f)
    {
    }

    constexpr Color(float r, float g, float b, float a = 1.0f) noexcept
        : r(r)
        , g(g)
        , b(b)
        , a(a)
    {
    }

    constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff) noexcept
        : r(uint8_to_float(r))
        , g(uint8_to_float(g))
        , b(uint8_to_float(b))
        , a(uint8_to_float(a))
    {
    }

    constexpr Color(uint32_t value, ChannelOrder channel_order)
    {
        Value32 x;
        const auto& rgba_indices = RGBA_INDICES_BY_CHANNEL_ORDER[static_cast<unsigned>(channel_order)];
        x.value = value;
        r = uint8_to_float(x.v[rgba_indices[0]]);
        g = uint8_to_float(x.v[rgba_indices[1]]);
        b = uint8_to_float(x.v[rgba_indices[2]]);
        a = uint8_to_float(x.v[rgba_indices[3]]);
    }

    void swap(Color& other) noexcept;



    // #############################################################################
    // Properties
    // #############################################################################

    constexpr uint8_t get_r() const { return float_to_uint8(r); }
    constexpr uint8_t get_g() const { return float_to_uint8(g); }
    constexpr uint8_t get_b() const { return float_to_uint8(b); }
    constexpr uint8_t get_a() const { return float_to_uint8(a); }

    constexpr Color& set_r(uint8_t r) { this->r = uint8_to_float(r); return *this; }
    constexpr Color& set_g(uint8_t g) { this->g = uint8_to_float(g); return *this; }
    constexpr Color& set_b(uint8_t b) { this->b = uint8_to_float(b); return *this; }
    constexpr Color& set_a(uint8_t a) { this->a = uint8_to_float(a); return *this; }

    constexpr uint32_t to_uint32(ChannelOrder channel_order) const
    {
        Value32 x;
        const auto& rgba_indices = RGBA_INDICES_BY_CHANNEL_ORDER[static_cast<unsigned>(channel_order)];
        x.v[rgba_indices[0]] = get_r();
        x.v[rgba_indices[1]] = get_g();
        x.v[rgba_indices[2]] = get_b();
        x.v[rgba_indices[3]] = get_a();
        return x.value;
    }

    constexpr uint32_t to_rgba32() const { return to_uint32(ChannelOrder::RGBA); }
    constexpr uint32_t to_argb32() const { return to_uint32(ChannelOrder::ARGB); }
    constexpr uint32_t to_abgr32() const { return to_uint32(ChannelOrder::ABGR); }
    constexpr uint32_t to_bgra32() const { return to_uint32(ChannelOrder::BGRA); } // Corresponds to D3DCOLOR of Direct3D 9.

    const float* get_rgba_ptr() const { return &r; }
    float* get_rgba_ptr() { return &r; }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns the hue of this color, with 0 <= H < 360.
     */
    float get_hue() const;

    /**
     * Returns the saturation of this color, with 0 <= S <= 1.
     */
    float get_saturation() const;

    /**
     * Returns the lightness of this color, with 0 <= L <= 1.
     */
    float get_lightness() const;

    /**
     * Returns the "value" (brightness) of this color, with 0 <= V <= 1.
     */
    float get_brightness() const;



    // #############################################################################
    // Manipulations
    // #############################################################################

    /**
     * Returns a copy of this color with the components clamped to the range [0;1].
     */
    Color saturated() const;

    /**
     * Clamps the components of this color to the range [0;1].
     */
    Color& saturate();



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    /**
     * Checks whether two colors are equal. The comparison is quite fuzzy:
     * ultimately, it is enough if the colors are the same in terms of their 32-bit
     * unsigned-integer representation, even if the floating point values should be
     * different.
     */
    constexpr bool operator==(const Color& c) const { return ((r == c.r) && (g == c.g) && (b == c.b) && (a == c.a)) || to_rgba32() == c.to_rgba32(); }

    /**
     * Checks whether two colors are unequal. The comparison is quite fuzzy:
     * Ultimately, it is not enough for the floating point values to be different;
     * the colors must also differ in terms of their 32-bit unsigned-integer
     * representation.
     */
    constexpr bool operator!=(const Color& c) const { return ((r != c.r) || (b != c.g) || (b != c.b) || (a != c.a)) && to_rgba32() != c.to_rgba32(); }



    // #############################################################################
    // Arithmetic Operators
    // #############################################################################

    constexpr Color operator+() const { return *this; }
    constexpr Color operator-() const { return {-r, -g, -b, -a}; }

    constexpr Color operator+(const Color& c) const { return {r + c.r, g + c.g, b + c.b, a + c.a}; }
    constexpr Color operator-(const Color& c) const { return {r - c.r, g - c.g, b - c.b, a - c.a}; }

    constexpr Color operator*(const Color& c) const { return {r * c.r, g * c.g, b * c.b, a * c.a}; }
    constexpr Color operator/(const Color& c) const { return {r / c.r, g / c.g, b / c.b, a / c.a}; }

    constexpr Color operator*(float s) const { return {r * s, g * s, b * s, a * s}; }
    constexpr Color operator/(float s) const { return {r / s, g / s, b / s, a / s}; }



    // #############################################################################
    // Arithmetic Assignment Operators
    // #############################################################################

    constexpr Color& operator+=(const Color& c) { r += c.r; g += c.g; b += c.b; a += c.a; return *this; }
    constexpr Color& operator-=(const Color& c) { r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this; }

    constexpr Color& operator*=(const Color& c) { r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this; }
    constexpr Color& operator/=(const Color& c) { r /= c.r; g /= c.g; b /= c.b; a /= c.a; return *this; }

    constexpr Color& operator*=(float s) { r *= s; g *= s; b *= s; a *= s; return *this; }
    constexpr Color& operator/=(float s) { r /= s; g /= s; b /= s; a /= s; return *this; }



    // #############################################################################
    // Access Operators
    // #############################################################################

    float operator[](unsigned i) const { assert(i < 4); return get_rgba_ptr()[i]; }
    float& operator[](unsigned i) { assert(i < 4); return get_rgba_ptr()[i]; }



    // #############################################################################
    // Factory Methods
    // #############################################################################

    /**
     * Factory method from hue, saturation, and "value" (brightness),
     * with 0 <= H < 360, 0 <= S <= 1, and 0 <= V <= 1.
     */
    static Color from_hsv(float hue, float saturation, float brightness);

    /**
     * Factory method from hue, saturation, and lightness,
     * with 0 <= H < 360, 0 <= S <= 1, and 0 <= L <= 1.
     */
    static Color from_hsl(float hue, float saturation, float lightness);



    // #############################################################################
    // Helpers
    // #############################################################################

    static constexpr uint8_t float_to_uint8(float x) { return static_cast<uint8_t>(x * 255.0f); }
    static constexpr float uint8_to_float(uint8_t x) { return static_cast<float>(x) / 255.0f; }



private:
    union Value32
    {
        struct
        {
            uint8_t v[4];
        };
        uint32_t value;
    };

}; // struct Color



    // #############################################################################
    // Additional Operators
    // #############################################################################

    constexpr Color operator*(float s, const Color& c) { return {s * c.r, s * c.g, s * c.b, s * c.a}; }



} // namespace xl7::graphics

#endif // XL7_GRAPHICS_COLOR_H
