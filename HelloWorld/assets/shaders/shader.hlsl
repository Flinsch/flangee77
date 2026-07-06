
#include "include/compat.hlsli"

cbuffer MyVertexConstants
{
    float3 VertexOffset;
};

cbuffer MyPixelConstants
{
    float4 PixelBaseColor;
};

DECLARE_TEXTURE2D(TextureMap, TextureSampler, 0, 0);

struct VertexIn
{
    float3 pos : POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
};

struct VertexOut
{
    float4 pos : VS_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
};

struct PixelIn
{
    float4 pos : VS_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
};

struct PixelOut
{
    float4 color : PS_TARGET0;
};

VertexOut mainVertex(VertexIn i)
{
    VertexOut o;

    o.pos = float4(i.pos + VertexOffset, 1);
    o.color = i.color;
    o.tex = i.tex;

    return o;
}

PixelOut mainPixel(PixelIn i)
{
    PixelOut o;

    o.color = i.color * PixelBaseColor * SAMPLE_TEX2D(TextureMap, TextureSampler, i.tex);

    return o;
}
