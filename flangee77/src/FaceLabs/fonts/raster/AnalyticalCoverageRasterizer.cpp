#include "AnalyticalCoverageRasterizer.h"

#include <MathLabs/LineSegment2.h>

#include <CoreLabs/utilities.h>

#include <stack>



namespace fl7::fonts::raster {



    static std::pair<ml7::LineSegment2f, float> _normalize_line_segment(const ml7::LineSegment2f& line_segment)
    {
        if (line_segment.start.y < line_segment.end.y)
            return {line_segment, +1.0f}; // Original.

        return {{line_segment.end, line_segment.start}, -1.0f}; // Flipped, inverted.
    }

    static void _process_scaled_and_flattened_segment(const ml7::LineSegment2f& line_segment, PixelOffset pixel_offset, dl7::Buffer2d<float> coverage_canvas)
    {
        // Skip horizontal edges.
        if (line_segment.start.y == line_segment.end.y)
            return;

        // The (downward) line segment with a lower y-coordinate at the starting
        // (top) point and a higher y-coordinate at the ending (bottom) point.
        // And a winding of +1 if downward in original orientation, else -1.
        const auto [down_segment, winding] = _normalize_line_segment(line_segment);

        const auto row_count = static_cast<int>(coverage_canvas.height());
        const auto column_count = static_cast<int>(coverage_canvas.width());

        assert(down_segment.start.y < down_segment.end.y);
        const bool is_ascending_x = down_segment.start.x < down_segment.end.x;

        const float dx_dy = (down_segment.end.x - down_segment.start.x) / (down_segment.end.y - down_segment.start.y);
        const float dx_dy_norm = is_ascending_x ? dx_dy : -dx_dy;

        // Determine the pixel rows that are
        // intersected by the current
        const auto row0 = static_cast<int>(std::floor(down_segment.start.y)) - pixel_offset.top;
        const auto row1 = static_cast<int>(std::ceil(down_segment.end.y)) - pixel_offset.top;
        assert(row0 < row1);

        ml7::Vector2f row_p0 = down_segment.start;
        ml7::Vector2f row_p1;

        for (int row = row0; row < row1; ++row, row_p0 = row_p1)
        {
            row_p1 = down_segment.end;
            if (row + 1 < row1)
            {
                // Not the last row? Then find the intersection point with the pixel row's bottom.
                row_p1.y = static_cast<float>(row + 1 + pixel_offset.top);
                row_p1.x = row_p0.x + (row_p1.y - row_p0.y) * dx_dy;
            }

            if (row < 0 || row >= row_count)
                continue;

            const auto [row_xlo, row_xhi] = cl7::flip_unless(is_ascending_x, row_p0.x, row_p1.x);

            // Determine the pixel columns that are intersected
            // by the current edge within the current row.
            const auto col0 = static_cast<int>(std::floor(row_xlo)) - pixel_offset.left;
            const auto col1 = static_cast<int>(std::ceil(row_xhi)) - pixel_offset.left;
            assert(col0 < col1);

            // The intersection height and rectangular
            // coverage within the current row.
            const float row_h = row_p1.y - row_p0.y;
            const float row_coverage = row_h * winding;

            // Fill/unfill all non-intersected pixels that are completely to the left of the current edge segment.
            for (int col = 0; col < col0; ++col)
                coverage_canvas.element(row, col) += row_coverage;

            // At this point, we completely disregard the edge orientation, as it's not relevant to us here.
            // For simplicity, we now model our points so that they are not only sorted vertically in ascending
            // order, but also horizontally, even if this is technically not correct. It doesn't matter,
            // because the trapezoid's orientation is irrelevant for calculating the intersection area.
            ml7::Vector2f fake_p0{row_xlo, row_p0.y};
            ml7::Vector2f fake_p1{row_xhi, row_p1.y};

            ml7::Vector2f col_p0 = fake_p0;
            ml7::Vector2f col_p1;

            for (int col = col0; col < col1; ++col, col_p0 = col_p1)
            {
                col_p1 = fake_p1;
                if (col + 1 < col1)
                {
                    // Not the last column? Then find the intersection point with the pixel column's right side.
                    col_p1.x = static_cast<float>(col + 1 + pixel_offset.left);
                    col_p1.y = col_p0.y + (col_p1.x - col_p0.x) / dx_dy_norm; // The denominator cannot be 0, otherwise the edge would not extend horizontally across multiple columns/pixels.
                }

                if (col < 0 || col >= column_count)
                    continue;

                // The intersection height and rectangular
                // coverage within the current column.
                const float col_h = col_p1.y - col_p0.y;
                const float col_coverage = col_h * winding;

                // Fill/unfill all (intersected) pixels that are to the left of the current edge segment
                // that have not yet been considered during the row-related processing above.
                for (int c = col0; c < col; ++c)
                    coverage_canvas.element(row, c) += col_coverage;

                const auto [col_xlo, col_xhi] = cl7::flip_unless(is_ascending_x, col_p0.x, col_p1.x);

                // Calculate the intersection area of the trapezoid within the current pixel.
                const float pix_w0 = col_xlo - std::floor(col_xlo); // Width from pixel's left side to edge's lower x-coordinate
                const float pix_w01 = col_xhi - col_xlo; // Horizontal projection "width" of the edge
                // The area to the left of the edge is fully considered (rectangle),
                // the area "along" the edge is only half considered (triangle).
                // Together, they form a trapezoid within the current pixel.
                const float pix_coverage = (pix_w0 + pix_w01 * 0.5f) * col_coverage;

                // Fill/unfill current pixel.
                coverage_canvas.element(row, col) += pix_coverage;
            } // for each covered pixel column
        } // for each covered pixel row
    }

