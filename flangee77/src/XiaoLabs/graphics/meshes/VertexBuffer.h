#ifndef XL7_GRAPHICS_MESHES_VERTEXBUFFER_H
#define XL7_GRAPHICS_MESHES_VERTEXBUFFER_H
#include "./MeshBuffer.h"

#include "./VertexBufferDesc.h"
#include "./VertexDataProvider.h"



namespace xl7::graphics::meshes {



class VertexBuffer
    : public resources::detail::ResourceBase<VertexBuffer, MeshBuffer>
{

public:
    VertexBuffer() = delete;

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    VertexBuffer(VertexBuffer&&) = delete;
    VertexBuffer& operator=(VertexBuffer&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"vertex buffer"; }

    /**
     * Returns the descriptor of the vertex buffer.
     */
    const VertexBufferDesc& get_desc() const { return _desc; }



protected:

    explicit VertexBuffer(const CreateContext& ctx, const VertexBufferDesc& desc);

    ~VertexBuffer() override = default;



private:
    /**
     * The descriptor of the vertex buffer.
     */
    const VertexBufferDesc _desc;

}; // class VertexBuffer



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_VERTEXBUFFER_H
