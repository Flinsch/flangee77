#ifndef FL7_FONTS_TRUETYPEFONTLOADER_H
#define FL7_FONTS_TRUETYPEFONTLOADER_H
#include "./FontLoader.h"

#include <CoreLabs/io/ReadableMemory.h>

#include <optional>
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

        /** Used to scale glyph coordinates to pixels. Typically ranges from 64 to 16384. */
        uint16_t units_per_em = 0;

        /** For all glyph bounding boxes. */
        int16_t x_min = 0;
        /** For all glyph bounding boxes. */
        int16_t y_min = 0;
        /** For all glyph bounding boxes. */
        int16_t x_max = 0;
        /** For all glyph bounding boxes. */
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

    static uint32_t _calculate_checksum(cl7::byte_span data);



    std::optional<bool> _init_state;

    OffsetSubtable _offset_subtable;
    std::unordered_map<std::string, TableDirectoryEntry> _table_directory;

    cl7::u8string _font_name;
    FontHeader _font_header;
    MaximumProfile _maximum_profile;

    std::unordered_map<cl7::text::codec::codepoint::value_type, uint32_t> _glyph_index_map;

}; // class TrueTypeFontLoader



} // namespace fl7::fonts

#endif // FL7_FONTS_TRUETYPEFONTLOADER_H
