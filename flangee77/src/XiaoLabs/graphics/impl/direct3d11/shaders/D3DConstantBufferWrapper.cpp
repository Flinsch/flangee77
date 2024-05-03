#include "D3DConstantBufferWrapper.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../RenderingContextImpl.h"
#include "../errors.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace shaders {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    D3DConstantBufferWrapper::D3DConstantBufferWrapper(const xl7::graphics::shaders::ConstantBufferLayout& layout)
        : _layout( layout )
        , _size( (_layout.calculate_size() + 15) / 16 * 16 ) // Ensure a multiple of 16.
        , _d3d_constant_buffer( nullptr )
        , _reference_count( 0 )
        , _data( nullptr )
        , _dirty_offset( 0 )
        , _dirty_size( 0 )
    {
        // https://developer.nvidia.com/content/constant-buffers-without-constant-pain-0
        _data = (std::byte*)_aligned_malloc( static_cast<size_t>( _size ), 16 );
        assert( _data );

        auto d3d_device = static_cast<RenderingDeviceImpl*>( GraphicsSystem::instance().get_rendering_device() )->get_raw_d3d_device();
        assert( d3d_device );

        D3D11_BUFFER_DESC buffer_desc;
        buffer_desc.ByteWidth = _size;
        buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
        buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        buffer_desc.MiscFlags = 0;
        buffer_desc.StructureByteStride = 0;

        HRESULT hresult = d3d_device->CreateBuffer(
            &buffer_desc,
            nullptr,
            &_d3d_constant_buffer );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11Device::CreateBuffer") ) );
            return;
        }
    }

    /**
     * Destructor.
     */
    D3DConstantBufferWrapper::~D3DConstantBufferWrapper()
    {
        if ( _data )
            _aligned_free( _data );
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Returns true if the constant buffer matches the given layout specification.
     */
    bool D3DConstantBufferWrapper::matches(const xl7::graphics::shaders::ConstantBufferLayout& layout) const
    {
        // Should we force a 1-to-1 correspondence, or is it enough if
        // this constant buffer at least covers the requested layout?
        if ( layout.constant_declarations.size() != _layout.constant_declarations.size() )
            return false;

        for ( const auto& constant_declaration : layout.constant_declarations )
        {
            bool found = false;

            for ( const auto& candidate_declaration : _layout.constant_declarations )
            {
                if ( candidate_declaration.offset != constant_declaration.offset )
                    continue;
                if ( candidate_declaration.size != constant_declaration.size )
                    continue;

                if ( candidate_declaration.constant_type != constant_declaration.constant_type )
                    continue;
                if ( candidate_declaration.constant_class != constant_declaration.constant_class )
                    continue;

                if ( candidate_declaration.name != constant_declaration.name )
                    continue;

                assert( candidate_declaration.row_count == constant_declaration.row_count );
                assert( candidate_declaration.column_count == constant_declaration.column_count );
                assert( candidate_declaration.element_count == constant_declaration.element_count );

                found = true;
                break;
            }

            if ( !found )
                return false;
        }

        return true;
    }

    /**
     * Updates the local copy of the constant buffer data based on the given source
     * data and the specified constant mapping.
     */
    bool D3DConstantBufferWrapper::update(cl7::byte_view data, const xl7::graphics::shaders::ConstantMapping& constant_mapping)
    {
        assert( constant_mapping.size > 0 );
        if ( constant_mapping.size == 0 )
            return true;

        assert( data.size() >= static_cast<size_t>( constant_mapping.source_offset + constant_mapping.size ) );
        assert( _size >= static_cast<size_t>( constant_mapping.shader_offset + constant_mapping.size ) );

        if ( ::memcmp( _data + constant_mapping.shader_offset, data.data() + constant_mapping.source_offset, constant_mapping.size ) == 0 )
            return true;

        ::memcpy( _data + constant_mapping.shader_offset, data.data() + constant_mapping.source_offset, constant_mapping.size );

        if ( _dirty_size == 0 )
        {
            _dirty_offset = constant_mapping.shader_offset;
            _dirty_size = constant_mapping.size;
        }
        else
        {
            if ( (constant_mapping.shader_offset + constant_mapping.size) > (_dirty_offset + _dirty_size) )
                _dirty_size += (constant_mapping.shader_offset + constant_mapping.size) - (_dirty_offset + _dirty_size);

            if ( constant_mapping.shader_offset < _dirty_offset )
            {
                _dirty_size += _dirty_offset - constant_mapping.shader_offset;
                _dirty_offset = constant_mapping.shader_offset;
            }
        }

        assert( _dirty_size > 0 );
        return true;
    }

    /**
     * Flushes any changes to the local copy of the constant buffer data to the
     * hardware.
     */
    bool D3DConstantBufferWrapper::flush()
    {
        if ( _dirty_size == 0 )
            return true;

        auto d3d_device_context = static_cast<RenderingContextImpl*>( GraphicsSystem::instance().get_rendering_device()->get_primary_context() )->get_raw_d3d_device_context();
        assert( d3d_device_context );

        D3D11_MAPPED_SUBRESOURCE mapped_subresource;
        HRESULT hresult = d3d_device_context->Map( _d3d_constant_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11DeviceContext::Map") ) );
            LOG_ERROR( TEXT("The constant buffer could not be mapped for writing.") );
            return false;
        }

        ::memcpy( mapped_subresource.pData, _data, _size );

        d3d_device_context->Unmap( _d3d_constant_buffer.Get(), 0 );

        _dirty_size = 0;
        return true;
    }

    /**
     * Increases the reference count and returns the new value.
     */
    unsigned D3DConstantBufferWrapper::add_reference()
    {
        return ++_reference_count;
    }

    /**
     * Decreases the reference count and returns the new value. When the reference
     * count reaches zero, the hardware buffer is not actually released yet: this
     * only happens when this object is destroyed.
     */
    unsigned D3DConstantBufferWrapper::release()
    {
        assert( _reference_count > 0 );
        if ( _reference_count > 0 )
            --_reference_count;
        return _reference_count;
    }



} // namespace shaders
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
