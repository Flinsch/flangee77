#include "sstream.h"
#include "strings.h"

#include <CoreLabs/sstream.h>

#include <cmath>



namespace cl7 {



    template <typename T>
        requires(std::is_arithmetic_v<T> && !std::is_floating_point_v<T>)
    static u8string _to_string(T val)
    {
        cl7::u8osstream oss;
        oss << val;
        return oss.str();
    }

    template <typename T>
        requires(std::is_arithmetic_v<T> && std::is_floating_point_v<T>)
    static u8string _to_string(T val)
    {
        cl7::u8osstream oss;

        T int_part;
        T frac_part = std::modf(val, &int_part);
        if (frac_part == T{0.0})
        {
            static constexpr auto min_int = static_cast<T>(std::numeric_limits<signed long>::min() + 1);
            static constexpr auto max_int = static_cast<T>(std::numeric_limits<signed long>::max() - 1);
            if (int_part >= min_int && int_part <= max_int)
            {
                oss << static_cast<signed long>(int_part) << u8".0";
                return oss.str();
            }

            oss << std::defaultfloat;
            oss << int_part;
        }
        else
        {
            oss << std::fixed;// << std::setprecision(6)
            oss << val;
        }

        u8string str = oss.str();
        if (str.length() < 3)
            return str;

        size_t p = 0;
        if (str[0] == u8'-')
            ++p;

        if (str[p] != u8'0' || str[p + 1] != u8'.')
            return str;

        for (p += 2; p < str.length(); ++p)
            if (str[p] != u8'0')
                return str;

        return u8"0.0";
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
