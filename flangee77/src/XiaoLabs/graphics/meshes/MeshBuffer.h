#pragma once
#ifndef XL7_GRAPHICS_MESHES_MESHBUFFER_H
#define XL7_GRAPHICS_MESHES_MESHBUFFER_H
#include "../../Resource.h"

#include "./Topology.h"



namespace xl7 {
namespace graphics {
namespace meshes {



class MeshBuffer;
typedef std::shared_ptr<MeshBuffer> MeshBufferPtr;



class MeshManager;



class MeshBuffer
    : public Resource
{

public:
    struct Desc
    {
        /** Identifies how the buffer is expected to be updated (frequency of update is a key factor). */
        ResourceUsage usage;
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
    MeshBuffer(const CreateParams<TDesc>& params, unsigned stride)
        : Resource( params )
        , _desc( params.desc )
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
     * The descriptor of the buffer.
     */
    const Desc _desc;

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
     * Returns the descriptor of the buffer.
     */
    const Desc& get_desc() const { return _desc; }

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
    bool update(const DataProvider& data_provider);



    // #############################################################################
    // Resource Implementations
    // #############################################################################
private:
    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    virtual bool _check_impl(const DataProvider& data_provider) override;



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
    virtual bool _update_impl(const DataProvider& data_provider, bool discard, bool no_overwrite) = 0;

}; // class MeshBuffer



} // namespace meshes
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_MESHES_MESHBUFFER_H
