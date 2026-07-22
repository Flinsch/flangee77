
#include "atlas-renderer-common.hlsli"

PixelOut mainPixel(PixelIn i)
{
    PixelOut o;

    // SDF: signed distance in the red channel; 0.5 is the glyph edge.
    float dist = SAMPLE_TEX2D(GlyphAtlas, GlyphSampler, i.uv).r;
    // Derive the antialiasing band from how fast the distance changes across
    // screen pixels (via fwidth), so edges stay equally crisp regardless of the
    // glyph's on-screen scale, instead of a fixed-width threshold that would
    // only be correct at one specific size.
    float width = max(fwidth(dist), 1e-5);
    // Pseudo-bold: bias the edge threshold below 0.5 so more of the glyph's
    // interior counts as covered, thickening the stroke without re-rasterizing.
    float edge = 0.5 - i.weight * 0.10;

    // Outline: a second, further-out threshold for the outline's outer boundary.
    // outline_width is in screen pixels; multiplying by `width` (distance-field
    // units per screen pixel, i.e., the same derivative used for antialiasing
    // above) converts it into the distance field's own units, so the outline
    // stays a constant width on screen regardless of font size/scale.
    float outline_edge = edge - i.outline_width * width;

    // See bitmap-renderer.hlsl for why each coverage mask is boosted before blending.
    float fill_coverage = pow(smoothstep(edge - width, edge + width, dist), 1.0 / 2.2);
    float outline_coverage = pow(smoothstep(outline_edge - width, outline_edge + width, dist), 1.0 / 2.2);

    // Blend outline -> fill color across the fill boundary; the outline
    // boundary's coverage is the overall visible shape (fill sits inside it).
    float4 blended = lerp(i.outline_color, i.color, fill_coverage);
    o.color = float4(blended.rgb, blended.a * outline_coverage);

    return o;
}
