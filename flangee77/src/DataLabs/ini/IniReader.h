#ifndef DL7_INI_INIREADER_H
#define DL7_INI_INIREADER_H

#include "./Ini.h"



namespace dl7::ini {



/**
 * A utility class for parsing INI data from UTF-8 encoded string representations.
 */
class IniReader
{

public:
    /**
     * Parses a UTF-8 encoded INI string and returns an `Ini` object.
     */
    static Ini parse(cl7::u8string_view source);

}; // class IniReader



} // namespace dl7::ini

#endif // DL7_INI_INIREADER_H
