#include "TrueTypeFontLoader.h"

#include "CoreLabs/io/ReadableMemory.h"
#include "CoreLabs/io/EndianAwareReader.h"
#include <CoreLabs/logging.h>
#include <CoreLabs/strings.h>



namespace fl7::fonts {



    /**
     * Checks whether the glyph corresponding to the specified Unicode code point
     * exists and can therefore potentially be loaded.
     */
    bool TrueTypeFontLoader::has_glyph(cl7::strings::codepoint codepoint)
    {
        if (!_try_ensure_init())
            return false;

        return _glyph_index_map.contains(codepoint.value);
    }

    /**
     * Attempts to load the glyph corresponding to the specified Unicode code point.
     * If the glyph could not be loaded, an "empty" glyph is returned (in the sense
     * that it contains no contour points, has no advance width, etc.).
     */
    Glyph TrueTypeFontLoader::load_glyph(cl7::strings::codepoint codepoint)
    {
        if (!_try_ensure_init())
            return {};

        return {};
    }



    bool TrueTypeFontLoader::_try_ensure_init()
    {
        if (!_init_state.has_value())
            _init_state = _init();

        return _init_state.value();
    }

    bool TrueTypeFontLoader::_init()
    {
        assert(!_init_state.has_value());

        auto& file = _open(0);
        if (!file.is_readable())
        {
            LOG_ERROR(u8"The TrueType font source cannot be processed.");
            return false;
        }

        if (!_read_offset_subtable())
        {
            LOG_ERROR(u8"Error reading TrueType font offset subtable.");
            return false;
        }

        assert(_offset_subtable.scaler_type == 0x00010000);

        if (_offset_subtable.num_tables == 0)
        {
            LOG_ERROR(u8"No offset subtables found in TrueType font source.");
            return false;
        }

        for (uint16_t i = 0; i < _offset_subtable.num_tables; ++i)
        {
            TableDirectoryEntry entry;

            if (!_read_table_directory_entry(static_cast<size_t>(i), entry))
            {
                LOG_ERROR(u8"Error reading TrueType font table directory entry.");
                return false;
            }

            char tag[5] = {0};
            tag[0] = entry.tag[0];
            tag[1] = entry.tag[1];
            tag[2] = entry.tag[2];
            tag[3] = entry.tag[3];

            assert(!_table_directory.contains(std::string(tag)));

            _table_directory[std::string(tag)] = entry;
        } // for each table directory entry

        if (!_try_read_font_name())
        {
            LOG_WARNING(u8"Error reading TrueType font name.");
        }

        if (!_read_font_header())
        {
            LOG_ERROR(u8"Error reading TrueType font header.");
            return false;
        }

        if (!_read_maximum_profile())
        {
            LOG_ERROR(u8"Error reading TrueType font maximum profile.");
            return false;
        }

        if (!_read_character_code_mapping())
        {
            LOG_ERROR(u8"Error reading TrueType font character code mapping.");
            return false;
        }

        _close();

        return true;
    }

    bool TrueTypeFontLoader::_read_offset_subtable()
    {
        auto& file = _open(0);
        cl7::io::EndianAwareReader<std::endian::big> reader{&file};

        if (reader.read_scalar(_offset_subtable.scaler_type) != sizeof(_offset_subtable.scaler_type)) return false;
        if (reader.read_scalar(_offset_subtable.num_tables) != sizeof(_offset_subtable.num_tables)) return false;
        if (reader.read_scalar(_offset_subtable.search_range) != sizeof(_offset_subtable.search_range)) return false;
        if (reader.read_scalar(_offset_subtable.entry_selector) != sizeof(_offset_subtable.entry_selector)) return false;
        if (reader.read_scalar(_offset_subtable.range_shift) != sizeof(_offset_subtable.range_shift)) return false;

        return true;
    }

