#ifndef XL7_GRAPHICS_TEXTURES_DIRTYLAYERRECTS_H
#define XL7_GRAPHICS_TEXTURES_DIRTYLAYERRECTS_H

#include "./DirtyRect.h"

#include <vector>
#include <algorithm>



namespace xl7::graphics::textures {



class DirtyLayerRects
{
public:
    bool is_dirty() const
    {
        return std::ranges::any_of(_layer_rects, [](const auto& rect) {
            return rect.is_dirty();
        });
    }

    void clear()
    {
        for (auto& rect : _layer_rects)
            rect.clear();
    }

    void update(unsigned layer, unsigned x, unsigned y, unsigned width, unsigned height)
    {
        if (width == 0 || height == 0)
            return;

        // Just a quick plausibility check,
        // which assumes that arrays larger
        // than 256 have no practical use.
        assert(layer < 256);

        if (layer >= _layer_rects.size())
            _layer_rects.resize(layer + 1);

        _layer_rects[layer].update(x, y, width, height);
    }

    const auto& layer_rects() const { return _layer_rects; }

private:
    std::vector<DirtyRect> _layer_rects;
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_DIRTYLAYERRECTS_H
