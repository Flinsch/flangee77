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
    typedef wchar_t char_type;
    typedef std::wstring string;
    typedef std::wstring_view string_view;
#else // => !UNICODE
    typedef char char_type;
    typedef std::string string;
    typedef std::string_view string_view;
#endif // #else => !UNICODE

#ifdef  UNICODE                     
#define __TEXT(quote) L##quote      
#define __RTEXT(quote) LR##quote    
#else   /* UNICODE */               
#define __TEXT(quote) quote         
#define __RTEXT(quote) R##quote     
#endif /* UNICODE */                
#define TEXT(quote) __TEXT(quote)   
#define RTEXT(quote) __RTEXT(quote) 



    typedef char        achar_type;
    typedef char8_t     u8char_type;
    typedef char16_t    u16char_type;
    typedef char32_t    u32char_type;
    typedef wchar_t     wchar_type;

    typedef std::string     astring;
    typedef std::u8string   u8string;
    typedef std::u16string  u16string;
    typedef std::u32string  u32string;
    typedef std::wstring    wstring;

    typedef std::string_view    astring_view;
    typedef std::u8string_view  u8string_view;
    typedef std::u16string_view u16string_view;
    typedef std::u32string_view u32string_view;
    typedef std::wstring_view   wstring_view;



    template <typename T>
    astring to_astring(const T& val) { return std::to_string( val ); }
    template <typename T>
    wstring to_wstring(const T& val) { return std::to_wstring( val ); }

    template <> inline
    astring to_astring(const bool& val) { return cl7::astring(val ? "true" : "false"); }
    template <> inline
    wstring to_wstring(const bool& val) { return cl7::wstring(val ? L"true" : L"false"); }

#ifdef UNICODE
    template <typename T>
    string to_string(const T& val) { return to_wstring( val ); }
#else // => !UNICODE
    template <typename T>
    string to_string(const T& val) { return to_astring( val ); }
#endif // #else => !UNICODE



} // namespace cl7

#endif // CL7_STRING_H
