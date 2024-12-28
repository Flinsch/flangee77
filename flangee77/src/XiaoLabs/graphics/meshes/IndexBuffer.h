#pragma once
#ifndef XL7_GRAPHICS_MESHES_INDEXBUFFER_H
#define XL7_GRAPHICS_MESHES_INDEXBUFFER_H
#include "./MeshBuffer.h"

#include "./IndexType.h"
#include "./IndexDataProvider.h"



namespace xl7::graphics::meshes {



class IndexBuffer // NOLINT(*-virtual-class-destructor)
    : public MeshBuffer
{

public:
    struct Desc
        : public MeshBuffer::Desc
    {
        /** The type/format (16 or 32 bits) of the index buffer. */
        IndexType index_type;
    };



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
    const Desc& get_desc() const { return _desc; }



protected:
    IndexBuffer(const CreateParams<Desc>& params);
    ~IndexBuffer() override = default;



private:
    /**
     * The descriptor of the index buffer.
     */
    const Desc _desc;

}; // class IndexBuffer



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_INDEXBUFFER_H
