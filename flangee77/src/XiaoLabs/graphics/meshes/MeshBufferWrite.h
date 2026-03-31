#ifndef XL7_GRAPHICS_MESHES_MESHBUFFERWRITE_H
#define XL7_GRAPHICS_MESHES_MESHBUFFERWRITE_H

#include <CoreLabs/byte_view.h>



namespace xl7::graphics::meshes {



struct MeshBufferWrite
{
    /** The provided source element data to write. */
    cl7::byte_view data;

    /** The first element to update, regardless of the topology. */
    unsigned first_element = 0;
    /** The number of elements to update, regardless of the topology. */
    unsigned element_count;
};



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_MESHBUFFERWRITE_H
