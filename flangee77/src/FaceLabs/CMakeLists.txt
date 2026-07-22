
target_sources(flangee77
    PRIVATE

    fonts/Contour.cpp
    fonts/Font.cpp
    fonts/FontLoader.cpp
    fonts/Glyph.cpp
    fonts/TextLayout.cpp
    fonts/TextMetrics.cpp

    fonts/detail/ttf/RawGlyph.cpp
    fonts/detail/ttf/TrueTypeFontLoader.cpp

    fonts/raster/AbstractBitmapRasterizer.cpp
    fonts/raster/AbstractRasterizer.cpp
    fonts/raster/AnalyticalCoverageRasterizer.cpp
    fonts/raster/GlyphRasterCache.cpp
    fonts/raster/MsdfRasterizer.cpp
    fonts/raster/OversampledBitmapRasterizer.cpp
    fonts/raster/SdfRasterizer.cpp
    fonts/raster/SimpleBitmapRasterizer.cpp

    fonts/raster/detail/glyph_geometry.cpp

    fonts/render/AbstractRenderer.cpp
    fonts/render/AbstractTextureAtlasBasedRenderer.cpp
    fonts/render/BitmapRenderer.cpp
    fonts/render/Markup.cpp
    fonts/render/MsdfRenderer.cpp
    fonts/render/SdfRenderer.cpp
    fonts/render/TestRenderer.cpp
)
