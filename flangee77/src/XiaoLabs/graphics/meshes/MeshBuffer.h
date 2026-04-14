#ifndef XL7_GRAPHICS_MESHES_MESHBUFFER_H
#define XL7_GRAPHICS_MESHES_MESHBUFFER_H
#include "../../resources/UpdatableResource.h"

#include "./MeshBufferDesc.h"
#include "./MeshBufferUpdater.h"



namespace xl7::graphics::meshes {



class MeshManager;



class MeshBuffer
    : public resources::ResourceWithData<MeshBuffer>
{

public:
    enum struct Type
    {
        VertexBuffer,
        IndexBuffer,
    };



    MeshBuffer() = delete;

    MeshBuffer(const MeshBuffer&) = delete;
    MeshBuffer& operator=(const MeshBuffer&) = delete;
    MeshBuffer(MeshBuffer&&) = delete;
    MeshBuffer& operator=(MeshBuffer&&) = delete;



    /**
     * Returns the type of the buffer.
     */
    Type get_type() const { return _type; }

    /**
     * Returns the descriptor of the buffer.
     */
    const MeshBufferDesc& get_desc() const { return _desc; }

    /**
     * Returns the size of each buffer element, in bytes.
     */
    unsigned get_element_stride() const { return _desc.element_stride; }

    /**
     * Returns the number of primitives represented by this buffer.
     */
    unsigned get_primitive_count() const { return _primitive_count; }



protected:

    MeshBuffer(const CreateContext& ctx, Type type, const MeshBufferDesc& desc);

    ~MeshBuffer() override = default;



private:

    /**
     * The type of the buffer.
     */
    const Type _type;

    /**
     * The descriptor of the buffer.
     */
    const MeshBufferDesc _desc;

    /**
     * The number of primitives represented by this buffer.
     */
    const unsigned _primitive_count;

}; // class MeshBuffer



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_MESHBUFFER_H
