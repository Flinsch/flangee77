#include "sstream.h"

#include <CoreLabs/strings.h>



namespace cl7 {



    u8isstream::u8isstream(u8string_view str)
        : _iss(std::string(cl7::strings::reinterpret_utf8(str)))
    {
    }

    unsigned int u8isstream::peek()
    {
        return static_cast<unsigned int>(_iss.peek());
    }

    unsigned int u8isstream::get()
    {
        return static_cast<unsigned int>(_iss.get());
    }

    u8isstream& u8isstream::get(u8char_type& ch)
    {
        char ch_;
        _iss.get(ch_);
        ch = static_cast<u8char_type>(ch_);
        return *this;
    }

    u8isstream& u8isstream::getline(u8char_type* s, std::streamsize count, u8char_type delim)
    {
        _iss.getline(reinterpret_cast<char*>(s), count, static_cast<char>(delim));
        return *this;
    }

    u8isstream& u8isstream::ignore(std::streamsize count, unsigned int delim)
    {
        _iss.ignore(count, static_cast<int>(delim));
        return *this;
    }

    u8isstream::operator bool() const
    {
        return static_cast<bool>(_iss);
    }



    u8string u8osstream::str() const
    {
        return u8string{cl7::strings::reinterpret_utf8(_oss.str())};
    }

    u8osstream& u8osstream::put(u8char_type ch)
    {
        _oss << static_cast<char>(ch);
        return *this;
    }

    u8osstream& u8osstream::operator<<(u8char_type value)
    {
        _oss << static_cast<char>(value);
        return *this;
    }

    u8osstream& u8osstream::operator<<(u8string_view value)
    {
        _oss << cl7::strings::reinterpret_utf8(value);
        return *this;
    }

    u8osstream& u8osstream::operator<<(const u8string& value)
    {
        _oss << cl7::strings::reinterpret_utf8(value);
        return *this;
    }

    u8osstream& u8osstream::operator<<(const u8char_type* value)
    {
        _oss << cl7::strings::reinterpret_utf8(value);
        return *this;
    }

    u8osstream::operator bool() const
    {
        return static_cast<bool>(_oss);
    }



} // namespace cl7



namespace std {

    cl7::u8isstream& getline(cl7::u8isstream& input, cl7::u8string& str, cl7::u8char_type delim)
    {
        str.clear();
        cl7::u8char_type ch;
        while (input.get(ch) && ch != delim)
            str.push_back(ch);
        return input;
    }

} // namespace std
