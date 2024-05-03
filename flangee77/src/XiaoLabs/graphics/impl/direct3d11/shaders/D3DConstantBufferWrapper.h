#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_SHADERS_D3DCONSTANTBUFFERWRAPPER_H
#define XL7_GRAPHICS_IMPL_D3D11_SHADERS_D3DCONSTANTBUFFERWRAPPER_H

#include "../../../shaders/ConstantBufferLayout.h"
#include "../../../shaders/ConstantMapping.h"

#include "../prerequisites.h"

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_view.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace shaders {



class D3DConstantBufferWrapper
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Explicit constructor.
     */
    D3DConstantBufferWrapper(const xl7::graphics::shaders::ConstantBufferLayout& layout);

    /**
     * Destructor.
     */
    ~D3DConstantBufferWrapper();

    /** Move constructor. */
    D3DConstantBufferWrapper(D3DConstantBufferWrapper&&) = default;

    /** Move  assignment operator. */
    D3DConstantBufferWrapper& operator = (D3DConstantBufferWrapper&&) = default;

private:
    /** Default constructor. */
    D3DConstantBufferWrapper() = delete;
    /** Copy constructor. */
    D3DConstantBufferWrapper(const D3DConstantBufferWrapper&) = delete;
    /** Copy assignment operator. */
    D3DConstantBufferWrapper& operator = (const D3DConstantBufferWrapper&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The layout specification of the constant buffer.
     */
    const xl7::graphics::shaders::ConstantBufferLayout _layout;

    /**
     * The size of the constant buffer, in bytes.
     */
    const unsigned _size;

private:
    /**
     * The Direct3D 11 constant buffer interface.
     */
    wrl::ComPtr<ID3D11Buffer> _d3d_constant_buffer;

private:
    /**
     * The reference count to determine whether the constant buffer is in use.
     */
    unsigned _reference_count;

    /**
     * The local copy of the constant buffer data.
     */
    std::byte* _data;

    /**
     * The offset of the dirty data region, in bytes.
     */
    unsigned _dirty_offset;

    /**
     * The size of the dirty data region, in bytes.
     */
    unsigned _dirty_size;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the layout specification of the constant buffer.
     */
    const xl7::graphics::shaders::ConstantBufferLayout& get_layout() const { return _layout; }

    /**
     * Returns the size of the constant buffer, in bytes.
     */
    unsigned get_size() const { return _size; }

public:
    /**
     * Returns the Direct3D 11 constant buffer interface.
     */
    ID3D11Buffer* get_raw_d3d_constant_buffer() const { return _d3d_constant_buffer.Get(); }

public:
    /**
     * Returns the reference count to determine whether the constant buffer is in use.
     */
    unsigned get_reference_count() const { return _reference_count; }

    /**
     * Returns the local copy of the constant buffer data.
     */
    cl7::byte_view get_data() const { return cl7::byte_view( _data, _size ); }

    /**
     * Returns true if the local data of the constant buffer is "dirty".
     */
    bool is_dirty() const { return _dirty_size > 0; }



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Returns true if the constant buffer matches the given layout specification.
     */
    bool matches(const xl7::graphics::shaders::ConstantBufferLayout& layout) const;

    /**
     * Updates the local copy of the constant buffer data based on the given source
     * data and the specified constant mapping.
     */
    bool update(cl7::byte_view data, const xl7::graphics::shaders::ConstantMapping& constant_mapping);

    /**
     * Flushes any changes to the local copy of the constant buffer data to the
     * hardware.
     */
    bool flush();

    /**
     * Increases the reference count and returns the new value.
     */
    unsigned add_reference();

    /**
     * Decreases the reference count and returns the new value. When the reference
     * count reaches zero, the hardware buffer is not actually released yet: this
     * only happens when this object is destroyed.
     */
    unsigned release();

}; // class D3DConstantBufferWrapper



} // namespace shaders
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_SHADERS_D3DCONSTANTBUFFERWRAPPER_H
