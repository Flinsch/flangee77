#include "MsdfRasterizer.h"

#include "./detail/glyph_geometry.h"

#include <XiaoLabs/graphics/PixelFormat.h>

#include <CoreLabs/bits.h>

#include <algorithm>
#include <bit>
#include <limits>



namespace fl7::fonts::raster {



    namespace {

        constexpr uint8_t CHANNEL_R = 1;
        constexpr uint8_t CHANNEL_G = 2;
        constexpr uint8_t CHANNEL_B = 4;

        constexpr uint8_t COLOR_YELLOW  = CHANNEL_R | CHANNEL_G;
        constexpr uint8_t COLOR_CYAN    = CHANNEL_G | CHANNEL_B;
        constexpr uint8_t COLOR_MAGENTA = CHANNEL_B | CHANNEL_R;
        constexpr uint8_t COLOR_WHITE   = CHANNEL_R | CHANNEL_G | CHANNEL_B;

        struct RGBA8
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        };

        uint8_t _encode_channel(float signed_distance, float pixel_range)
        {
            const float normalized = ml7::clamp01(0.5f + signed_distance / pixel_range);
            return static_cast<uint8_t>(normalized * 255.0f);
        }

    } // namespace



    /**
     * Constructs an MSDF rasterizer. `pixel_range` is the distance, in pixels, that
     * maps to the full encoded [0, 1] output range; distances beyond it saturate to
     * fully inside/outside.
     */
    MsdfRasterizer::MsdfRasterizer(float pixel_range) noexcept
        : AbstractRasterizer(xl7::graphics::PixelFormat::R8G8B8A8_UNORM)
        , _pixel_range(pixel_range)
    {
    }



    std::vector<std::vector<MsdfRasterizer::ColoredSegment>> MsdfRasterizer::_color_contours(const std::vector<std::vector<ml7::QuadraticBezier2f>>& contours)
    {
        // Angle threshold (as a tangent dot product) beyond which two consecutive
        // segments are considered to meet at a corner rather than a smooth join.
        constexpr float corner_dot_threshold = 0.9986f; // ~3 degrees

        constexpr uint8_t colors[3] = {COLOR_YELLOW, COLOR_CYAN, COLOR_MAGENTA};

        std::vector<std::vector<ColoredSegment>> result;
        result.reserve(contours.size());

        for (const auto& segments : contours)
        {
            const size_t n = segments.size();

            std::vector<bool> is_corner(n, false);
            size_t corner_count = 0;
            for (size_t i = 0; i < n; ++i)
            {
                const ml7::Vector2f tangent_out = segments[i].tangent_at(1.0f).normalized();
                const ml7::Vector2f tangent_in = segments[(i + 1) % n].tangent_at(0.0f).normalized();
                if (tangent_out.dot(tangent_in) < corner_dot_threshold)
                {
                    is_corner[i] = true;
                    ++corner_count;
                }
            }

            // The neighboring edge's tangent at each of this edge's two corners (see
            // ColoredSegment's fields), needed regardless of whether that corner was
            // sharp enough to end a color run: _query_edge's wedge test degrades
            // gracefully to the single-tangent case when the two tangents are nearly
            // parallel anyway, so there's no reason to special-case smooth joins here.
            std::vector<ColoredSegment> colored(n);
            for (size_t i = 0; i < n; ++i)
            {
                colored[i].bezier = segments[i];
                colored[i].tangent_in_at_start = segments[(i + n - 1) % n].tangent_at(1.0f);
                colored[i].tangent_out_at_end = segments[(i + 1) % n].tangent_at(0.0f);
            }

            if (corner_count == 0)
            {
                // A fully smooth contour has no corners to preserve; one shared color
                // (all channels agree) reconstructs identically to a plain SDF.
                for (size_t i = 0; i < n; ++i)
                    colored[i].channels = COLOR_WHITE;
            }
            else
            {
                // Group segments into runs between consecutive corners, in cyclic
                // order, each starting right after the corner that ends it. Index 0
                // isn't necessarily itself a run boundary (the contour is cyclic, and
                // index 0 is an arbitrary cut point into it), so this is the run list,
                // not just a re-indexing of the segment array.
                std::vector<size_t> run_starts;
                run_starts.reserve(corner_count);
                for (size_t i = 0; i < n; ++i)
                {
                    if (is_corner[i])
                        run_starts.push_back((i + 1) % n);
                }
                const size_t run_count = run_starts.size();

                // A run's segment count, i.e., the distance from its start to the next
                // run's start going forward around the (cyclic) contour. Computed
                // explicitly rather than looping "from start until reaching the next
                // run's start", since with a single run (run_count == 1) that next
                // start coincides with this run's own start - start == end is
                // otherwise ambiguous between "empty range" and "the full circle",
                // and it must mean the latter here (there's nowhere else for those
                // segments to belong).
                const auto run_length = [&](size_t r)
                {
                    return (run_starts[(r + 1) % run_count] + n - run_starts[r] - 1) % n + 1;
                };

                // Each run's bounding box (over its segments' control points, a
                // conservative bound, since a quadratic Bezier's curve always lies
                // within its control points' convex hull), used below to detect runs
                // that pass close to each other in space despite being far apart
                // along the contour (e.g., the outer and inner edge of a thin curved
                // stroke, like the two sides of an "S").
                constexpr float bound_inf = std::numeric_limits<float>::max();
                std::vector<ml7::Vector2f> run_bb_min(run_count, ml7::Vector2f{bound_inf, bound_inf});
                std::vector<ml7::Vector2f> run_bb_max(run_count, ml7::Vector2f{-bound_inf, -bound_inf});
                for (size_t r = 0; r < run_count; ++r)
                {
                    const size_t length = run_length(r);
                    for (size_t k = 0; k < length; ++k)
                    {
                        const size_t i = (run_starts[r] + k) % n;
                        for (const ml7::Vector2f& p : {segments[i].p0, segments[i].p1, segments[i].p2})
                        {
                            run_bb_min[r].x = std::min(run_bb_min[r].x, p.x);
                            run_bb_min[r].y = std::min(run_bb_min[r].y, p.y);
                            run_bb_max[r].x = std::max(run_bb_max[r].x, p.x);
                            run_bb_max[r].y = std::max(run_bb_max[r].y, p.y);
                        }
                    } // for each segment in this run
                } // for each run

                ml7::Vector2f contour_bb_min{bound_inf, bound_inf};
                ml7::Vector2f contour_bb_max{-bound_inf, -bound_inf};
                for (size_t r = 0; r < run_count; ++r)
                {
                    contour_bb_min.x = std::min(contour_bb_min.x, run_bb_min[r].x);
                    contour_bb_min.y = std::min(contour_bb_min.y, run_bb_min[r].y);
                    contour_bb_max.x = std::max(contour_bb_max.x, run_bb_max[r].x);
                    contour_bb_max.y = std::max(contour_bb_max.y, run_bb_max[r].y);
                }
                // Self-scaling regardless of font size: two runs closer than this
                // fraction of the whole contour's bounding box diagonal are treated
                // as potentially ambiguous if they end up sharing a channel.
                const ml7::Vector2f contour_bb_size = contour_bb_max - contour_bb_min;
                const float proximity_threshold = 0.15f * contour_bb_size.length();

                const auto aabb_distance = [](const ml7::Vector2f& a_min, const ml7::Vector2f& a_max, const ml7::Vector2f& b_min, const ml7::Vector2f& b_max)
                {
                    const float dx = std::max({a_min.x - b_max.x, b_min.x - a_max.x, 0.0f});
                    const float dy = std::max({a_min.y - b_max.y, b_min.y - a_max.y, 0.0f});
                    return std::sqrt(dx * dx + dy * dy);
                };

                // Two constraint graphs over the runs: `adjacency` alone (every pair
                // of cyclically consecutive runs, since they meet at a real corner -
                // always satisfiable, a cycle graph is 3-colorable) and `all_constraints`,
                // which additionally includes non-consecutive run pairs that pass close
                // enough in space to risk the same channel-collision ambiguity that
                // _channel_signed_distance can't otherwise tell apart from a genuine
                // nearby edge (e.g., the outer and inner edge of a thin curved stroke).
                std::vector<std::vector<size_t>> adjacency(run_count);
                std::vector<std::vector<size_t>> all_constraints(run_count);
                const auto add_constraint = [](std::vector<std::vector<size_t>>& graph, size_t r1, size_t r2)
                {
                    graph[r1].push_back(r2);
                    graph[r2].push_back(r1);
                };
                // A single run (one corner, e.g., an otherwise-round hole with one
                // minor kink) has no other run to be adjacent to; without this guard,
                // `(r + 1) % run_count` would wrap back to itself and add a "must
                // differ from itself" constraint, which is unsatisfiable, leaving
                // every segment of this contour with no color/channel at all.
                if (run_count >= 2)
                {
                    for (size_t r = 0; r < run_count; ++r)
                    {
                        add_constraint(adjacency, r, (r + 1) % run_count);
                        add_constraint(all_constraints, r, (r + 1) % run_count);
                    }
                }
                for (size_t r1 = 0; r1 < run_count; ++r1)
                {
                    for (size_t r2 = r1 + 2; r2 < run_count; ++r2)
                    {
                        if (r1 == 0 && r2 == run_count - 1)
                            continue; // Already a mandatory (adjacent, wrap-around) constraint above.
                        const float d = aabb_distance(run_bb_min[r1], run_bb_max[r1], run_bb_min[r2], run_bb_max[r2]);
                        if (d < proximity_threshold)
                            add_constraint(all_constraints, r1, r2);
                    }
                }

                // Solve via simple backtracking (run_count is always small, a handful
                // of corners per glyph, so this is cheap regardless of approach).
                // Falls back to the adjacency-only graph (always solvable) if the
                // proximity constraints turn out to be jointly unsatisfiable, which is
                // rare and, when it happens, still strictly better than not having
                // attempted them at all.
                std::vector<int> run_color_signed(run_count, -1);
                const auto try_assign = [&](const auto& self, const std::vector<std::vector<size_t>>& graph, size_t r) -> bool
                {
                    if (r == run_count)
                        return true;
                    for (uint8_t c = 0; c < 3; ++c)
                    {
                        bool conflict = false;
                        for (size_t other : graph[r])
                        {
                            if (other < r && run_color_signed[other] == c)
                            {
                                conflict = true;
                                break;
                            }
                        }
                        if (conflict)
                            continue;
                        run_color_signed[r] = c;
                        if (self(self, graph, r + 1))
                            return true;
                        run_color_signed[r] = -1;
                    }
                    return false;
                };
                if (!try_assign(try_assign, all_constraints, 0))
                {
                    std::ranges::fill(run_color_signed, -1);
                    try_assign(try_assign, adjacency, 0);
                }

                std::vector<uint8_t> run_color(run_count);
                for (size_t r = 0; r < run_count; ++r)
                    run_color[r] = static_cast<uint8_t>(run_color_signed[r]);

                for (size_t r = 0; r < run_count; ++r)
                {
                    const size_t length = run_length(r);
                    for (size_t k = 0; k < length; ++k)
                        colored[(run_starts[r] + k) % n].channels = colors[run_color[r]];
                } // for each run
            }

            result.push_back(std::move(colored));
        } // for each contour

        return result;
    }

    MsdfRasterizer::EdgeQuery MsdfRasterizer::_query_edge(const ColoredSegment& segment, ml7::Vector2f point)
    {
        constexpr float t_epsilon = 1e-4f;
        const ml7::QuadraticBezier2f& bezier = segment.bezier;

        // The true (clamped) nearest point on this segment; always geometrically
        // bounded, unlike projecting onto an unbounded extension of the tangent line,
        // which can produce a misleadingly small distance for points far from this
        // segment entirely (if they happen to lie near where that infinite line
        // passes, regardless of proximity to the segment itself).
        const float t = bezier.parameter(point);
        const ml7::Vector2f closest = bezier.point_at(t);
        const ml7::Vector2f to_point = point - closest;
        const bool is_clamped_start = t <= t_epsilon;
        const bool is_clamped_end = t >= 1.0f - t_epsilon;

        float signed_orthogonality;
        if (!is_clamped_start && !is_clamped_end)
        {
            // A true interior match: this edge's own local tangent unambiguously
            // determines which side the point is on.
            signed_orthogonality = bezier.tangent_at(t).cross(to_point);
        }
        else
        {
            // Clamped to a corner: this edge's own tangent alone can't tell inside
            // from outside for a point off to the side of the corner rather than in
            // front of the edge, that depends on the angle the two edges meeting
            // there actually form. Resolve it against BOTH tangents at that corner
            // (this edge's own, and the neighboring edge's, stored on `segment`):
            // - Convex corner (tangents turn the same way the point's offset does):
            //   only genuinely inside if BOTH tangents agree it's inside (an AND),
            //   since the point could sit in the notch just outside the corner's tip
            //   where either tangent alone would wrongly call it inside.
            // - Concave/reflex corner: inside if EITHER tangent says so (an OR),
            //   since the point could sit just inside the notch where either tangent
            //   alone would wrongly call it outside.
            const ml7::Vector2f tangent_here = is_clamped_start ? bezier.tangent_at(0.0f) : bezier.tangent_at(1.0f);
            const ml7::Vector2f tangent_other = is_clamped_start ? segment.tangent_in_at_start : segment.tangent_out_at_end;
            // Convention matches this edge's own tangent order (incoming -> outgoing).
            const ml7::Vector2f tangent_in = is_clamped_start ? tangent_other : tangent_here;
            const ml7::Vector2f tangent_out = is_clamped_start ? tangent_here : tangent_other;

            const float orth_in = tangent_in.cross(to_point);
            const float orth_out = tangent_out.cross(to_point);
            const bool is_convex = tangent_in.cross(tangent_out) >= 0.0f;
            signed_orthogonality = is_convex ? std::min(orth_in, orth_out) : std::max(orth_in, orth_out);
        }

        return {
            .unsigned_distance = to_point.length(),
            .signed_orthogonality = signed_orthogonality,
            .is_true_distance = !is_clamped_start && !is_clamped_end,
        };
    }

    float MsdfRasterizer::_channel_signed_distance(const std::vector<std::vector<ColoredSegment>>& contours, uint8_t channel, ml7::Vector2f point)
    {
        // Tracked separately from the "true" (non-clamped) distance below: a corner's
        // pseudo-distance can easily be numerically the smallest (a point can sit
        // right next to a short edge's endpoint) while its sign is meaningless there,
        // since it comes from that one edge's tangent rather than the actual boundary
        // orientation at the corner. Preferring any true match over a smaller-but-only
        // pseudo match avoids picking up that unreliable sign, which otherwise shows
        // up as stray, isolated mis-colored pixels near corners (particularly short
        // "cut" edges, e.g., a letter's flat stroke terminals).
        float best_true_distance = std::numeric_limits<float>::max();
        float best_true_orthogonality = 0.0f;
        bool found_true = false;

        float best_pseudo_distance = std::numeric_limits<float>::max();
        float best_pseudo_orthogonality = 0.0f;
        bool found_pseudo = false;

        for (const auto& contour : contours)
        {
            for (const auto& segment : contour)
            {
                if ((segment.channels & channel) == 0)
                    continue;

                const EdgeQuery query = _query_edge(segment, point);
                if (query.is_true_distance)
                {
                    if (query.unsigned_distance < best_true_distance)
                    {
                        best_true_distance = query.unsigned_distance;
                        best_true_orthogonality = query.signed_orthogonality;
                        found_true = true;
                    }
                }
                else if (query.unsigned_distance < best_pseudo_distance)
                {
                    best_pseudo_distance = query.unsigned_distance;
                    best_pseudo_orthogonality = query.signed_orthogonality;
                    found_pseudo = true;
                }
            } // for each colored segment
        } // for each contour

        // No segment carries this channel at all (e.g., a glyph with no contours,
        // like space): there is no boundary to be near, so the point is
        // unambiguously outside. Without this, the loop-local defaults above
        // (0.0f is >= 0.0f) fall into the "inside" branch below, encoding as
        // fully opaque/white everywhere, i.e., a solid glyph out of empty geometry.
        if (!found_true && !found_pseudo)
            return -std::numeric_limits<float>::max();

        // True wins whenever it's at least as close as the best pseudo match -
        // including (near-)ties, e.g., two edges meeting at a corner both reporting
        // that same corner point as their nearest point. Two independent curve
        // evaluations of the same shared corner point won't generally land bit-
        // identical, so a small tolerance (well above float noise at these
        // pixel-scale magnitudes, but far below any meaningfully different
        // distance) is needed to actually catch that case; a pseudo match that's
        // genuinely closer by more than that (the common case almost everywhere
        // away from a corner, including far outside the shape entirely, in the
        // padding margin) still correctly wins.
        constexpr float distance_tie_epsilon = 1e-3f;
        const bool use_true = found_true && (!found_pseudo || best_true_distance <= best_pseudo_distance + distance_tie_epsilon);
        const float best_unsigned_distance = use_true ? best_true_distance : best_pseudo_distance;
        const float best_signed_orthogonality = use_true ? best_true_orthogonality : best_pseudo_orthogonality;

        return best_signed_orthogonality >= 0.0f ? best_unsigned_distance : -best_unsigned_distance;
    }

    void MsdfRasterizer::_rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, dl7::Buffer2dSpan canvas)
    {
        const auto contours = detail::transform_glyph_contours(glyph, size_config.font_size);
        const auto colored_contours = _color_contours(contours);

        for (size_t row = 0; row < canvas.height(); ++row)
        {
            for (size_t col = 0; col < canvas.width(); ++col)
            {
                const ml7::Vector2f point{
                    0.5f + static_cast<float>(col) + static_cast<float>(pixel_offset.left),
                    0.5f + static_cast<float>(row) + static_cast<float>(pixel_offset.top),
                };

                const float r = _channel_signed_distance(colored_contours, CHANNEL_R, point);
                const float g = _channel_signed_distance(colored_contours, CHANNEL_G, point);
                const float b = _channel_signed_distance(colored_contours, CHANNEL_B, point);

                canvas.map_element<RGBA8>(row, col) = {
                    .r = _encode_channel(r, _pixel_range),
                    .g = _encode_channel(g, _pixel_range),
                    .b = _encode_channel(b, _pixel_range),
                    .a = 255,
                };
            } // for each pixel column
        } // for each pixel row
    }



    size_t MsdfRasterizer::_cache_key_params_hash() const
    {
        // Bump this if `_rasterize_glyph_into` (or edge-coloring) above
        // changes algorithmically (i.e., inputs unchanged, but the output
        // would now differ), to force-invalidate previously cached results.
        constexpr uint32_t ALGORITHM_VERSION = 1;

        size_t hash = 0;
        cl7::bits::hash_combine(hash, static_cast<size_t>(ALGORITHM_VERSION));
        cl7::bits::hash_combine(hash, static_cast<size_t>(std::bit_cast<uint32_t>(_pixel_range)));
        return hash;
    }



} // namespace fl7::fonts::raster
