
#include "atlas-renderer-common.hlsli"

PixelOut mainPixel(PixelIn i)
{
    PixelOut o;
    float alpha = SAMPLE_TEX2D(GlyphAtlas, GlyphSampler, i.uv).a;
    // The render target is not sRGB, so fixed-function blending happens directly on
    // gamma-encoded values instead of linear light. This under-weights partially
    // covered edge pixels, making text look thinner than it should. Boosting the
    // coverage before blending approximates the effect of correct linear blending.
    alpha = pow(alpha, 1.0 / 2.2);
    o.color = float4(i.color.rgb, i.color.a * alpha);
    return o;
}
