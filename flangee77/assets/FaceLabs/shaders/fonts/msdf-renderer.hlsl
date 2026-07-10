
#include "atlas-renderer-common.hlsli"

float median(float r, float g, float b)
{
    return max(min(r, g), min(max(r, g), b));
}

PixelOut mainPixel(PixelIn i)
{
    PixelOut o;
    // MSDF: multi-channel signed distance in RGB channels; the median of the
    // three reconstructs sharp corners that a single channel alone would round off.
    float3 sample = SAMPLE_TEX2D(GlyphAtlas, GlyphSampler, i.uv).rgb;
    float dist = median(sample.r, sample.g, sample.b);
    // See sdf-renderer.hlsl for why the antialiasing band is derived via fwidth
    // instead of a fixed threshold.
    float width = max(fwidth(dist), 1e-5);
    float alpha = smoothstep(0.5 - width, 0.5 + width, dist);
    o.color = float4(i.color.rgb, i.color.a * alpha);
    return o;
}
