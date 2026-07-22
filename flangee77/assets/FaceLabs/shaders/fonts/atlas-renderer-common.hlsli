
#include "../include/compat.hlsli"

cbuffer Constants
{
    float2 InvTargetSize;
};

DECLARE_TEXTURE2D(GlyphAtlas, GlyphSampler, 0, 0);

struct VertexIn
{
    float2 pos           : POSITION;
    float2 uv            : TEXCOORD0;
    float4 color         : COLOR0;
    float  weight        : TEXCOORD1;
    float4 outline_color : COLOR1;
    float  outline_width : TEXCOORD2;
};

struct VertexOut
{
    float4 pos           : VS_POSITION;
    float2 uv            : TEXCOORD0;
    float4 color         : COLOR0;
    float  weight        : TEXCOORD1;
    float4 outline_color : COLOR1;
    float  outline_width : TEXCOORD2;
};

struct PixelIn
{
    float4 pos           : VS_POSITION;
    float2 uv            : TEXCOORD0;
    float4 color         : COLOR0;
    float  weight        : TEXCOORD1;
    float4 outline_color : COLOR1;
    float  outline_width : TEXCOORD2;
};

struct PixelOut
{
    float4 color : PS_TARGET0;
};

VertexOut mainVertex(VertexIn i)
{
    VertexOut o;

    float2 ndc;
    ndc.x = i.pos.x * InvTargetSize.x * 2.0 - 1.0;
    ndc.y = 1.0 - i.pos.y * InvTargetSize.y * 2.0;

    o.pos = float4(ndc, 0.5, 1.0);
    o.uv = i.uv;
    o.color = i.color;
    o.weight = i.weight;
    o.outline_color = i.outline_color;
    o.outline_width = i.outline_width;

    return o;
}
