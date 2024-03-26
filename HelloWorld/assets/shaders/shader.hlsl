
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
    o.color = i.color;
    o.tex = i.tex;

    return o;
}

PixelOut mainPixel(PixelIn i)
{
    PixelOut o;

    o.color = i.color;
    o.color = tex2D(TextureSampler, i.tex);

    return o;
}
