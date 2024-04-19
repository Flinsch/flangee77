
cbuffer MyVertexConstants
{
    float4 VertexBaseColor;
};

cbuffer MyPixelConstants
{
    float4 PixelBaseColor;
};

sampler2D TextureSampler;

struct VertexIn
{
    float3 pos : POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
};

struct VertexOut
{
    float4 pos : POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
};

#define PixelIn VertexOut

struct PixelOut
{
    float4 color : COLOR0;
};

VertexOut mainVertex(VertexIn i)
{
    VertexOut o;

    o.pos = float4(i.pos, 1);
    o.color = (VertexBaseColor + i.color) * 0.5f;
    o.tex = i.tex;

    return o;
}

PixelOut mainPixel(PixelIn i)
{
    PixelOut o;

    o.color = i.color * (PixelBaseColor + tex2D(TextureSampler, i.tex)) * 0.5f;

    return o;
}