    bool TrueTypeFontLoader::_read_table_directory_entry(size_t index, TableDirectoryEntry& entry)
    {
        auto& file = _open(index * sizeof(TableDirectoryEntry) + sizeof(OffsetSubtable));
        cl7::io::EndianAwareReader<std::endian::big> reader{&file};

        if (reader.read_bytes(cl7::make_byte_span(entry.tag, 4)) != 4)
            return false;

        if (reader.read_scalar(entry.checksum) != sizeof(entry.checksum)) return false;
        if (reader.read_scalar(entry.offset) != sizeof(entry.offset)) return false;
        if (reader.read_scalar(entry.length) != sizeof(entry.length)) return false;

        return true;
    }

    cl7::io::ReadableMemory TrueTypeFontLoader::_read_table(const std::string& tag)
    {
        if (!_table_directory.contains(tag))
            return {};

        const auto& entry = _table_directory[tag];

        const auto offset = static_cast<size_t>(entry.offset);
        const auto length = static_cast<size_t>(entry.length);

        cl7::byte_vector buffer(length);

        auto& file = _open(offset);
        cl7::io::ByteReader reader{&file};

        if (reader.read_bytes(cl7::make_byte_span(buffer)) != buffer.size())
            return {};

        auto checksum = _calculate_checksum(buffer);

        if (tag == "head")
        {
            cl7::io::ReadableMemory tmp_readable{buffer};
            cl7::io::EndianAwareReader<std::endian::big> tmp_reader{&tmp_readable};
            tmp_readable.seek_read(8); // Skip (major/minor) version and font_revision
            auto checksum_adjustment = tmp_reader.read_scalar<uint32_t>();
            checksum -= checksum_adjustment;
        }

        if (checksum != entry.checksum)
        {
            LOG_ERROR(u8"Checksum verification of table \"" + cl7::u8string(cl7::strings::reinterpret_utf8(tag)) + u8"\" failed.");
            return {};
        }

        return buffer;
    }

    bool TrueTypeFontLoader::_try_read_font_name()
    {
        // Prepare fallback from the filename.
        _font_name = get_fallback_name();

        auto readable = _read_table("name");
        if (readable.is_eof())
        {
            LOG_WARNING(u8"Could not read \"name\" table from TrueType font source.");
            return false;
        }

        cl7::io::EndianAwareReader<std::endian::big> reader{&readable};

        readable.seek_read(2); // Skip format

        auto name_record_count = reader.read_scalar<uint16_t>();
        if (name_record_count == 0)
        {
            LOG_WARNING(u8"No name records found in TrueType font source.");
            return false;
        }

        auto string_offset = static_cast<size_t>(reader.read_scalar<uint16_t>());

        cl7::u16string best_utf16_en_us;
        cl7::u16string best_utf16_any;
        cl7::astring best_assumed_ascii_en_us;
        cl7::astring best_assumed_ascii_any;

        for (uint16_t i = 0; i < _offset_subtable.num_tables; ++i)
        {
            uint16_t platform_id = 0;
            uint16_t platform_specific_id = 0;
            uint16_t language_id = 0;
            uint16_t name_id = 0;
            uint16_t length = 0;
            uint16_t offset = 0;

            if (reader.read_scalar(platform_id) != sizeof(platform_id)) return false;
            if (reader.read_scalar(platform_specific_id) != sizeof(platform_specific_id)) return false;
            if (reader.read_scalar(language_id) != sizeof(language_id)) return false;
            if (reader.read_scalar(name_id) != sizeof(name_id)) return false;
            if (reader.read_scalar(length) != sizeof(length)) return false;
            if (reader.read_scalar(offset) != sizeof(offset)) return false;

            // We are only interested in
            // the full name of the font.
            if (name_id != 4)
                continue;

            bool is_utf16 = false;
            is_utf16 |= platform_id == 0 && (platform_specific_id == 3 || platform_specific_id == 4); // Unicode (0): BMP only (3) or full repertoire (4)
            is_utf16 |= platform_id == 3 && (platform_specific_id == 1 || platform_specific_id == 10); // Microsoft Windows (3): BMP only (1) or full repertoire (10)

            bool is_en_us = platform_id == 3 && language_id == 0x0409; // Microsoft Windows (3): en-US (0x0409)

            auto position = readable.get_read_position();
            readable.set_read_position(string_offset + static_cast<size_t>(offset));

            if (is_utf16)
            {
                assert(length % 2 == 0);
                cl7::byte_vector bytes(static_cast<size_t>(length) + 2);
                bytes[0] = std::byte{0xfe};
                bytes[1] = std::byte{0xff};
                reader.read_bytes(cl7::make_byte_span(bytes.data() + 2, bytes.size() - 2));

                if (is_en_us)
                {
                    best_utf16_en_us = cl7::strings::to_utf16_unchecked(bytes);
                    break; // early out
                }

                if (best_utf16_any.empty())
                    best_utf16_any = cl7::strings::to_utf16_unchecked(bytes);
            }
            else
            {
                cl7::byte_vector bytes(static_cast<size_t>(length));
                reader.read_bytes(cl7::make_byte_span(bytes));

                if (is_en_us)
                    best_assumed_ascii_en_us = cl7::strings::to_ascii_unchecked(bytes);
                else if (best_assumed_ascii_any.empty())
                    best_assumed_ascii_any = cl7::strings::to_ascii_unchecked(bytes);
            }

            readable.set_read_position(position);
        } // for each name record

        if (!best_utf16_en_us.empty())
        {
            _font_name = cl7::strings::to_utf8(best_utf16_en_us);
            return true;
        }

        if (!best_utf16_any.empty())
        {
            _font_name = cl7::strings::to_utf8(best_utf16_any);
            return true;
        }

        if (!best_assumed_ascii_en_us.empty())
        {
            _font_name = cl7::strings::to_utf8(best_assumed_ascii_en_us);
            return true;
        }

        if (!best_assumed_ascii_any.empty())
        {
            _font_name = cl7::strings::to_utf8(best_assumed_ascii_any);
            return true;
        }

        LOG_WARNING(u8"No font name found in TrueType font source.");
        return false;
    }

