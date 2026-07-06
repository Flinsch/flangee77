
cbuffer VertexConstants
{
    float4x4 WorldViewProjection;
}

cbuffer PixelConstants
{
    float4 BaseColor;
}

#ifdef D3D9_COMPAT

struct VertexIn
{
    float3 pos : POSITION;
    float4 color : COLOR0;
};

struct VertexOut
{
    float4 pos : POSITION;
    float4 color : COLOR0;
};

struct PixelIn
{
    float4 color : COLOR0;
};

struct PixelOut
{
    float4 color : COLOR0;
};

#else // D3D9_COMPAT

struct VertexIn
{
    float3 pos : POSITION;
    float4 color : COLOR0;
};

struct VertexOut
{
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
};

struct PixelIn
{
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
};

struct PixelOut
{
    float4 color : SV_TARGET0;
};

#endif // D3D9_COMPAT
