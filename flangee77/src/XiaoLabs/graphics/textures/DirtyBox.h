#ifndef XL7_GRAPHICS_TEXTURES_DIRTYBOX_H
#define XL7_GRAPHICS_TEXTURES_DIRTYBOX_H

#include "./TextureBox.h"



namespace xl7::graphics::textures {



class DirtyBox
{
public:
    bool is_dirty() const { return _all_dirty || (_region.width > 0 && _region.height > 0 && _region.depth > 0); }

    bool is_all_dirty() const { return _all_dirty; }

    void clear()
    {
        _all_dirty = false;
        _region = TextureBox::zero();
    }

    void set_dirty()
    {
        _all_dirty = true;
    }

    void update(unsigned x, unsigned y, unsigned z, unsigned width, unsigned height, unsigned depth)
    {
        update({
            .x = x,
            .y = y,
            .z = z,
            .width = width,
            .height = height,
            .depth = depth,
        });
    }

    void update(const TextureBox& region)
    {
        if (is_all_dirty())
            return;

        if (!is_dirty())
        {
            _region = region;
            return;
        }

        const unsigned x0 = std::min(_region.x, region.x);
        const unsigned y0 = std::min(_region.y, region.y);
        const unsigned z0 = std::min(_region.z, region.z);

        const unsigned x1 = std::max(_region.x + _region.width, region.x + region.width);
        const unsigned y1 = std::max(_region.y + _region.height, region.y + region.height);
        const unsigned z1 = std::max(_region.z + _region.depth, region.z + region.depth);

        _region.x = x0;
        _region.y = y0;
        _region.z = z0;

        _region.width = x1 - x0;
        _region.height = y1 - y0;
        _region.depth = z1 - z0;
    }

    const TextureBox& region() const { return _region; }

    unsigned x() const { return _region.x; }
    unsigned y() const { return _region.y; }
    unsigned z() const { return _region.z; }

    unsigned width() const { return _region.width; }
    unsigned height() const { return _region.height; }
    unsigned depth() const { return _region.depth; }

private:
    bool _all_dirty = false;
    TextureBox _region = TextureBox::zero();
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_DIRTYBOX_H
