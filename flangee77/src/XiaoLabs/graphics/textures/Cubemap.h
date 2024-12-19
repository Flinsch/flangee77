#pragma once
#ifndef XL7_GRAPHICS_TEXTURES_CUBEMAP_H
#define XL7_GRAPHICS_TEXTURES_CUBEMAP_H
#include "./Texture.h"



namespace xl7::graphics::textures {



class Cubemap // NOLINT(*-virtual-class-destructor)
    : public Texture
{

public:
    struct Desc
        : public Texture::Desc
    {
        
    };



    Cubemap() = delete;

    Cubemap(const Cubemap&) = delete;
    Cubemap& operator = (const Cubemap&) = delete;
    Cubemap(Cubemap&&) = delete;
    Cubemap& operator = (Cubemap&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::string_view get_type_string() const override { return TEXT("cubemap"); }

    /**
     * Returns the descriptor of the cubemap.
     */
    const Desc& get_desc() const { return _desc; }

    /**
     * Returns an "image view" of the specified texture face data.
     */
    images::Image as_image(unsigned face_index) const { return _as_image(face_index); }

    /**
     * Creates and returns mipmaps of the specified texture face "image".
     */
    std::vector<images::Image> create_mipmaps(unsigned face_index, images::ResamplingMethod resampling_method = images::ResamplingMethod::LinearInterpolation) const { return _create_mipmaps(face_index, resampling_method); }



protected:
    Cubemap(const CreateParams<Desc>& params);

    ~Cubemap() override = default;



private:
    /**
     * The descriptor of the cubemap.
     */
    const Desc _desc;

}; // class Cubemap



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_CUBEMAP_H
