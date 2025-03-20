#ifndef CL7_SSTREAM_H
#define CL7_SSTREAM_H

#include <CoreLabs/string.h>

#include <sstream>



namespace cl7 {



    using aisstream = std::istringstream;
    using wisstream = std::wistringstream;

    using aosstream = std::ostringstream;
    using wosstream = std::wostringstream;

    // As long as the "experts" from the C++ committee cannot get the simplest
    // Unicode-related things under control, we will have to take care of certain
    // things ourselves (see below).

    //using u8isstream = std::basic_istringstream<char8_t>;
    using u16isstream = std::basic_istringstream<char16_t>;
    using u32isstream = std::basic_istringstream<char32_t>;

    //using u8osstream = std::basic_ostringstream<char8_t>;
    using u16osstream = std::basic_ostringstream<char16_t>;
    using u32osstream = std::basic_ostringstream<char32_t>;



class u8isstream
{
public:
    u8isstream(u8string_view str);

    unsigned int peek();
    unsigned int get();
    u8isstream& get(u8char_t& ch);
    u8isstream& getline(u8char_t* s, std::streamsize count, u8char_t delim = cl7::u8char_t{'\n'});
    u8isstream& ignore(std::streamsize count = 1, unsigned int delim = std::char_traits<u8char_t>::eof());

    template <typename T>
    u8isstream& operator>>(T& value)
    {
        _iss >> value;
        return *this;
    }

    explicit operator bool() const;

private:
    std::istringstream _iss;
};



class u8osstream
{
public:
    u8string str() const;

    u8osstream& put(u8char_t ch);

    template <typename T>
        requires(std::is_fundamental_v<T>)
    u8osstream& operator<<(T value)
    {
        _oss << value;
        return *this;
    }

    template <typename T>
        requires(!std::is_fundamental_v<T>)
    u8osstream& operator<<(const T& value)
    {
        _oss << value;
        return *this;
    }

    u8osstream& operator<<(u8char_t value);
    u8osstream& operator<<(u8string_view value);
    u8osstream& operator<<(const u8string& value);
    u8osstream& operator<<(const u8char_t* value);

    explicit operator bool() const;

private:
    std::ostringstream _oss;
};



} // namespace cl7



namespace std {

    cl7::u8isstream& getline(cl7::u8isstream& input, cl7::u8string& str, cl7::u8char_t delim = cl7::u8char_t{'\n'});

} // namespace std

#endif // CL7_SSTREAM_H