    bool TrueTypeFontLoader::_read_font_header()
    {
        auto readable = _read_table("head");
        if (readable.is_eof())
        {
            LOG_ERROR(u8"Could not read \"head\" table from TrueType font source.");
            return false;
        }

        cl7::io::EndianAwareReader<std::endian::big> reader{&readable};

        readable.seek_read(18); // Skip (major/minor) version, font_revision, checksum_adjustment, magic_number, and flags

        if (reader.read_scalar(_font_header.units_per_em) != sizeof(_font_header.units_per_em))
            return false;

        readable.seek_read(16); // Skip created and modified

        if (reader.read_scalar(_font_header.x_min) != sizeof(_font_header.x_min)) return false;
        if (reader.read_scalar(_font_header.y_min) != sizeof(_font_header.y_min)) return false;
        if (reader.read_scalar(_font_header.x_max) != sizeof(_font_header.x_max)) return false;
        if (reader.read_scalar(_font_header.y_max) != sizeof(_font_header.y_max)) return false;

        readable.seek_read(2); // Skip mac_style

        if (reader.read_scalar(_font_header.lowest_rec_ppem) != sizeof(_font_header.lowest_rec_ppem))
            return false;

        readable.seek_read(2); // Skip font_direction_hint

        if (reader.read_scalar(_font_header.index_to_loc_format) != sizeof(_font_header.index_to_loc_format))
            return false;

        return _font_header.units_per_em > 0;
    }

    bool TrueTypeFontLoader::_read_maximum_profile()
    {
        auto readable = _read_table("maxp");
        if (readable.is_eof())
        {
            LOG_ERROR(u8"Could not read \"maxp\" table from TrueType font source.");
            return false;
        }

        cl7::io::EndianAwareReader<std::endian::big> reader{&readable};

        readable.seek_read(4); // Skip version

        if (reader.read_scalar(_maximum_profile.num_glyphs) != sizeof(_maximum_profile.num_glyphs))
            return false;

        return _maximum_profile.num_glyphs > 0;
    }

