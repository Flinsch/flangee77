
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
    float alpha = smoothstep(0.5 - width, 0.5 + width, dist);
    // See bitmap-renderer.hlsl for why the coverage is boosted before blending.
    alpha = pow(alpha, 1.0 / 2.2);

    o.color = float4(i.color.rgb, i.color.a * alpha);

    return o;
}
