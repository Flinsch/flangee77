#pragma once
#ifndef XL7_VIDEO_IMPL_D3D9_MAINOBJECTIMPL_H
#define XL7_VIDEO_IMPL_D3D9_MAINOBJECTIMPL_H
#include "../../MainObject.h"

#include <d3d9.h>

#include <wrl.h>
namespace wrl = Microsoft::WRL;



namespace xl7 {
namespace video {
namespace impl {
namespace direct3d9 {



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
     * The Direct3D 9 main interface.
     */
    wrl::ComPtr<IDirect3D9> _d3d_interface;



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
     * Creates the Direct3D 9 main interface.
     */
    bool _create_main_interface();

    /**
     * Releases the Direct3D 9 main interface.
     */
    bool _release_main_interface();

}; // class MainObjectImpl



} // namespace direct3d9
} // namespace impl
} // namespace video
} // namespace xl7

#endif // XL7_VIDEO_IMPL_D3D9_MAINOBJECTIMPL_H
