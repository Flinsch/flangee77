
#include "../include/compat.hlsli"

cbuffer Constants
{
    float2 InvTargetSize;
};

#if defined(TEXTURED)
DECLARE_TEXTURE2D(QuadTexture, QuadSampler, 0, 0);
#endif

struct VertexIn
{
    float2 pos   : POSITION;
    float2 uv    : TEXCOORD0;
    float4 color : COLOR0;
};

struct VertexOut
{
    float4 pos   : VS_POSITION;
    float2 uv    : TEXCOORD0;
    float4 color : COLOR0;
};

struct PixelIn
{
    float4 pos   : VS_POSITION;
    float2 uv    : TEXCOORD0;
    float4 color : COLOR0;
};

struct PixelOut
{
    float4 color : PS_TARGET;
};

// Screen pixel space (y-down, origin top-left) to NDC, no projection matrix.
VertexOut mainVertex(VertexIn i)
{
    VertexOut o;

    float2 ndc;
    ndc.x = i.pos.x * InvTargetSize.x * 2.0 - 1.0;
    ndc.y = 1.0 - i.pos.y * InvTargetSize.y * 2.0;

    o.pos = float4(ndc, 0.5, 1.0);
    o.uv = i.uv;
    o.color = i.color;

    return o;
}

PixelOut mainPixel(PixelIn i)
{
    PixelOut o;

#if defined(TEXTURED)
    o.color = SAMPLE_TEX2D(QuadTexture, QuadSampler, i.uv) * i.color;
#else
    o.color = i.color;
#endif

    return o;
}
