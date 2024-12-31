#ifndef XL7_GRAPHICS_SHADERS_CONSTANTBUFFERMAPPING_H
#define XL7_GRAPHICS_SHADERS_CONSTANTBUFFERMAPPING_H

#include "./ConstantMapping.h"

#include <vector>



namespace xl7::graphics::shaders {



struct ConstantBufferMapping
{

    /** . */
    std::vector<ConstantMapping> constant_mappings;



    /**
     * Attempts to merge consecutive constant mappings to minimize the number of
     * entries. Ideally, this results in a single mapping entry when there is a
     * continuous 1-to-1 mapping between (sections of) source and shader buffer.
     * The padded sizes of the constant mappings must be set correctly for this
     * operation to be effectively carried out as intended.
     */
    void try_merge_constant_mappings();

}; // struct ConstantBufferMapping



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_CONSTANTBUFFERMAPPING_H
