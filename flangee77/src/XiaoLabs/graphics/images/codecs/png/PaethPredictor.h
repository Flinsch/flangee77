#ifndef XL7_GRAPHICS_IMAGES_CODECS_PNG_PAETHPREDICTOR_H
#define XL7_GRAPHICS_IMAGES_CODECS_PNG_PAETHPREDICTOR_H

#include <cstdint>



namespace xl7::graphics::images::codecs::png {



struct PaethPredictor
{

    /**
     * Computes a simple linear function of the three neighboring pixels (left, above,
     * upper left) and returns the neighboring pixel closest to the computed value.
     * See PNG specification, section 9.4.
     */
    static uint8_t predict(uint8_t a, uint8_t b, uint8_t c);

}; // struct PaethPredictor



} // namespace xl7::graphics::images::codecs::png

#endif // XL7_GRAPHICS_IMAGES_CODECS_PNG_PAETHPREDICTOR_H
