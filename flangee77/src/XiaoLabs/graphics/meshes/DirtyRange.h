#ifndef XL7_GRAPHICS_SHADERS_DIRTYRANGE_H
#define XL7_GRAPHICS_SHADERS_DIRTYRANGE_H

#include <CoreLabs/root.h>



namespace xl7::graphics::meshes {



class DirtyRange
{
public:
    bool is_dirty() const { return _element_count > 0; }

    void clear() { _first_element = _element_count = 0; }

    void update(unsigned first_element, unsigned element_count)
    {
        if (!is_dirty())
        {
            _first_element = first_element;
            _element_count = element_count;
            return;
        }

        unsigned begin = std::min(_first_element, first_element);
        unsigned end = std::max(_first_element + _element_count, first_element + element_count);
        _first_element = begin;
        _element_count = end - begin;
    }

    unsigned first_element() const { return _first_element; }
    unsigned element_count() const { return _element_count; }

private:
    unsigned _first_element = 0;
    unsigned _element_count = 0;
};



} // namespace xl7::graphics::meshes

#endif // XL7_GRAPHICS_SHADERS_DIRTYRANGE_H
