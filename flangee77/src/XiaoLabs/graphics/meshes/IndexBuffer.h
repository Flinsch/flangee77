#ifndef XL7_GRAPHICS_MESHES_INDEXBUFFER_H
#define XL7_GRAPHICS_MESHES_INDEXBUFFER_H
#include "./MeshBuffer.h"

#include "./IndexBufferDesc.h"
#include "./IndexDataProvider.h"



namespace xl7::graphics::meshes {



class IndexBuffer
    : public resources::detail::ResourceBase<IndexBuffer, MeshBuffer>
{

public:
    IndexBuffer() = delete;

    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;
    IndexBuffer(IndexBuffer&&) = delete;
    IndexBuffer& operator=(IndexBuffer&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"index buffer"; }

    /**
     * Returns the descriptor of the index buffer.
     */
    const IndexBufferDesc& get_desc() const { return _desc; }



protected:

    explicit IndexBuffer(const CreateContext& ctx, const IndexBufferDesc& desc);

    ~IndexBuffer() override = default;



private:
    /**
     * The descriptor of the index buffer.
     */
    const IndexBufferDesc _desc;

}; // class IndexBuffer



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_INDEXBUFFER_H
