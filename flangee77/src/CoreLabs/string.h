#pragma once
#ifndef CL7_STRING_H
#define CL7_STRING_H

#include <CoreLabs/root.h>

#include <string>



/**
 * To all clients of this code, including myself, let me drop a few words about
 * Unicode and character encodings:
 * 
 * Every programmer, software developer, IT person, and perhaps even every human
 * using electronic devices should have dealt with "The Absolute Minimum Every
 * Software Developer Absolutely, Positively Must Know About Unicode and Character
 * Sets (No Excuses!)" by Joel Spolsky at least once:
 * https://www.joelonsoftware.com/2003/10/08/the-absolute-minimum-every-software-developer-absolutely-positively-must-know-about-unicode-and-character-sets-no-excuses/
 * This was recently taken up again in a blog post by Nikita Prokopov (Niki Tonsky),
 * which is certainly also worth a look:
 * https://tonsky.me/blog/unicode/
 * And even then you have only scratched the surface when it comes to the entire
 * topic.
 * 
 * Then Salik Tariq also has a few interesting things to say, including some best
 * practices regarding what encoding should be used when, which I have found
 * confirmed in a similar way in other places (and what indirectly corresponds to
 * what Joel Spolsky said):
 * https://www.linkedin.com/pulse/mastering-unicode-modern-c-comprehensive-guide-wide-characters-tariq
 * 
 * In summary, what this means for us here: The framework provides 5 string types of
 * different encodings (as well as corresponding character and string view types):
 *  cl7::astring    ASCII/Latin-1
 *  cl7::u8string   UTF-8
 *  cl7::u16string  UTF-16
 *  cl7::u32string  UTF-32
 *  cl7::wstring    UTF-16 (Windows) or UTF-32 (non-Windows), I guess
 * 
 * These types are simple typedefs of corresponding types from the C++ Standard
 * Library.
 * 
 * Then there is the generic string type, cl7::string, which maps to either
 * cl7::wstring or cl7::astring, depending on whether or not the use of Unicode is
 * enabled via the UNICODE preprocessor definition.
 * 
 * Regarding "best practices", the different string types are used within this
 * framework as follows (which you can follow in your own code, but of course you
 * don't have to):
 *  Internal processing         cl7::wstring (or cl7::u16string or cl7::u32string
 *                              specifically); sometimes cl7::astring for rather
 *                              language-independent technical stuff
 *  Storage and transmission    cl7::u8string mainly; sometimes cl7::astring for
 *                              rather language-independent technical stuff
 *  User interaction            cl7::wstring (or cl7::u16string or cl7::u32string
 *                              specifically)
 * 
 * ASCII/Latin-1 and UTF-32 have the great advantage that one character corresponds
 * to exactly one (representable) code point. This makes string manipulations easy
 * or even possible. (Let's ignore things like graphemes or normalization for now.)
 * In addition, UTF-32 has the advantage that "all" characters in the world are
 * supported (good for user interaction); ASCII/Latin-1 has/have limited coverage
 * but the advantage of being space-saving, simple, and "close to code" (good for
 * language-independent technical stuff).
 * UTF-8 tends to being space-saving as well, but at the same time has the ability
 * to "somehow" represent "all" characters (good for storage and transmission of
 * non-technical, language-dependent stuff).
 * UTF-16 is a kind of hybrid: it covers "all" characters, is okay in terms of
 * memory requirements, and allows for some string manipulations because many or even
 * most common code points can be represented with one single code unit (good/okay
 * for almost everything, but loses to UTF-8 in storage and transmission).
 * 
 * Don't be afraid to use different character encodings or string types within your
 * software. If you look at the different ways strings are used within a project,
 * you will see that in most cases different types don't even touch each other. But
 * where there are points of contact, use should of course be as uniform as possible
 * and conversions should be minimized. "Most" important thing is that you always
 * know which encoding a given string (or file, etc.) has.
 */



#if defined(UNICODE) ^ defined(_UNICODE)
#error Inconsistent UNICODE and _UNICODE definition.
#endif



namespace cl7 {



#ifdef UNICODE
    using char_type = wchar_t;
    using string = std::wstring;
    using string_view = std::wstring_view;
#else // => !UNICODE
    using char_type = char;
    using string = std::string;
    using string_view = std::string_view;
#endif // #else => !UNICODE

#ifdef  UNICODE
#define __TEXT(quote) L##quote      // NOLINT(bugprone-reserved-identifier)
#define __RTEXT(quote) LR##quote    // NOLINT(bugprone-reserved-identifier)
#else   /* UNICODE */               
#define __TEXT(quote) quote         
#define __RTEXT(quote) R##quote     
#endif /* UNICODE */                
#define TEXT(quote) __TEXT(quote)
#define RTEXT(quote) __RTEXT(quote)



    using achar_type        = char;
    using u8char_type       = char8_t;
    using u16char_type      = char16_t;
    using u32char_type      = char32_t;
    using wchar_type        = wchar_t;

