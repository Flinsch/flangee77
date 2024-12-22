#include "VertexBufferBinding.h"



namespace xl7::graphics::impl::shared::meshes {



    VertexBufferBinding::VertexBufferBinding()
        : vertex_buffer_ids{}
        , stream_count(0)
    {
    }

    bool VertexBufferBinding::operator==(const VertexBufferBinding& rhs) const noexcept
    {
        if (stream_count != rhs.stream_count)
            return false;

        for (unsigned i = 0; i < stream_count; ++i)
            if (vertex_buffer_ids[i] != rhs.vertex_buffer_ids[i])
                return false;

        return true;
    }

    size_t VertexBufferBinding::hash() const noexcept
    {
        auto hash = static_cast<size_t>(stream_count);

        for (unsigned i = 0; i < stream_count; ++i)
            hash ^= static_cast<size_t>(vertex_buffer_ids[i].value()) << (i + 1);

        return hash;
    }



} // namespace xl7::graphics::impl::shared::meshes
