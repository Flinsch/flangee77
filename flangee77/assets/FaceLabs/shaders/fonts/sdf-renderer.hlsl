
// TODO: SDF rasterizer not yet implemented; this is a stub.

#include "atlas-renderer-common.hlsli"

PixelOut mainPixel(PixelIn i)
{
    PixelOut o;
    // SDF: signed distance in the red channel; threshold at 0.5
    float dist = SAMPLE_TEX2D(GlyphAtlas, GlyphSampler, i.uv).r;
    float alpha = smoothstep(0.4, 0.6, dist);
    o.color = float4(i.color.rgb, i.color.a * alpha);
    return o;
}
