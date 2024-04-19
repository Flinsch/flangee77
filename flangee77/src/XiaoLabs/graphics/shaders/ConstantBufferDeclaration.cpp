#include "ConstantBufferDeclaration.h"



namespace xl7 {
namespace graphics {
namespace shaders {



    /** Calculates the total size of the constant buffer, in bytes. */
    unsigned ConstantBufferDeclaration::calculate_size() const
    {
        unsigned size = 0;
        for ( const auto& p : constant_table )
            size += p.second.size;
        return size;
    }



} // namespace shaders
} // namespace graphics
} // namespace xl7
