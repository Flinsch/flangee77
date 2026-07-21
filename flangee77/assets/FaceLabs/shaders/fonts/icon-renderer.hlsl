
#include "atlas-renderer-common.hlsli"

PixelOut mainPixel(PixelIn i)
{
    PixelOut o;

    // Unlike the glyph/bitmap/sdf/msdf shaders, icons sample their texture
    // directly and keep its own colors: the texture isn't coverage/distance
    // data, it's the actual image. Only alpha is modulated by the vertex
    // color (see AbstractTextureAtlasBasedRenderer::_emit_icon).
    float4 icon = SAMPLE_TEX2D(GlyphAtlas, GlyphSampler, i.uv);
    o.color = float4(icon.rgb, icon.a * i.color.a);

    return o;
}
