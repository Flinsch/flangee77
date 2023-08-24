#pragma once
#ifndef XL7_VIDEO_MAINOBJECT_H
#define XL7_VIDEO_MAINOBJECT_H
#include "../Component.h"



namespace xl7 {
namespace video {



class MainObject final
    : public Component<MainObject>
{
    friend cl7::creational::Singleton<MainObject>;

    // #############################################################################
    // Factory Function
    // #############################################################################
private:
    static MainObject* factory_func();



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Default constructor.
     */
    MainObject(void);

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
    // Component Implementations
    // #############################################################################
private:
    /**
     * Initializes the component.
     */
    virtual bool _init() override;

    /**
     * De-initializes the component.
     */
    virtual bool _shutdown() override;



    // #############################################################################
    // Helpers
    // #############################################################################
private:

}; // class MainObject



} // namespace video
} // namespace xl7

#endif // XL7_VIDEO_MAINOBJECT_H
