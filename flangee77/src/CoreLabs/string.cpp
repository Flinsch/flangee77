#include "sstream.h"
#include "strings.h"

#include <CoreLabs/sstream.h>



namespace cl7 {


    template <typename T>
        requires(std::is_arithmetic_v<T>)
    static u8string _to_string(T val)
    {
        cl7::u8osstream oss;
        oss << std::fixed;// << std::setprecision(6);
        oss << val;
        return oss.str();
    }

    u8string to_string(bool val) { return {val ? u8"true" : u8"false"}; }

    u8string to_string(signed val)              { return _to_string(val); }
    u8string to_string(signed long val)         { return _to_string(val); }
    u8string to_string(signed long long val)    { return _to_string(val); }
    u8string to_string(unsigned val)            { return _to_string(val); }
    u8string to_string(unsigned long val)       { return _to_string(val); }
    u8string to_string(unsigned long long val)  { return _to_string(val); }
    u8string to_string(float val)               { return _to_string(val); }
    u8string to_string(double val)              { return _to_string(val); }
    u8string to_string(long double val)         { return _to_string(val); }



} // namespace cl7
