#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_SHADERS_D3DCONSTANTBUFFERWRAPPER_H
#define XL7_GRAPHICS_IMPL_D3D11_SHADERS_D3DCONSTANTBUFFERWRAPPER_H

#include "../../../shaders/ConstantBufferLayout.h"
#include "../../../shaders/ConstantMapping.h"

#include "../prerequisites.h"

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_view.h>



namespace xl7::graphics::impl::direct3d11::shaders {



class D3DConstantBufferWrapper
{

public:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    D3DConstantBufferWrapper() = delete;

    D3DConstantBufferWrapper(xl7::graphics::shaders::ConstantBufferLayout layout);

    D3DConstantBufferWrapper(const D3DConstantBufferWrapper&) = delete;
    D3DConstantBufferWrapper& operator = (const D3DConstantBufferWrapper&) = delete;

    D3DConstantBufferWrapper(D3DConstantBufferWrapper&&) = default;
    D3DConstantBufferWrapper& operator = (D3DConstantBufferWrapper&&) = default;

    ~D3DConstantBufferWrapper();



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the layout specification of the constant buffer.
     */
    const xl7::graphics::shaders::ConstantBufferLayout& get_layout() const { return _layout; }

    /**
     * Returns the size of the constant buffer, in bytes.
     */
    unsigned get_size() const { return _size; }

    /**
     * Returns the Direct3D 11 constant buffer interface.
     */
    ID3D11Buffer* get_raw_d3d_constant_buffer() const { return _d3d_constant_buffer.Get(); }

    /**
     * Returns the reference count to determine whether the constant buffer is in use.
     */
    unsigned get_reference_count() const { return _reference_count; }

    /**
     * Returns the local copy of the constant buffer data.
     */
    cl7::byte_view get_data() const { return {_data, _size}; }

    /**
     * Returns true if the local data of the constant buffer is "dirty".
     */
    bool is_dirty() const { return _dirty_size > 0; }



    // #############################################################################
    // Methods
    // #############################################################################

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



private:

    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The layout specification of the constant buffer.
     */
    xl7::graphics::shaders::ConstantBufferLayout _layout;

    /**
     * The size of the constant buffer, in bytes.
     */
    unsigned _size;

    /**
     * The Direct3D 11 constant buffer interface.
     */
    wrl::ComPtr<ID3D11Buffer> _d3d_constant_buffer;

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

}; // class D3DConstantBufferWrapper



} // namespace xl7::graphics::impl::direct3d11::shaders

#endif // XL7_GRAPHICS_IMPL_D3D11_SHADERS_D3DCONSTANTBUFFERWRAPPER_H
