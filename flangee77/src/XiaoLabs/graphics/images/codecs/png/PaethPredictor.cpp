#include "PaethPredictor.h"

#include <cstdlib>
#include <cstdint>



namespace xl7::graphics::images::codecs::png {



    /**
     * Computes a simple linear function of the three neighboring pixels (left, above,
     * upper left) and returns the neighboring pixel closest to the computed value.
     * See PNG specification, section 9.4.
     */
    uint8_t PaethPredictor::predict(uint8_t a, uint8_t b, uint8_t c)
    {
        const auto A = static_cast<signed>(a);
        const auto B = static_cast<signed>(b);
        const auto C = static_cast<signed>(c);

        const signed p = A + B - C;
        const signed pA = std::abs(p - A);
        const signed pB = std::abs(p - B);
        const signed pC = std::abs(p - C);

        if (pA <= pB && pA <= pC)
            return a;
        if (pB <= pC)
            return b;
        return c;
    }



} // namespace xl7::graphics::images::codecs::png
