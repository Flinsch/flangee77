
#include "atlas-renderer-common.hlsli"

PixelOut mainPixel(PixelIn i)
{
    PixelOut o;
    // Wireframe debug overlay: texcoord is unused, just output the vertex color directly.
    o.color = i.color;
    return o;
}