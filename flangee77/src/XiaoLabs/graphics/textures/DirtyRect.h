#ifndef XL7_GRAPHICS_TEXTURES_DIRTYRECT_H
#define XL7_GRAPHICS_TEXTURES_DIRTYRECT_H

#include <CoreLabs/root.h>



namespace xl7::graphics::textures {



class DirtyRect
{
public:
    bool is_dirty() const { return _width > 0 && _height > 0; }

    void clear()
    {
        _x = _y = _width = _height = 0;
    }

    void update(unsigned x, unsigned y, unsigned width, unsigned height)
    {
        if (!is_dirty())
        {
            _x = x;
            _y = y;
            _width = width;
            _height = height;
            return;
        }

        unsigned x0 = std::min(_x, x);
        unsigned y0 = std::min(_y, y);
        unsigned x1 = std::max(_x + _width, x + width);
        unsigned y1 = std::max(_y + _height, y + height);

        _x = x0;
        _y = y0;
        _width = x1 - x0;
        _height = y1 - y0;
    }

private:
    unsigned _x = 0;
    unsigned _y = 0;
    unsigned _width = 0;
    unsigned _height = 0;
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_DIRTYRECT_H
