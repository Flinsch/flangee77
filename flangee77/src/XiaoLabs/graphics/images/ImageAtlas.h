#ifndef XL7_GRAPHICS_IMAGES_IMAGEATLAS_H
#define XL7_GRAPHICS_IMAGES_IMAGEATLAS_H

#include "./Image.h"

#include <AlgoLabs/packing/AbstractRectPacker.h>

#include <CoreLabs/byte_vector.h>



namespace xl7::graphics::images {



/**
 * Represents a 2D atlas of images with shared format but arbitrary dimensions.
 * Images are individually positioned within a packed rectangular space.
 */
class ImageAtlas
{

public:

    struct Entry
    {
        /** Specifies the position within the atlas. */
        al7::packing::Rect rect;
        /** Specifies whether the width and height have been swapped for positioning. */
        bool flipped;
        /** Indicates whether the image was successfully positioned using a rect packer. */
        bool valid;
        /** The image data, in its original width-by-height layout, i.e., without taking into account any possible swapping of width and height. */
        cl7::byte_vector image_data;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    ImageAtlas();
    ImageAtlas(PixelFormat pixel_format, ChannelOrder channel_order, al7::packing::AbstractRectPacker* default_rect_packer = nullptr);



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the pixel format of the images.
     */
    PixelFormat get_pixel_format() const { return _pixel_format; }

    /**
     * Returns the channel order of the images.
     */
    ChannelOrder get_channel_order() const { return _channel_order; }

    /**
     * Returns the number of contained images.
     */
    unsigned get_image_count() const { return static_cast<unsigned>(_entries.size()); }

    /**
     * Returns the atlas entries with their respective positions, image data, etc.
     */
    const std::vector<Entry>& get_entries() const { return _entries; }

    /**
     * Returns true if all images were successfully positioned using a rect packer.
     */
    bool is_valid() const;

    /**
     * Returns the default rect packer to be used to directly position newly added
     * images incrementally.
     */
    al7::packing::AbstractRectPacker* get_default_rect_packer() const { return _default_rect_packer; }

    /**
     * Sets the default rect packer to be used to directly position newly added
     * images incrementally.
     */
    void set_default_rect_packer(al7::packing::AbstractRectPacker* default_rect_packer) { _default_rect_packer = default_rect_packer; }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * (Re)initializes the image atlas.
     * The specified default rect packer is used to directly position newly added
     * images incrementally. If not specified, no auto-positioning will take place.
     */
    void init(PixelFormat pixel_format, ChannelOrder channel_order, al7::packing::AbstractRectPacker* default_rect_packer = nullptr);

    /**
     * Adds the specified image to this image atlas by copying its data.
     * Returns false if the image does not confirm to the common pixel format and
     * channel order, or if the default rect packer, if specified, could not insert
     * a corresponding item.
     */
    bool add_image(Image image);

    /**
     * Adds the specified image data to this image atlas by copying the data.
     * Returns false if the image data does not confirm to the specified image size
     * or if the default rect packer, if specified, could not insert a corresponding
     * item.
     */
    bool add_image_data(unsigned width, unsigned height, cl7::byte_vector image_data);

    /**
     * Returns the data of the specified image.
     */
    cl7::byte_view get_image_data(unsigned image_index) const;

    /**
     * Returns the descriptor of the specified image.
     */
    ImageDesc get_image_desc(unsigned image_index) const;

    /**
     * Returns the specified image as a "view" of the image data.
     */
    Image get_image(unsigned image_index) const;

    /**
     * Extracts the specified image by creating a new one and copying the data.
     */
    Image extract_image(unsigned image_index) const;

    /**
     * Rearranges the contained images. If no rect packer is specified, the default
     * packer is used. Returns false if the effectively used rect packer could not
     * successfully process all images.
     */
    bool rearrange_images(al7::packing::AbstractRectPacker* rect_packer = nullptr);

    /**
     * Calculates the required atlas size for the successfully positioned images,
     * including estimates for any possible padding. If in doubt, the specified size
     * of the rect packer should be used.
     */
    al7::packing::Size estimate_size() const;



private:

    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Attempts to update the positional information of an entry based on the
     * specified rect result. If the rect result is "empty", false is returned.
     * In any case, the "valid" flag of the entry will be updated accordingly.
     */
    static bool _try_update_entry(Entry& entry, const std::optional<al7::packing::Rect>& rect_result);



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The pixel format of the images.
     */
    PixelFormat _pixel_format;

    /**
     * The channel order of the images.
     */
    ChannelOrder _channel_order;

    /**
     * The atlas entries with their respective positions, image data, etc.
     */
    std::vector<Entry> _entries;

    /**
     * The default rect packer to be used to directly position newly added images
     * incrementally.
     */
    al7::packing::AbstractRectPacker* _default_rect_packer = nullptr;

}; // class ImageAtlas



} // namespace xl7::graphics::images

#endif // XL7_GRAPHICS_IMAGES_IMAGEATLAS_H
