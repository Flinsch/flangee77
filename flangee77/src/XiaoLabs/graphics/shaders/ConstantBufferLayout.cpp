#include "ConstantBufferLayout.h"

#include <algorithm>



namespace xl7::graphics::shaders {



    /** Calculates the total size of the constant buffer, in bytes. */
    unsigned ConstantBufferLayout::calculate_size() const
    {
        if (constant_declarations.empty())
            return 0;

        // At this point we cannot assume that the constant declarations have
        // (already) been sorted, so we have to take the "naive" approach.

        unsigned first_offset = constant_declarations[0].offset;
        unsigned last_offset = constant_declarations[0].offset;
        unsigned last_size = constant_declarations[0].size;

        for (size_t i = 1; i < constant_declarations.size(); ++i)
        {
            if (constant_declarations[i].offset < first_offset)
                first_offset = constant_declarations[i].offset;

            if (constant_declarations[i].offset > last_offset)
            {
                last_offset = constant_declarations[i].offset;
                last_size = constant_declarations[i].size;
            }
        }

        assert(first_offset <= last_offset);

        return last_size + last_offset - first_offset;
    }

    /**
     * Sorts the constant declarations and adjusts their padded sizes to fill any
     * gaps within the constant buffer resulting from memory alignment.
     */
    void ConstantBufferLayout::sort_and_adjust_padded_sizes()
    {
        if (constant_declarations.empty())
            return;

        std::ranges::sort(constant_declarations, [](const auto& a, const auto& b) {
            return a.offset < b.offset;
        });

        for (size_t i = 1; i < constant_declarations.size(); ++i)
            constant_declarations[i - 1].padded_size = constant_declarations[i].offset - constant_declarations[i - 1].offset;
        constant_declarations.back().padded_size = constant_declarations.back().size;
    }



} // namespace xl7::graphics::shaders
