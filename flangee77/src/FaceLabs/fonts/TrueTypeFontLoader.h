#ifndef FL7_FONTS_TRUETYPEFONTLOADER_H
#define FL7_FONTS_TRUETYPEFONTLOADER_H
#include "./FontLoader.h"

#include "./detail/RawGlyph.h"

#include <CoreLabs/io/ReadableMemory.h>

#include <optional>
#include <span>
#include <unordered_map>



namespace fl7::fonts {



/**
 * Provides methods to load font data from a TrueType font file (TTF).
 */
class TrueTypeFontLoader
    : public FontLoader
{

public:
    /**
     * Prepares a TrueType font loader for reading from the specified font file.
     */
    explicit TrueTypeFontLoader(cl7::u8string_view file_path) : FontLoader(file_path) {}

    ~TrueTypeFontLoader() override = default;



    /**
     * Checks whether the glyph corresponding to the specified Unicode code point
     * exists and can therefore potentially be loaded.
     */
    bool has_glyph(cl7::text::codec::codepoint codepoint) override;

    /**
     * Attempts to load the glyph corresponding to the specified Unicode code point.
     * If the glyph could not be loaded, an "empty" glyph is returned (in the sense
     * that it contains no contour points, has no advance width, etc.).
     */
    Glyph load_glyph(cl7::text::codec::codepoint codepoint) override;

    /**
     * Loads the general typographic properties of the font.
     */
    FontMetrics load_metrics() override;



private:
    struct OffsetSubtable
    {
        /** SFNT version: a tag to indicate the OFA scaler to be used to rasterize this font; TrueType outlines should use a value of 0x00010000. */
        uint32_t scaler_type = 0;
        /** The number of tables. */
        uint16_t num_tables = 0;
        /** (maximum power of 2 <= numTables)*16 */
        uint16_t search_range = 0;
        /** log2(maximum power of 2 <= numTables) */
        uint16_t entry_selector = 0;
        /** numTables*16-searchRange */
        uint16_t range_shift = 0;
    };
    static_assert(sizeof(OffsetSubtable) == 12);

    struct TableDirectoryEntry
    {
        /** The 4-byte identifier. */
        char tag[4] = {0};
        /** The checksum for this table. */
        uint32_t checksum = 0;
        /** The byte offset from the beginning of "sfnt". */
        uint32_t offset = 0;
        /** The length of this table in bytes (actual length, not padded length). */
        uint32_t length = 0;
    };
    static_assert(sizeof(TableDirectoryEntry) == 16);

    struct FontHeader
    {
        // We skip a lot of information here that doesn't really interest us.

        /** Used to scale glyph coordinates (in font units) to pixels. Typically ranges from 64 to 16384. */
        uint16_t units_per_em = 0;

        /** For all glyph bounding boxes (in font units). */
        int16_t x_min = 0;
        /** For all glyph bounding boxes (in font units). */
        int16_t y_min = 0;
        /** For all glyph bounding boxes (in font units). */
        int16_t x_max = 0;
        /** For all glyph bounding boxes (in font units). */
        int16_t y_max = 0;

        /** Smallest readable size in pixels. */
        uint16_t lowest_rec_ppem = 0;

        /** 0 for short offsets (2 bytes), 1 for long (4 bytes). */
        int16_t index_to_loc_format = 0;
    };

    struct MaximumProfile
    {
        // We skip a lot of information here that doesn't really interest us.

        /** The number of glyphs in the font. */
        uint16_t num_glyphs = 0;
    };

    struct TrueTypeFontMetric
    {
        // We skip a lot of information here that doesn't really interest us.

        /** The typographic ascent, in font units. */
        int16_t ascender = 0;
        /** The typographic descent, in font units. */
        int16_t descender = 0;
        /** The typographic line gap (leading), in font units. */
        int16_t line_gap = 0;
        /** The maximum advance width value of glyphs, in font units. */
        uint16_t advance_width_max = 0;
        /** The minimum left-side bearing for glyphs with contours, in font units. */
        int16_t min_left_side_bearing = 0;
        /** The minimum right-side bearing for glyphs with contours, in font units. */
        int16_t min_right_side_bearing = 0;
        /** `max(lsb + (x_max - x_min))` */
        int16_t x_max_extent = 0;
    };

    struct OpenTypeFontMetric
    {
        // We skip a lot of information here that doesn't really interest us.

        /** The version number for the OS/2 table: 0x0000 to 0x0005. */
        uint16_t version = 0xffff;
        /** The arithmetic average of the escapement (width) of all non-zero width glyphs in the font. */
        int16_t avg_char_width = 0;

        /** The typographic ascender, in font units. */
        int16_t typo_ascender = 0;
        /** The typographic descender, in font units. */
        int16_t typo_descender = 0;
        /** The typographic line gap, in font units. */
        int16_t typo_line_gap = 0;

        /** The distance between the baseline and the approximate height of non-ascending lowercase letters (e.g., lowercase 'x'), in font units. */
        int16_t x_height = 0;
        /** The distance between the baseline and the approximate height of uppercase letters (e.g., uppercase 'H'), in font units. */
        int16_t cap_height = 0;
    };

    struct GlyphMetric
    {
        /** The advance width, in font units. */
        uint16_t advance_width = 0;
        /** The left-side bearing, in font units. */
        int16_t left_side_bearing = 0;
    };

    struct GlyphEntry
    {
        Glyph glyph = {};
        bool is_loaded = false;
    };



    bool _try_ensure_init();
    bool _init();

    bool _read_offset_subtable();
    bool _read_table_directory_entry(size_t index, TableDirectoryEntry& entry);

    cl7::io::ReadableMemory _read_table(const std::string& tag);

    bool _try_read_font_name();
    bool _read_font_header();
    bool _read_maximum_profile();
    bool _read_character_code_mapping();
    bool _read_character_code_mapping_format_4(cl7::io::ReadableMemory& readable);
    bool _read_character_code_mapping_format_12(cl7::io::ReadableMemory& readable);
    bool _read_glyph_offsets();

    bool _read_font_and_glyph_metrics();
    bool _try_read_os2_metrics();

    bool _read_glyph_data(std::span<const uint32_t> indices);
    bool _read_glyph_data(cl7::io::ReadableMemory& readable, uint32_t index);
    std::pair<Glyph, bool> _read_simple_glyph_description(cl7::io::ReadableMemory& readable, size_t number_of_contours);
    static std::pair<Glyph, bool> _read_composite_glyph_description(cl7::io::ReadableMemory& readable);
    static std::vector<int16_t> _read_glyph_coordinates(cl7::io::ReadableMemory& readable, const std::vector<uint8_t>& point_flags, uint8_t short_vector_flag, uint8_t is_same_or_positive_short_vector_flag);

    void _insert_loaded_glyph(uint32_t index, Glyph&& glyph);

    uint32_t _get_glyph_index(cl7::text::codec::codepoint codepoint) const;

    static uint32_t _calculate_checksum(cl7::byte_span data);



    std::optional<bool> _init_state;

    OffsetSubtable _offset_subtable;
    std::unordered_map<std::string, TableDirectoryEntry> _table_directory;

    cl7::u8string _font_name;
    FontHeader _font_header;
    MaximumProfile _maximum_profile;

    std::unordered_map<cl7::text::codec::codepoint::value_type, uint32_t> _glyph_index_map;
    std::vector<uint32_t> _glyph_offsets;

    TrueTypeFontMetric _true_type_font_metric;
    OpenTypeFontMetric _open_type_font_metric;
    std::vector<GlyphMetric> _glyph_metrics;

    std::vector<GlyphEntry> _glyph_entries;
    FontMetrics _font_metrics;

}; // class TrueTypeFontLoader



} // namespace fl7::fonts

#endif // FL7_FONTS_TRUETYPEFONTLOADER_H
