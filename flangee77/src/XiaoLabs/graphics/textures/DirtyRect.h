#ifndef XL7_GRAPHICS_TEXTURES_DIRTYRECT_H
#define XL7_GRAPHICS_TEXTURES_DIRTYRECT_H

#include "./TextureRect.h"



namespace xl7::graphics::textures {



class DirtyRect
{
public:
    bool is_dirty() const { return _all_dirty || (_region.width > 0 && _region.height > 0); }

    bool is_all_dirty() const { return _all_dirty; }

    void clear()
    {
        _all_dirty = false;
        _region = TextureRect::zero();
    }

    void set_dirty()
    {
        _all_dirty = true;
    }

    void update(unsigned x, unsigned y, unsigned width, unsigned height)
    {
        update({
            .x = x,
            .y = y,
            .width = width,
            .height = height,
        });
    }

    void update(const TextureRect& region)
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
        const unsigned x1 = std::max(_region.x + _region.width, region.x + region.width);
        const unsigned y1 = std::max(_region.y + _region.height, region.y + region.height);

        _region.x = x0;
        _region.y = y0;
        _region.width = x1 - x0;
        _region.height = y1 - y0;
    }

    const TextureRect& region() const { return _region; }

    unsigned x() const { return _region.x; }
    unsigned y() const { return _region.y; }
    unsigned width() const { return _region.width; }
    unsigned height() const { return _region.height; }

private:
    bool _all_dirty = false;
    TextureRect _region = TextureRect::zero();
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_DIRTYRECT_H
