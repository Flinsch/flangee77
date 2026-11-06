#ifndef FL7_GUI_FACES_PICTUREBOX_H
#define FL7_GUI_FACES_PICTUREBOX_H
#include "../Static.h"
#include "../HasBackground.h"
#include "../HasImage.h"

#include <optional>



namespace fl7::gui::faces {



/**
 * A display-only image, shown per get_image_scale_mode() (default: Stretch; see
 * ImageScaleMode): distorted to exactly fill this face's own bounds, uniformly
 * scaled (preserving its own aspect ratio) to fit within them, or drawn at its own
 * native size, centered. Also HasBackground, so a themed fill/chrome shows through
 * wherever the image itself doesn't fully cover this face's own bounds
 * (Zoom's/Center's letterboxing), same as any other boxed face's.
 */
class PictureBox
    : public Static
    , public HasBackground
    , public HasImage
{

public:
    PictureBox() = default;

    explicit PictureBox(Image image)
        : _image(std::move(image))
    {
    }

    PictureBox(const PictureBox&) = delete;
    PictureBox& operator=(const PictureBox&) = delete;
    PictureBox(PictureBox&&) = delete;
    PictureBox& operator=(PictureBox&&) = delete;

    ~PictureBox() override = default;



    // #############################################################################
    // HasImage Implementations
    // #############################################################################

    /** Returns this picture box's image, or nullptr if none is currently set; see set_image(). */
    const Image* get_image() const override { return _image ? &*_image : nullptr; }

    /** Returns this picture box's scale mode; see set_image_scale_mode(). Default: Stretch. */
    ImageScaleMode get_image_scale_mode() const override { return _scale_mode; }



    // #############################################################################
    // Properties
    // #############################################################################

    /** Sets this picture box's image, or clears it (nullopt), unless already that. */
    void set_image(std::optional<Image> image) { _image = std::move(image); }

    /** Sets this picture box's scale mode, unless already that; see ImageScaleMode. */
    void set_image_scale_mode(ImageScaleMode scale_mode) { _scale_mode = scale_mode; }



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /** Returns this picture box's theme role. */
    cl7::u8string_view _get_theme_key() const override { return u8"picture_box"; }



private:

    // #############################################################################
    // Attributes
    // #############################################################################

    std::optional<Image> _image;

    ImageScaleMode _scale_mode = ImageScaleMode::Stretch;

}; // class PictureBox



} // namespace fl7::gui::faces

#endif // FL7_GUI_FACES_PICTUREBOX_H
