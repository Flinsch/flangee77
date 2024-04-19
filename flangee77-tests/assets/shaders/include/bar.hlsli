
cbuffer VertexConstants
{
    float4x4 WorldViewProjection;
}

cbuffer PixelConstants
{
    float4 BaseColor;
}

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
