#ifndef XL7_GRAPHICS_TEXTURES_DIRTYBOX_H
#define XL7_GRAPHICS_TEXTURES_DIRTYBOX_H

#include <CoreLabs/root.h>



namespace xl7::graphics::textures {



class DirtyBox
{
public:
    bool is_dirty() const { return _width > 0 && _height > 0 && _depth > 0; }

    void clear()
    {
        _x = _y = _z = 0;
        _width = _height = _depth = 0;
    }

    void update(unsigned x, unsigned y, unsigned z, unsigned width, unsigned height, unsigned depth)
    {
        if (!is_dirty())
        {
            _x = x;
            _y = y;
            _z = z;
            _width = width;
            _height = height;
            _depth = depth;
            return;
        }

        unsigned x0 = std::min(_x, x);
        unsigned y0 = std::min(_y, y);
        unsigned z0 = std::min(_z, z);

        unsigned x1 = std::max(_x + _width, x + width);
        unsigned y1 = std::max(_y + _height, y + height);
        unsigned z1 = std::max(_z + _depth, z + depth);

        _x = x0;
        _y = y0;
        _z = z0;

        _width = x1 - x0;
        _height = y1 - y0;
        _depth = z1 - z0;
    }

    unsigned x() const { return _x; }
    unsigned y() const { return _y; }
    unsigned z() const { return _z; }

    unsigned width() const { return _width; }
    unsigned height() const { return _height; }
    unsigned depth() const { return _depth; }

private:
    unsigned _x = 0;
    unsigned _y = 0;
    unsigned _z = 0;

    unsigned _width = 0;
    unsigned _height = 0;
    unsigned _depth = 0;
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_DIRTYBOX_H
