#ifndef XL7_GRAPHICS_SHADERS_CONSTANTBUFFERLAYOUT_H
#define XL7_GRAPHICS_SHADERS_CONSTANTBUFFERLAYOUT_H

#include "./ConstantDeclaration.h"

#include <vector>



namespace xl7::graphics::shaders {



struct ConstantBufferLayout
{

    /** The "ordered list" of constant declarations. */
    std::vector<ConstantDeclaration> constant_declarations;



    /** Calculates the total size of the constant buffer, in bytes. */
    unsigned calculate_size() const;

    /**
     * Sorts the constant declarations and adjusts their padded sizes to fill any
     * gaps within the constant buffer resulting from memory alignment.
     */
    void sort_and_adjust_padded_sizes();

}; // struct ConstantBufferLayout



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_CONSTANTBUFFERLAYOUT_H