    static void _process_scaled_bezier(ml7::QuadraticBezier2f bezier, const PixelOffset& pixel_offset, const dl7::Buffer2d<float>& coverage_canvas)
    {
        // Flattening threshold in pixels (max distance from control
        // point to chord): 0.35 pixels is reasonable. (Or is it?)
        constexpr float flatness_threshold = 0.35f;
        constexpr float flatness_threshold_squared = flatness_threshold * flatness_threshold;

        std::stack<ml7::QuadraticBezier2f> stack;
        stack.push(bezier);

        while (!stack.empty())
        {
            bezier = stack.top();
            stack.pop();

            const ml7::LineSegment2f line_segment{bezier.p0, bezier.p2};
            const float distance_squared = line_segment.distance_squared(bezier.p1);

            if (distance_squared <= flatness_threshold_squared)
            {
                _process_scaled_and_flattened_segment(line_segment, pixel_offset, coverage_canvas);
                continue;
            }

            const auto pair = bezier.subdivide(0.5f);
            stack.push(pair.first);
            stack.push(pair.second);
        } // while segment not flat
    }

    static void _process_glyph(const Glyph& glyph, float font_size, const PixelOffset& pixel_offset, const dl7::Buffer2d<float>& coverage_canvas)
    {
        const ml7::Vector2f transform{font_size, -font_size};

        for (const auto& contour : glyph.contours)
        {
            for (size_t i = 0; i < contour.segment_count(); ++i)
            {
                ml7::QuadraticBezier2f bezier = contour.segment_at(i);
                bezier.p0 *= transform;
                bezier.p1 *= transform;
                bezier.p2 *= transform;

                _process_scaled_bezier(bezier, pixel_offset, coverage_canvas);
            } // for each segment
        } // for each contour
    }



    void AnalyticalCoverageRasterizer::_rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, dl7::Buffer2dSpan canvas)
    {
        std::vector<float> coverage_data(canvas.size());
        dl7::Buffer2d<float> coverage_canvas(coverage_data, canvas.width(), canvas.height());

        _process_glyph(glyph, size_config.font_size, pixel_offset, coverage_canvas);

        for (size_t row = 0; row < canvas.height(); ++row)
        {
            for (size_t col = 0; col < canvas.width(); ++col)
            {
                const float coverage = coverage_canvas.element(row, col);

                canvas.map_element<uint8_t>(row, col) = static_cast<uint8_t>(ml7::clamp01(coverage) * 255.0f);
            } // for each pixel column
        } // for each pixel row
    }



} // namespace fl7::fonts::raster
