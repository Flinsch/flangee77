#pragma once
#ifndef XL7_GRAPHICS_TEXTURES_TEXTURE_H
#define XL7_GRAPHICS_TEXTURES_TEXTURE_H
#include "../../resources/Resource.h"

#include "../PixelFormat.h"
#include "../ChannelOrder.h"



namespace xl7 {
namespace graphics {
namespace textures {



class TextureManager;



class Texture
    : public resources::Resource
{

public:
    enum class Type
    {
        Texture2D,
        Texture3D,
        Texture2DArray,
        Cubemap,
    };

public:
    struct Desc
    {
        /** The pixel format. */
        PixelFormat pixel_format;
        /** The preferred channel order. (The actual channel order may vary depending on hardware capabilities.) */
        ChannelOrder preferred_channel_order;

        /** The width of the texture, in pixels. */
        unsigned width;
        /** The height of the texture, in pixels. */
        unsigned height;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    Texture(Type type, const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~Texture() = default;

private:
    /** Default constructor. */
    Texture() = delete;
    /** Copy constructor. */
    Texture(const Texture&) = delete;
    /** Copy assignment operator. */
    Texture& operator = (const Texture&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
protected:
    /**
     * The type of the texture.
     */
    const Type _type;

    /**
     * The descriptor of the texture.
     */
    const Desc _desc;

    /**
     * The size of each pixel, in bytes.
     */
    const unsigned _stride;

    /**
     * The size of a line (the offset between the start of one line and the start of
     * the next line), in bytes.
     */
    const unsigned _line_pitch;

    /**
     * The size of a 2D image (the offset between the start of one 2D image and the
     * start of the next 2D image if applicable), in bytes.
     */
    const unsigned _image_pitch;

private:
    /**
     * The actual channel order. This may differ from the preferred channel order,
     * depending on hardware capabilities.
     */
    ChannelOrder _channel_order;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the type of the texture.
     */
    Type get_type() const { return _type; }

    /**
     * Returns the descriptor of the texture.
     */
    const Desc& get_desc() const { return _desc; }

    /**
     * Returns the size of each pixel, in bytes.
     */
    unsigned get_stride() const { return _stride; }

    /**
     * Returns the size of a line (the offset between the start of one line and the
     * start of the next line), in bytes.
     */
    unsigned get_line_pitch() const { return _line_pitch; }

    /**
     * Returns the size of a 2D image (the offset between the start of one 2D image
     * and the start of the next 2D image if applicable), in bytes.
     */
    unsigned get_image_pitch() const { return _image_pitch; }

public:
    /**
     * Returns the actual channel order. This may differ from the preferred channel
     * order, depending on hardware capabilities.
     */
    ChannelOrder get_channel_order() const { return _channel_order; }



    // #############################################################################
    // Resource Implementations
    // #############################################################################
private:
    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    virtual bool _check_impl(const resources::DataProvider& data_provider) override;

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_impl(const resources::DataProvider& data_provider) override;

public:
    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    virtual cl7::string_view get_type_string() const override { return TEXT("texture"); }



    // #############################################################################
    // Prototypes
    // #############################################################################
private:
    /**
     * Requests/acquires a precompiled shader resource.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    virtual bool _acquire_impl(const resources::DataProvider& code_provider, ChannelOrder& channel_order_out) = 0;

}; // class Texture



} // namespace textures
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_TEXTURES_TEXTURE_H
