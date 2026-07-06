
#include "atlas-renderer-common.hlsli"

PixelOut mainPixel(PixelIn i)
{
    PixelOut o;
    float alpha = SAMPLE_TEX2D(GlyphAtlas, GlyphSampler, i.uv).a;
    o.color = float4(i.color.rgb, i.color.a * alpha);
    return o;
}
