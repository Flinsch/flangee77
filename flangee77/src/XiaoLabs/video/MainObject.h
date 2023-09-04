#pragma once
#ifndef XL7_VIDEO_MAINOBJECT_H
#define XL7_VIDEO_MAINOBJECT_H
#include "../Component.h"



#define XL7_VIDEO_IMPL_DIRECT3D9    0x6b8f8219
#define XL7_VIDEO_IMPL_DIRECT3D11   0x9b09a2b4

#define XL7_VIDEO_IMPL_DEFAULT      XL7_VIDEO_IMPL_DIRECT3D11

#ifndef XL7_VIDEO_IMPL
#define XL7_VIDEO_IMPL              XL7_VIDEO_IMPL_DEFAULT
#endif // !XL7_VIDEO_IMPL



namespace xl7 {
namespace video {



class MainObject
    : public Component<MainObject>
{
    friend cl7::creational::Singleton<MainObject>;

    // #############################################################################
    // Factory Function
    // #############################################################################
protected:
    static MainObject* factory_func();



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Default constructor.
     */
    MainObject(void) = default;

    /**
     * Destructor.
     */
    virtual ~MainObject(void) override = default;

private:
    /** Copy constructor. */
    MainObject(const MainObject&) = delete;
    /** Copy assignment operator. */
    MainObject& operator = (const MainObject&) = delete;



    // #############################################################################
    // Component Implementations
    // #############################################################################
private:
    /**
     * Initializes the component.
     */
    virtual bool _init() final;

    /**
     * De-initializes the component.
     */
    virtual bool _shutdown() final;



    // #############################################################################
    // Prototypes
    // #############################################################################
private:
    /**
     * Initializes the component.
     */
    virtual bool _init_without_logging_final_result() = 0;

    /**
     * De-initializes the component.
     */
    virtual bool _shutdown_without_logging_final_result() = 0;

}; // class MainObject



} // namespace video
} // namespace xl7

#endif // XL7_VIDEO_MAINOBJECT_H
