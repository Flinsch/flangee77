
target_sources(flangee77
    PRIVATE

    fonts/Contour.cpp
    fonts/Font.cpp
    fonts/FontLoader.cpp
    fonts/Glyph.cpp
    fonts/TextMetrics.cpp
    fonts/TrueTypeFontLoader.cpp

    fonts/detail/RawGlyph.cpp

    fonts/render/AbstractRenderer.cpp
    fonts/render/TestRenderer.cpp
)
