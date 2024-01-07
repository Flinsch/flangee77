#include "include/foo.hlsli"

VertexOut mainVS(VertexIn i)
{
    VertexOut o;

    o.pos = mul(float4(i.pos, 1), WorldViewProjection);
    o.color = i.color;

    return o;
}

PixelOut mainPS(PixelIn i)
{
    PixelOut o;

    o.color = i.color * BaseColor;

    return o;
}
