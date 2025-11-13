
target_sources(flangee77
    PRIVATE

    fonts/Contour.cpp
    fonts/Font.cpp
    fonts/FontLoader.cpp
    fonts/Glyph.cpp
    fonts/TextMetrics.cpp

    fonts/detail/ttf/RawGlyph.cpp
    fonts/detail/ttf/TrueTypeFontLoader.cpp

    fonts/raster/AbstractRasterizer.cpp
    fonts/raster/AnalyticalCoverageRasterizer.cpp
    fonts/raster/OversampledBitmapRasterizer.cpp
    fonts/raster/SimpleBitmapRasterizer.cpp

    fonts/render/AbstractRenderer.cpp
    fonts/render/TestRenderer.cpp
)
