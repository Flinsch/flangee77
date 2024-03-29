#pragma once
#ifndef XL7_GRAPHICS_MESHES_MESHBUFFER_H
#define XL7_GRAPHICS_MESHES_MESHBUFFER_H
#include "../../resources/Resource.h"

#include "./Topology.h"
#include "./MeshUtil.h"



namespace xl7 {
namespace graphics {
namespace meshes {



class MeshManager;



class MeshBuffer
    : public resources::Resource
{

public:
    enum class Type
    {
        VertexBuffer,
        IndexBuffer,
    };

public:
    struct Desc
    {
        /** Identifies how the buffer is expected to be updated (frequency of update is a key factor). */
        resources::ResourceUsage usage;

        /** The topology of the primitives that the buffer represents. */
        Topology topology;
        /** The number of elements within the buffer. */
        unsigned count;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    template <class TDesc>
        requires( std::derived_from<TDesc, Desc> )
    MeshBuffer(Type type, const CreateParams<TDesc>& params, unsigned stride)
        : Resource( params )
        , _type( type )
        , _desc( params.desc )
        , _primitive_count( MeshUtil::calculate_primitive_count( params.desc.topology, params.desc.count ) )
        , _stride( stride )
        , _size( _stride * _desc.count )
    {
    }

    /**
     * Destructor.
     */
    virtual ~MeshBuffer() = default;

private:
    /** Default constructor. */
    MeshBuffer() = delete;
    /** Copy constructor. */
    MeshBuffer(const MeshBuffer&) = delete;
    /** Copy assignment operator. */
    MeshBuffer& operator = (const MeshBuffer&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
protected:
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

private:



    // #############################################################################
    // Properties
    // #############################################################################
public:
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

public:



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Updates the contents of this buffer (unless it is immutable).
     */
    bool update(const resources::DataProvider& data_provider);



    // #############################################################################
    // Resource Implementations
    // #############################################################################
private:
    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    virtual bool _check_data_impl(const resources::DataProvider& data_provider) override;



    // #############################################################################
    // Prototypes
    // #############################################################################
private:
    /**
     * Updates the contents of this buffer (unless it is immutable).
     * The given data provider can possibly be ignored because the local data buffer
     * has already been updated based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _update_impl(const resources::DataProvider& data_provider, bool discard, bool no_overwrite) = 0;

}; // class MeshBuffer



} // namespace meshes
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_MESHES_MESHBUFFER_H
