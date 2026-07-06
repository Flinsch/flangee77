
#include "../include/compat.hlsli"

cbuffer VertexConstants
{
    float2 ScaledFontSize;
    float2 PixelPosition;
    float2 InvTargetSize;
};

cbuffer PixelConstants
{
    float4 Color;
};

struct VertexIn
{
    float2 pos : POSITION;
};

struct VertexOut
{
    float4 pos : VS_POSITION;
};

struct PixelOut
{
    float4 color : PS_TARGET0;
};

VertexOut mainVertex(VertexIn i)
{
    VertexOut o;

    i.pos *= ScaledFontSize * float2(+1, -1);
    i.pos += PixelPosition;
    i.pos *= InvTargetSize;
    i.pos *= 2;
    i.pos -= float2(1, 1);
    i.pos *= float2(+1, -1);

    o.pos = float4(i.pos, 0.5, 1);

    return o;
}

PixelOut mainPixel()
{
    PixelOut o;

    o.color = Color;

    return o;
}
