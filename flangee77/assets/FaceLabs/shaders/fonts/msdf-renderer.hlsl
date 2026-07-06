
// TODO: MSDF rasterizer not yet implemented; this is a stub.

#include "atlas-renderer-common.hlsli"

float median(float r, float g, float b)
{
    return max(min(r, g), min(max(r, g), b));
}

PixelOut mainPixel(PixelIn i)
{
    PixelOut o;
    // MSDF: multi-channel signed distance in RGB channels
    float3 sample = SAMPLE_TEX2D(GlyphAtlas, GlyphSampler, i.uv).rgb;
    float dist = median(sample.r, sample.g, sample.b);
    float alpha = smoothstep(0.4, 0.6, dist);
    o.color = float4(i.color.rgb, i.color.a * alpha);
    return o;
}
