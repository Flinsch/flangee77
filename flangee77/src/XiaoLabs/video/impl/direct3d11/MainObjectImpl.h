#pragma once
#ifndef XL7_VIDEO_IMPL_D3D11_MAINOBJECTIMPL_H
#define XL7_VIDEO_IMPL_D3D11_MAINOBJECTIMPL_H
#include "../../MainObject.h"



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

}; // class MainObjectImpl



} // namespace direct3d11
} // namespace impl
} // namespace video
} // namespace xl7

#endif // XL7_VIDEO_IMPL_D3D11_MAINOBJECTIMPL_H
