#ifndef XL7_GRAPHICS_MESHES_MESHBUFFER_H
#define XL7_GRAPHICS_MESHES_MESHBUFFER_H
#include "../../resources/Resource.h"

#include "./MeshBufferDesc.h"
#include "./DirtyRange.h"
#include "./MeshUtil.h"



namespace xl7::graphics::meshes {



class MeshManager;



class MeshBuffer
    : public resources::detail::ResourceBaseDirty<MeshBuffer, DirtyRange>
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
     * Returns the number of primitives represented by this buffer.
     */
    unsigned get_primitive_count() const { return _primitive_count; }

    /**
     * Returns the size of each buffer element, in bytes.
     */
    unsigned get_stride() const { return _stride; }

    /**
     * Returns the size of this buffer, in bytes.
     */
    unsigned get_size() const { return _size; }



    /**
     * Updates the contents of this buffer (unless it is immutable).
     */
    bool update(const resources::DataProvider& data_provider);



protected:

    MeshBuffer(Type type, const CreateContext& ctx, const MeshBufferDesc& desc, unsigned stride);

    ~MeshBuffer() override = default;



private:

    /**
     * Updates the contents of this buffer (unless it is immutable).
     * The given data provider can possibly be ignored because the local data buffer
     * has already been updated based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _update_impl(const resources::DataProvider& data_provider, bool discard, bool no_overwrite) = 0;



    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    bool _check_data_impl(const resources::DataProvider& data_provider) override;



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

    /**
     * The size of each buffer element, in bytes.
     */
    const unsigned _stride;

    /**
     * The size of this buffer, in bytes.
     */
    const unsigned _size;

}; // class MeshBuffer



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_MESHES_MESHBUFFER_H
