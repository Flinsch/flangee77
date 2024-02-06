
struct VertexIn
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VertexOut
{
    float4 pos : POSITION;
    float4 color : COLOR;
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

    return o;
}

PixelOut mainPixel(PixelIn i)
{
    PixelOut o;

    o.color = i.color;

    return o;
}
