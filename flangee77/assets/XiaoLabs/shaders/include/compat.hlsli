
#ifndef COMPAT_HLSLI
#define COMPAT_HLSLI

// D3D9_COMPAT is defined by D3DShaderCompiler for shader model < 4 (SM3 / D3D9).

// Any PixelIn struct must declare VS_POSITION as its first member (even if
// unused in the pixel shader body). D3D11 links VS outputs to PS inputs by
// register number, not by name, and SV_POSITION still occupies a register
// slot in the VS output signature. Omitting it from PixelIn shifts every
// other PS input by one register relative to what the VS actually wrote.

#ifdef D3D9_COMPAT

#define VS_POSITION POSITION
#define PS_TARGET   COLOR
#define PS_TARGET0  COLOR0
#define PS_TARGET1  COLOR1
#define PS_TARGET2  COLOR2
#define PS_TARGET3  COLOR3

#define DECLARE_TEXTURE2D(tex_name, samp_name, t_reg, s_reg) \
    sampler2D samp_name

#define SAMPLE_TEX2D(tex_name, samp_name, uv) \
    tex2D(samp_name, uv)

#else // D3D9_COMPAT

#define VS_POSITION SV_POSITION
#define PS_TARGET   SV_TARGET
#define PS_TARGET0  SV_TARGET0
#define PS_TARGET1  SV_TARGET1
#define PS_TARGET2  SV_TARGET2
#define PS_TARGET3  SV_TARGET3

#define DECLARE_TEXTURE2D(tex_name, samp_name, t_reg, s_reg) \
    Texture2D tex_name : register(t ## t_reg); \
    SamplerState samp_name : register(s ## s_reg)

#define SAMPLE_TEX2D(tex_name, samp_name, uv) \
    tex_name.Sample(samp_name, uv)

#endif // D3D9_COMPAT

#endif // COMPAT_HLSLI
