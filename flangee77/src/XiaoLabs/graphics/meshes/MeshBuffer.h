#ifndef XL7_GRAPHICS_MESHES_MESHBUFFER_H
#define XL7_GRAPHICS_MESHES_MESHBUFFER_H
#include "../../resources/Resource.h"

#include "./Topology.h"
#include "./MeshUtil.h"



namespace xl7::graphics::meshes {



class MeshManager;



class MeshBuffer
    : public resources::Resource
{

public:
    XL7_DECLARE_RESOURCE_ID();

    enum struct Type
    {
        VertexBuffer,
        IndexBuffer,
    };

    struct Desc
    {
        /** Identifies how the buffer is expected to be updated (frequency of update is a key factor). */
        resources::ResourceUsage usage;

        /** The topology of the primitives that the buffer represents. */
        Topology topology;
        /** The number of elements within the buffer, regardless of the topology (i.e., vertices or indices, not primitives). */
        unsigned count;
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
    const Desc& get_desc() const { return _desc; }

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

    template <class TDesc>
        requires(std::derived_from<TDesc, Desc>)
    MeshBuffer(Type type, const CreateParams<TDesc>& params, unsigned stride)
        : Resource(params)
        , _type(type)
        , _desc(params.desc) // NOLINT(*-slicing)
        , _primitive_count(MeshUtil::calculate_primitive_count(params.desc.topology, params.desc.count))
        , _stride(stride)
        , _size(_stride * _desc.count)
    {
    }

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
    const Desc _desc;

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
