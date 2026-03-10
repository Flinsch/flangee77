#ifndef XL7_GRAPHICS_SHADERS_DIRTYRANGE_H
#define XL7_GRAPHICS_SHADERS_DIRTYRANGE_H

#include <CoreLabs/root.h>



namespace xl7::graphics::meshes {



class DirtyRange
{
public:
    bool is_dirty() const { return _count > 0; }

    void clear() { _first = _count = 0; }

    void update(unsigned first, unsigned count)
    {
        unsigned begin = std::min(_first, first);
        unsigned end = std::max(_first + _count, first + count);
        _first = begin;
        _count = end - begin;
    }

private:
    unsigned _first = 0;
    unsigned _count = 0;
};



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_SHADERS_DIRTYRANGE_H
