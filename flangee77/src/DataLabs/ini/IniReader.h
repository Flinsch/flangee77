#ifndef DL7_INI_INIREADER_H
#define DL7_INI_INIREADER_H

#include "./Ini.h"

#include <DataLabs/syntax/Diagnostics.h>



namespace dl7::ini {



/**
 * Parses INI data from UTF-8 encoded string representations, keeping whatever it
 * had to complain about along the way.
 */
class IniReader
{

public:
    /**
     * Parses a UTF-8 encoded INI string and returns an `Ini` object. Whatever
     * there is to complain about ends up in the diagnostics, which are cleared
     * beforehand.
     */
    Ini parse(cl7::u8string_view source);

    /**
     * Returns the diagnostics of the most recent `parse` call.
     */
    const syntax::Diagnostics& get_diagnostics() const noexcept { return _diagnostics; }



private:
    syntax::Diagnostics _diagnostics;

}; // class IniReader



} // namespace dl7::ini

#endif // DL7_INI_INIREADER_H
