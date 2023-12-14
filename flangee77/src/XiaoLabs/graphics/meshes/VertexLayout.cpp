#include "VertexLayout.h"



namespace xl7 {
namespace graphics {
namespace meshes {



    /** Returns the size of the given data type, in bytes. */
    unsigned VertexLayout::Element::get_size(DataType data_type)
    {
        switch ( data_type )
        {
        case DataType::FLOAT1:
            return 1 * sizeof(float);
        case DataType::FLOAT2:
            return 2 * sizeof(float);
        case DataType::FLOAT3:
            return 3 * sizeof(float);
        case DataType::FLOAT4:
            return 4 * sizeof(float);
        case DataType::COLOR:
        case DataType::UBYTE4:
            return 4;
        default:
            assert( false );
        }

        return 0;
    }

    /** Returns the data size of this element, in bytes. */
    unsigned VertexLayout::Element::get_size() const
    {
        return get_size( data_type );
    }

    /** Calculates the total size of the vertex layout, in bytes. */
    unsigned VertexLayout::calculate_size() const
    {
        unsigned size = 0;
        for ( const Element& element : elements )
            size += element.get_size();
        return size;
    }



} // namespace meshes
} // namespace graphics
} // namespace xl7
