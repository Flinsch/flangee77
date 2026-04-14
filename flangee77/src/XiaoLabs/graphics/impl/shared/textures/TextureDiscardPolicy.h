#ifndef XL7_GRAPHICS_IMPL_SHARED_TEXTURES_TEXTUREDISCARDPOLICY_H
#define XL7_GRAPHICS_IMPL_SHARED_TEXTURES_TEXTUREDISCARDPOLICY_H

#include "../../../textures/TextureExtent.h"
#include "../../../textures/TextureRegion.h"

#include "../../../textures/TextureUsage.h"



namespace xl7::graphics::impl::shared::textures {



/**
 * Decides whether a hardware texture update should discard the entire pixel data
 * instead of updating only the dirty region.
 */
template <typename TExtent = graphics::textures::TextureExtent, typename TRegion = graphics::textures::TextureRegion>
class TextureDiscardPolicy
{
public:
    using Extent = TExtent;
    using Region = TRegion;

    struct DiscardThresholds
    {
        /** Textures smaller than this size (in bytes) are always discarded. */
        unsigned small_texture_threshold = 64 * 1024; // 64 KiB
        /** Discards if the dirty region exceeds this fraction of the total pixel count. */
        float large_dirty_fraction = 0.5f; // 50 %
        /** Discards dynamic textures if the dirty region exceeds this fraction of the total pixel count. */
        float dynamic_dirty_fraction = 0.35f; // 35 %
    };

    struct UpdateRecommendation
    {
        /** The recommended update region, in pixels. */
        Region region;
        /** Whether to discard the entire texture data. */
        bool discard;
    };



    TextureDiscardPolicy() = default;
    explicit TextureDiscardPolicy(const DiscardThresholds& thresholds) : _thresholds(thresholds) {}



    /**
     * Returns the recommended update region and whether this would mean discarding
     * all texture data.
     */
    UpdateRecommendation recommend(bool all_dirty, const Region& dirty_region, const Extent& texture_extent, unsigned bytes_per_pixel, graphics::textures::TextureUsage usage) const
    {
        if (all_dirty || should_discard(dirty_region, texture_extent, bytes_per_pixel, usage))
            return {.region = Region::full(texture_extent), .discard = true};

        return {.region = dirty_region, .discard = false};
    }

    /**
     * Returns true if the texture data should be fully discarded instead of only
     * updating the specified dirty region.
     */
    bool should_discard(const Region& dirty_region, const Extent& texture_extent, unsigned bytes_per_pixel, graphics::textures::TextureUsage usage) const
    {
        const auto dirty_bytes = static_cast<size_t>(bytes_per_pixel) * dirty_region.to_extent().volume();
        const auto total_bytes = static_cast<size_t>(bytes_per_pixel) * texture_extent.volume();

        if (dirty_bytes >= total_bytes)
            return true;

        if (total_bytes < _thresholds.small_texture_threshold)
            return true;

        const float dirty_fraction = static_cast<float>(dirty_bytes) / static_cast<float>(total_bytes);

        if (dirty_fraction >= _thresholds.large_dirty_fraction)
            return true;

        if (usage == graphics::textures::TextureUsage::Dynamic &&
            dirty_fraction >= _thresholds.dynamic_dirty_fraction)
            return true;

        return false;
    }



private:
    DiscardThresholds _thresholds{};

}; // class TextureDiscardPolicy



} // namespace xl7::graphics::impl::shared::textures

#endif // XL7_GRAPHICS_IMPL_SHARED_TEXTURES_TEXTUREDISCARDPOLICY_H