    bool TrueTypeFontLoader::_read_character_code_mapping()
    {
        auto readable = _read_table("cmap");
        if (readable.is_eof())
        {
            LOG_ERROR(u8"Could not read \"cmap\" table from TrueType font source.");
            return false;
        }

        cl7::io::EndianAwareReader<std::endian::big> reader{&readable};

        readable.seek_read(2); // Skip version

        auto num_subtables = reader.read_scalar<uint16_t>();
        if (num_subtables == 0)
        {
            LOG_ERROR(u8"No character code mapping subtables found in TrueType font source.");
            return false;
        }

        struct CmapSubtableInfo
        {
            /** Format 4: legacy BMP-only fonts in the range up to 0xFFFF; or Format 12: full Unicode range. */
            uint16_t format = 0;
            /** The byte offset in the font data, relative to the "cmap" offset. */
            uint32_t offset = 0;
        };

        CmapSubtableInfo best_subtable_info;

        for (uint16_t i = 0; i < _offset_subtable.num_tables; ++i)
        {
            uint16_t platform_id = 0;
            uint16_t platform_specific_id = 0;
            uint32_t offset = 0;

            if (reader.read_scalar(platform_id) != sizeof(platform_id)) return false;
            if (reader.read_scalar(platform_specific_id) != sizeof(platform_specific_id)) return false;
            if (reader.read_scalar(offset) != sizeof(offset)) return false;

            auto position = readable.get_read_position();
            readable.set_read_position(static_cast<size_t>(offset));

            uint16_t format = 0;
            bool format_okay = reader.read_scalar(format) == sizeof(format);

            readable.set_read_position(position);

            if (!format_okay)
                continue;

            // Should we still check the platform ID (Unicode, Microsoft Windows),
            // or should we be satisfied with the fact that we've found format 4 or 12?

            // Prefer Format 12: full Unicode range
            if (format == 12)
            {
                best_subtable_info = {
                    .format = format,
                    .offset = offset,
                };
                break;
            }

            // Accept Format 4: legacy BMP-only fonts
            if (format == 4)
            {
                assert(best_subtable_info.format != 12);
                best_subtable_info = {
                    .format = format,
                    .offset = offset,
                };
            }
        } // for each subtable

        readable.set_read_position(static_cast<size_t>(best_subtable_info.offset));

        switch (best_subtable_info.format)
        {
        case 4:
            if (!_read_character_code_mapping_format_4(readable))
            {
                LOG_ERROR(u8"Error reading TrueType font Format 4 character-to-glyph-index mapping subtable (Unicode BMP only).");
                return false;
            }
            return true;

        case 12:
            if (!_read_character_code_mapping_format_12(readable))
            {
                LOG_ERROR(u8"Error reading TrueType font Format 12 character-to-glyph-index mapping subtable (Unicode full repertoire).");
                return false;
            }
            return true;

        default:
            LOG_ERROR(u8"No suitable character-to-glyph-index mapping subtable format found in TrueType font source. Only Format 4 (Unicode BMP only) and Format 12 (Unicode full repertoire) are supported.");
            return false;
        } // switch subtable format
    }

