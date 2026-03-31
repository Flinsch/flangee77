#ifndef XL7_GRAPHICS_SHADERS_DIRTYFLAG_H
#define XL7_GRAPHICS_SHADERS_DIRTYFLAG_H

#include <CoreLabs/root.h>



namespace xl7::graphics::shaders {



class DirtyFlag
{
public:
    bool is_dirty() const { return _all_dirty; }

    bool is_all_dirty() const { return _all_dirty; }

    void clear()
    {
        _all_dirty = false;
    }

    void set_dirty()
    {
        _all_dirty = true;
    }

    void update(bool dirty)
    {
        _all_dirty |= dirty;
    }

private:
    bool _all_dirty = false;
};



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_DIRTYFLAG_H
