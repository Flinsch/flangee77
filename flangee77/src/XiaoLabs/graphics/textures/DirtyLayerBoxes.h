#ifndef XL7_GRAPHICS_TEXTURES_DIRTYLAYERBOXES_H
#define XL7_GRAPHICS_TEXTURES_DIRTYLAYERBOXES_H

#include "./DirtyBox.h"

#include <vector>
#include <algorithm>



namespace xl7::graphics::textures {



class DirtyLayerBoxes
{
public:
    bool is_dirty() const
    {
        if (_all_dirty)
            return true;

        return std::ranges::any_of(_layer_boxes, [](const auto& box) {
            return box.is_dirty();
        });
    }

    bool is_all_dirty() const { return _all_dirty; }

    void clear()
    {
        _all_dirty = false;

        for (auto& box : _layer_boxes)
            box.clear();
    }

    void set_dirty()
    {
        _all_dirty = true;
    }

    void update(unsigned layer, unsigned x, unsigned y, unsigned z, unsigned width, unsigned height, unsigned depth)
    {
        if (is_all_dirty())
            return;

        if (width == 0 || height == 0 || depth == 0)
            return;

        // Just a quick plausibility check,
        // which assumes that arrays larger
        // than 256 have no practical use.
        assert(layer < 256);

        if (layer >= _layer_boxes.size())
            _layer_boxes.resize(layer + 1);

        _layer_boxes[layer].update(x, y, z, width, height, depth);
    }

    const auto& layer_boxes() const { return _layer_boxes; }

private:
    bool _all_dirty = false;

    std::vector<DirtyBox> _layer_boxes;
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_DIRTYLAYERBOXES_H