    bool TrueTypeFontLoader::_read_character_code_mapping_format_4(cl7::io::ReadableMemory& readable)
    {
        assert(_glyph_index_map.empty());

        cl7::io::EndianAwareReader<std::endian::big> reader{&readable};

        auto format = reader.read_scalar<uint16_t>();
        assert(format == 4);

        readable.seek_read(4); // Skip length and language

        auto seg_count_x2 = reader.read_scalar<uint16_t>();
        if (seg_count_x2 == 0)
        {
            LOG_ERROR(u8"No contiguous range segments found in TrueType font source.");
            return false;
        }

        assert(seg_count_x2 % 2 == 0);
        auto seg_count = static_cast<size_t>(seg_count_x2 / 2);

        readable.seek_read(6); // Skip search_range, entry_selector, and range_shift

        std::vector<uint16_t> end_codes(seg_count);
        std::vector<uint16_t> start_codes(seg_count);
        std::vector<int16_t> id_deltas(seg_count);
        std::vector<uint16_t> id_range_offsets(seg_count);

        for (auto& end_code : end_codes) if (reader.read_scalar(end_code) != sizeof(end_code)) return false;
        readable.seek_read(2); // Skip reserved_pad
        for (auto& start_code : start_codes) if (reader.read_scalar(start_code) != sizeof(start_code)) return false;
        for (auto& id_delta : id_deltas) if (reader.read_scalar(id_delta) != sizeof(id_delta)) return false;
        for (auto& id_range_offset : id_range_offsets) if (reader.read_scalar(id_range_offset) != sizeof(id_range_offset)) return false;

        auto glyph_id_array_offset = readable.get_read_position();

        for (size_t i = 0; i < seg_count; ++i)
        {
            auto start_code = start_codes[i];
            auto end_code = end_codes[i];
            auto id_delta = id_deltas[i];
            auto id_range_offset = id_range_offsets[i];

            assert(start_code <= end_code);

            uint16_t char_count = end_code - start_code + 1;
            for (uint16_t c = 0; c < char_count; ++c)
            {
                uint16_t glyph_index = 0;

                if (id_range_offset == 0)
                {
                    glyph_index = static_cast<uint16_t>(start_code + id_delta) % 65536;
                }
                else
                {
                    assert(glyph_id_array_offset > seg_count * 2);
                    auto base_offset = glyph_id_array_offset - seg_count * 2;
                    auto offset = id_range_offset + c * 2;
                    readable.set_read_position(base_offset + static_cast<size_t>(offset));
                    glyph_index = reader.read_scalar<uint16_t>();
                    if (glyph_index != 0)
                        glyph_index = static_cast<uint16_t>(glyph_index + id_delta) % 65536;
                }

                if (glyph_index != 0)
                    _glyph_index_map[static_cast<cl7::strings::codepoint::value_type>(start_code + c)] = static_cast<uint32_t>(glyph_index);
            } // for each char code
        } // for each contiguous range segment

        return true;
    }

    bool TrueTypeFontLoader::_read_character_code_mapping_format_12(cl7::io::ReadableMemory& readable)
    {
        assert(_glyph_index_map.empty());

        cl7::io::EndianAwareReader<std::endian::big> reader{&readable};

        auto format = reader.read_scalar<uint16_t>();
        assert(format == 12);

        readable.seek_read(10); // Skip reserved, length, and language

        auto num_groups = reader.read_scalar<uint32_t>();
        if (num_groups == 0)
        {
            LOG_ERROR(u8"No sequential map group records found in TrueType font source.");
            return false;
        }

        for (uint32_t i = 0; i < num_groups; ++i)
        {
            uint32_t start_char_code = 0;
            uint32_t end_char_code = 0;
            uint32_t start_glyph_id = 0;

            if (reader.read_scalar(start_char_code) != sizeof(start_char_code)) return false;
            if (reader.read_scalar(end_char_code) != sizeof(end_char_code)) return false;
            if (reader.read_scalar(start_glyph_id) != sizeof(start_glyph_id)) return false;

            assert(start_char_code <= end_char_code);

            for (uint32_t char_code = start_char_code; char_code <= end_char_code; ++char_code)
            {
                _glyph_index_map[static_cast<cl7::strings::codepoint::value_type>(char_code)] = start_glyph_id + (char_code - start_char_code);
            } // for each char code
        } // for each sequential map group

        return true;
    }

    uint32_t TrueTypeFontLoader::_calculate_checksum(cl7::byte_span data)
    {
        uint32_t checksum = 0;

        const auto size_bytes = data.size_bytes();
        size_t i = 0;

        // Process full 4-byte chunks.
        for (; i + 4 <= size_bytes; i += 4)
        {
            uint32_t qword =
                (static_cast<uint32_t>(static_cast<uint8_t>(data[i + 0])) << 24) |
                (static_cast<uint32_t>(static_cast<uint8_t>(data[i + 1])) << 16) |
                (static_cast<uint32_t>(static_cast<uint8_t>(data[i + 2])) << 8) |
                (static_cast<uint32_t>(static_cast<uint8_t>(data[i + 3])) << 0);
            checksum += qword;
        }

        // Handle padding for leftover bytes.
        if (i < size_bytes)
        {
            uint32_t qword = 0;
            for (size_t j = 0; i + j < size_bytes; ++j)
                qword |= (static_cast<uint32_t>(static_cast<uint8_t>(data[i + j])) << (24 - 8 * j));
            checksum += qword;
        }

        return checksum;
    }



} // namespace fl7::fonts
