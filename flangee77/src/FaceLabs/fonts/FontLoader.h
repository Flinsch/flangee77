#ifndef FL7_FONTS_FONTLOADER_H
#define FL7_FONTS_FONTLOADER_H

#include "./Font.h"

#include <CoreLabs/io/File.h>



namespace fl7::fonts {



/**
 * An abstract base class for loading font data from a font file (e.g., TTF).
 */
class FontLoader
{

public:
    /**
     * Prepares a font loader for reading from the specified font file.
     */
    explicit FontLoader(cl7::u8string_view file_path) : _file_path(file_path), _file(file_path, cl7::io::OpenMode::Read) {}

    virtual ~FontLoader() = default;



    /** Returns the file path to the font file. */
    const cl7::u8string& get_file_path() const { return _file_path; }

    /**
     * Returns a fallback name based on the font's filename.
     */
    cl7::u8string get_fallback_name() const;



    /**
     * Checks whether the glyph corresponding to the specified Unicode code point
     * exists and can therefore potentially be loaded.
     */
    virtual bool has_glyph(cl7::strings::codec::codepoint codepoint) = 0;

    /**
     * Attempts to load the glyph corresponding to the specified Unicode code point.
     * If the glyph could not be loaded, an "empty" glyph is returned (in the sense
     * that it contains no contour points, has no advance width, etc.).
     */
    virtual Glyph load_glyph(cl7::strings::codec::codepoint codepoint) = 0;



protected:
    cl7::io::IReadable& _open(size_t byte_offset);
    void _close();



private:
    /** The file path to the font file. */
    cl7::u8string _file_path;

    /** The file for reading font data from. */
    cl7::io::File _file;

}; // class FontLoader



} // namespace fl7::fonts

#endif // FL7_FONTS_FONTLOADER_H
