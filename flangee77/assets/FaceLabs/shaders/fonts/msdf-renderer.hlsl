
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
    // instead of a fixed threshold, and for the pseudo-bold edge bias and the
    // outline dual-threshold blend below.
    float width = max(fwidth(dist), 1e-5);
    float edge = 0.5 - i.weight * 0.10;
    float outline_edge = edge - i.outline_width * width;

    float fill_coverage = pow(smoothstep(edge - width, edge + width, dist), 1.0 / 2.2);
    float outline_coverage = pow(smoothstep(outline_edge - width, outline_edge + width, dist), 1.0 / 2.2);

    float4 blended = lerp(i.outline_color, i.color, fill_coverage);
    o.color = float4(blended.rgb, blended.a * outline_coverage);

    return o;
}
