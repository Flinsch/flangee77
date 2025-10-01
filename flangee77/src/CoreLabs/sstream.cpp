#include "sstream.h"

#include <CoreLabs/text/codec.h>



namespace cl7 {



    u8isstream::u8isstream(u8string_view str)
        : _iss(std::string(cl7::text::codec::reinterpret_utf8(str)))
    {
    }

    void u8isstream::str(const u8string& s)
    {
        _iss.str(std::string(cl7::text::codec::reinterpret_utf8(s)));
    }

    void u8isstream::str(u8string&& s)
    {
        _iss.str(std::string(cl7::text::codec::reinterpret_utf8(std::move(s))));
    }

    unsigned int u8isstream::peek()
    {
        return static_cast<unsigned int>(_iss.peek());
    }

    unsigned int u8isstream::get()
    {
        return static_cast<unsigned int>(_iss.get());
    }

    u8isstream& u8isstream::get(u8char_t& ch)
    {
        char ch_;
        _iss.get(ch_);
        ch = static_cast<u8char_t>(ch_);
        return *this;
    }

    u8isstream& u8isstream::getline(u8char_t* s, std::streamsize count, u8char_t delim)
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



    void u8osstream::str(const u8string& s)
    {
        _oss.str(std::string(cl7::text::codec::reinterpret_utf8(s)));
    }

    void u8osstream::str(u8string&& s)
    {
        _oss.str(std::string(cl7::text::codec::reinterpret_utf8(std::move(s))));
    }

    u8string u8osstream::str() const
    {
        return u8string{cl7::text::codec::reinterpret_utf8(_oss.view())};
    }

    u8string_view u8osstream::view() const
    {
        return cl7::text::codec::reinterpret_utf8(_oss.view());
    }

    u8osstream& u8osstream::put(u8char_t ch)
    {
        _oss << static_cast<char>(ch);
        return *this;
    }

    u8osstream& u8osstream::operator<<(u8char_t value)
    {
        _oss << static_cast<char>(value);
        return *this;
    }

    u8osstream& u8osstream::operator<<(u8string_view value)
    {
        _oss << cl7::text::codec::reinterpret_utf8(value);
        return *this;
    }

    u8osstream& u8osstream::operator<<(const u8string& value)
    {
        _oss << cl7::text::codec::reinterpret_utf8(value);
        return *this;
    }

    u8osstream& u8osstream::operator<<(const u8char_t* value)
    {
        _oss << cl7::text::codec::reinterpret_utf8(value);
        return *this;
    }

    u8osstream::operator bool() const
    {
        return static_cast<bool>(_oss);
    }



} // namespace cl7



namespace std {

    cl7::u8isstream& getline(cl7::u8isstream& input, cl7::u8string& str, cl7::u8char_t delim)
    {
        str.clear();
        cl7::u8char_t ch;
        while (input.get(ch) && ch != delim)
            str.push_back(ch);
        return input;
    }

} // namespace std
