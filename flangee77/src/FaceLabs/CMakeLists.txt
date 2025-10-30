
target_sources(flangee77
    PRIVATE

    fonts/Contour.cpp
    fonts/Font.cpp
    fonts/FontLoader.cpp
    fonts/Glyph.cpp
    fonts/TextMetrics.cpp

    fonts/detail/ttf/RawGlyph.cpp
    fonts/detail/ttf/TrueTypeFontLoader.cpp

    fonts/render/AbstractRenderer.cpp
    fonts/render/TestRenderer.cpp
)
