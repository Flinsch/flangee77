#pragma once
#ifndef XL7_GRAPHICS_SHADERS_SHADERMANAGER_H
#define XL7_GRAPHICS_SHADERS_SHADERMANAGER_H
#include "../../ResourceManager.h"

#include "./IShaderFactory.h"



namespace xl7 {
namespace graphics {
    class RenderingDevice;
namespace shaders {



class ShaderManager
    : public ResourceManager
{

public:
    class Attorney
    {
        static ShaderManager* create(IShaderFactory* factory) { return new ShaderManager( factory ); }
        static void destroy(ShaderManager* manager) { delete manager; }
        friend class RenderingDevice;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    ShaderManager(IShaderFactory* factory);

    /**
     * Destructor.
     */
    virtual ~ShaderManager();

private:
    /** Default constructor. */
    ShaderManager() = delete;
    /** Copy constructor. */
    ShaderManager(const ShaderManager&) = delete;
    /** Copy assignment operator. */
    ShaderManager& operator = (const ShaderManager&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The shader factory.
     */
    IShaderFactory* const _factory;



    // #############################################################################
    // Methods
    // #############################################################################
public:



    // #############################################################################
    // Prototypes
    // #############################################################################
private:

}; // class ShaderManager



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_SHADERMANAGER_H
