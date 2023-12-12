#pragma once
#ifndef XL7_GRAPHICS_SHADERS_SHADERMANAGER_H
#define XL7_GRAPHICS_SHADERS_SHADERMANAGER_H
#include "../../ResourceManager.h"



namespace xl7 {
namespace graphics {
namespace shaders {



class ShaderManager
    : public ResourceManager
{

protected:
    static void _destroy(ShaderManager* shader_manager) { delete shader_manager; }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Default constructor.
     */
    ShaderManager();

    /**
     * Destructor.
     */
    virtual ~ShaderManager();

private:
    /** Copy constructor. */
    ShaderManager(const ShaderManager&) = delete;
    /** Copy assignment operator. */
    ShaderManager& operator = (const ShaderManager&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:



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
