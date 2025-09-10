#include "format.h"



namespace cl7::strings::format {



    u8string to_hex(unsigned long long val, unsigned pad_zeros, u8char_t ca)
    {
        return to_hex<u8string>(val, pad_zeros, ca);
    }

    u8string to_0xhex(unsigned long long val, unsigned pad_zeros, u8char_t ca)
    {
        return to_0xhex<u8string>(val, pad_zeros, ca);
    }



} // namespace cl7::strings::format
