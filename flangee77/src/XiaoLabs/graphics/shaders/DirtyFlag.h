#ifndef XL7_GRAPHICS_SHADERS_DIRTYFLAG_H
#define XL7_GRAPHICS_SHADERS_DIRTYFLAG_H

#include <CoreLabs/root.h>



namespace xl7::graphics::shaders {



class DirtyFlag
{
public:
    bool is_dirty() const { return _dirty; }

    void clear() { _dirty = false; }

    void update(bool dirty)
    {
        _dirty |= dirty;
    }

private:
    bool _dirty = false;
};



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_DIRTYFLAG_H
