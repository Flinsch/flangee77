#pragma once
#ifndef XL7_VIDEO_IMPL_D3D11_MAINOBJECTIMPL_H
#define XL7_VIDEO_IMPL_D3D11_MAINOBJECTIMPL_H
#include "../../MainObject.h"

#include <dxgi.h>
#include <d3d11.h>

#include <wrl.h>
namespace wrl = Microsoft::WRL;



namespace xl7 {
namespace video {
namespace impl {
namespace direct3d11 {



class MainObjectImpl final
    : public MainObject
{
    friend MainObject* MainObject::factory_func();



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Default constructor.
     */
    MainObjectImpl(void);

    /**
     * Destructor.
     */
    virtual ~MainObjectImpl(void) override = default;

private:
    /** Copy constructor. */
    MainObjectImpl(const MainObjectImpl&) = delete;
    /** Copy assignment operator. */
    MainObjectImpl& operator = (const MainObjectImpl&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The DXGI factory interface.
     */
    wrl::ComPtr<IDXGIFactory> _dxgi_factory;



    // #############################################################################
    // Properties
    // #############################################################################
public:



    // #############################################################################
    // Methods
    // #############################################################################
public:



    // #############################################################################
    // MainObject Implementations
    // #############################################################################
private:
    /**
     * Initializes the component.
     */
    virtual bool _init_without_logging_final_result() override;

    /**
     * De-initializes the component.
     */
    virtual bool _shutdown_without_logging_final_result() override;



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Creates the DXGI factory interface.
     */
    bool _create_dxgi_factory();

    /**
     * Releases the DXGI factory interface.
     */
    bool _release_dxgi_factory();

}; // class MainObjectImpl



} // namespace direct3d11
} // namespace impl
} // namespace video
} // namespace xl7

#endif // XL7_VIDEO_IMPL_D3D11_MAINOBJECTIMPL_H