    using astring           = std::string;
    using u8string          = std::u8string;
    using u16string         = std::u16string;
    using u32string         = std::u32string;
    using wstring           = std::wstring;

    using astring_view      = std::string_view;
    using u8string_view     = std::u8string_view;
    using u16string_view    = std::u16string_view;
    using u32string_view    = std::u32string_view;
    using wstring_view      = std::wstring_view;



    template <class Tstring>
    concept is_any_string_v = std::is_base_of_v<std::basic_string<typename Tstring::value_type, typename Tstring::traits_type, typename Tstring::allocator_type>, Tstring>;

    template <class Tstring_view>
    concept is_any_string_view_v = std::is_base_of_v<std::basic_string_view<typename Tstring_view::value_type, typename Tstring_view::traits_type>, Tstring_view>;

    template <class Tstring_or_view>
    concept is_any_string_or_view_v = is_any_string_v<Tstring_or_view> || is_any_string_view_v<Tstring_or_view>;



    template <class Tstring>
        requires(is_any_string_v<Tstring>)
    auto make_string_view(const Tstring& s)
    {
        return std::basic_string_view<typename Tstring::value_type, typename Tstring::traits_type>(s.data(), s.size());
    }



    template <typename Tchar = cl7::char_type>
    struct string_hash
    {
        using is_transparent = void;
        using Tstring_view = std::basic_string_view<Tchar>;
        using Tstring = std::basic_string<Tchar>;
        size_t operator () (const Tchar* str) const { return std::hash<Tstring_view>{}(str); }
        size_t operator () (Tstring_view str) const { return std::hash<Tstring_view>{}(str); }
        size_t operator () (const Tstring& str) const { return std::hash<Tstring>{}(str); }
    };

    using astring_hash = string_hash<cl7::achar_type>;
    using u8string_hash = string_hash<cl7::u8char_type>;
    using u16string_hash = string_hash<cl7::u16char_type>;
    using u32string_hash = string_hash<cl7::u32char_type>;
    using wstring_hash = string_hash<cl7::wchar_type>;



    inline astring to_astring(signed val) { return std::to_string(val); }
    inline astring to_astring(signed long val) { return std::to_string(val); }
    inline astring to_astring(signed long long val) { return std::to_string(val); }
    inline astring to_astring(unsigned val) { return std::to_string(val); }
    inline astring to_astring(unsigned long val) { return std::to_string(val); }
    inline astring to_astring(unsigned long long val) { return std::to_string(val); }
    inline astring to_astring(float val) { return std::to_string(val); }
    inline astring to_astring(double val) { return std::to_string(val); }
    inline astring to_astring(long double val) { return std::to_string(val); }

    inline wstring to_wstring(signed val) { return std::to_wstring(val); }
    inline wstring to_wstring(signed long val) { return std::to_wstring(val); }
    inline wstring to_wstring(signed long long val) { return std::to_wstring(val); }
    inline wstring to_wstring(unsigned val) { return std::to_wstring(val); }
    inline wstring to_wstring(unsigned long val) { return std::to_wstring(val); }
    inline wstring to_wstring(unsigned long long val) { return std::to_wstring(val); }
    inline wstring to_wstring(float val) { return std::to_wstring(val); }
    inline wstring to_wstring(double val) { return std::to_wstring(val); }
    inline wstring to_wstring(long double val) { return std::to_wstring(val); }


    inline astring to_astring(bool val) { return {val ? "true" : "false"}; }

    inline wstring to_wstring(bool val) { return {val ? L"true" : L"false"}; }


#ifdef UNICODE
    inline string to_string(signed val) { return to_wstring(val); }
    inline string to_string(signed long val) { return to_wstring(val); }
    inline string to_string(signed long long val) { return to_wstring(val); }
    inline string to_string(unsigned val) { return to_wstring(val); }
    inline string to_string(unsigned long val) { return to_wstring(val); }
    inline string to_string(unsigned long long val) { return to_wstring(val); }
    inline string to_string(float val) { return to_wstring(val); }
    inline string to_string(double val) { return to_wstring(val); }
    inline string to_string(long double val) { return to_wstring(val); }

    inline string to_string(bool val) { return to_wstring(val); }
#else // => !UNICODE
    inline string to_string(signed val) { return to_astring(val); }
    inline string to_string(signed long val) { return to_astring(val); }
    inline string to_string(signed long long val) { return to_astring(val); }
    inline string to_string(unsigned val) { return to_astring(val); }
    inline string to_string(unsigned long val) { return to_astring(val); }
    inline string to_string(unsigned long long val) { return to_astring(val); }
    inline string to_string(float val) { return to_astring(val); }
    inline string to_string(double val) { return to_astring(val); }
    inline string to_string(long double val) { return to_astring(val); }

    inline string to_string(bool val) { return to_astring(val); }
#endif // #else => !UNICODE



} // namespace cl7

#endif // CL7_STRING_H
