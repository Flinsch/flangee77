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
 * And then there is the "UTF-8 Everywhere" manifesto by Pavel Radzivilovsky, Yakov
 * Galka, and Slava Novgorodov:
 * https://utf8everywhere.org/
 * UTF-8 remains the most widely supported and efficient encoding for applications.
 * 
 * In summary, what this means for us here: The framework provides 5 string types of
 * different encodings (as well as corresponding character and string view types):
 *  cl7::astring    ASCII
 *  cl7::u8string   UTF-8
 *  cl7::u16string  UTF-16
 *  cl7::u32string  UTF-32
 *  cl7::wstring    UTF-16 (Windows) or UTF-32 (non-Windows), I guess
 * 
 * These types are simple typedefs of corresponding types from the C++ Standard
 * Library.
 * 
 * The intention of these types is to emphasize the respective encoding a little
 * more semantically, especially with regard to cl7::astring and cl7::u8string, as a
 * contrast to std::string, which can contain "any" encoding, ASCII, ANSI, UTF-8,
 * ISO 8859, etc. If you need the flexibility of std::string, you can still use it,
 * even in parallel with the types of this framework (which are just typedefs
 * anyway). You just have to be aware of which encoding you are using (which you
 * "have" to do anyway). This framework also sometimes uses std::string (internally)
 * here and there, but more for utility and compatibility purposes.
 * 
 * Regarding "best practices", the different string types are used within this
 * framework as follows (which you can follow in your own code, but of course you
 * don't have to):
 *  Platform-specific stuff     "Native" std::string and/or std::wstring
 *  Internal processing         Whatever serves, preferably cl7::u8string (even if
 *                              cl7::astring would be "enough" in case of rather
 *                              language-independent technical stuff)
 *  Storage and transmission    cl7::u8string
 *  User interaction            cl7::u32string
 * 
 * ASCII and UTF-32 have the great advantage that one character (code unit)
 * corresponds to exactly one (representable) code point. This makes string
 * manipulations easy or even possible (let's ignore things like graphemes or
 * normalization for now).
 * In addition, UTF-32 has the advantage that "all" symbols in the world are
 * supported (good for user interaction); ASCII has limited coverage but the
 * advantage of being space-saving, simple, and "close to code" (good for
 * language-independent technical stuff).
 * UTF-8 tends to being space-saving as well, but at the same time has the ability
 * to "somehow" represent "all" characters (good for storage and transmission of
 * language-dependent stuff and (other) special characters; as a superset of ASCII,
 * it is also suitable for language-independent technical stuff as well).
 * UTF-16 is a kind of hybrid: it covers "all" characters, is okay in terms of
 * memory requirements, and allows for some string manipulations because many or even
 * most common code points can be represented with one single code unit (good/okay
 * for almost everything, but loses to UTF-8 in storage and transmission). However,
 * UTF-16 is rarely used within this framework (tends to be used only in the case of
 * platform-specific things).
 * 
 * Despite the presence of the eighth bit of cl7::astring, we deliberately do not
 * support ANSI, ISO 8859, etc., not even Latin-1, although it corresponds 1-to-1 to
 * the first 256 Unicode characters. If you need these encodings (for whatever
 * reason, for example under Windows), you have to take care of them yourself. At
 * best, you avoid ANSI, ISO 8859, etc. as much as possible and stick with ASCII and
 * Unicode. And then you end up with UTF-8 as a superset of ASCII (or UTF-32 if you
 * need the quasi-1-to-1 correspondence of code units and code points).
 * 
 * So you see: UTF-8 (almost) everywhere. As mentioned before, you may have to take
 * care of ISO 8859, ANSI, etc. yourself, but to avoid this as much as possible, we
 * are forcing the UNICODE/_UNICODE definition, also as suggested by the manifesto,
 * "UTF-8 Everywhere".
 * 
 * I'm already taking cover, but despite everything, don't be afraid to use
 * different character encodings or string types within your software. If you look
 * at the different ways strings are used within a project, you will see that in
 * most cases different types don't even touch each other. But where there are
 * points of contact, you should of course be as uniform as possible and conversions
 * should be minimized. Okay, at least when it comes to logging, all possible
 * encodings potentially come into contact with each other. Anyway, "most" important
 * thing is that you always know which encoding a given string (or file, etc.) has.
 */



#if defined(UNICODE) ^ defined(_UNICODE)
#error Inconsistent UNICODE and _UNICODE definition.
#elif !defined(UNICODE) || !defined(_UNICODE)
#error Missing UNICODE and/or _UNICODE definition.
#endif



namespace cl7 {



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



    template <typename Tchar = cl7::u8char_type>
    struct string_hash
    {
        using is_transparent = void;
        using Tstring_view = std::basic_string_view<Tchar>;
        using Tstring = std::basic_string<Tchar>;
        size_t operator()(const Tchar* str) const { return std::hash<Tstring_view>{}(str); }
        size_t operator()(Tstring_view str) const { return std::hash<Tstring_view>{}(str); }
        size_t operator()(const Tstring& str) const { return std::hash<Tstring>{}(str); }
    };

    using astring_hash = string_hash<cl7::achar_type>;
    using u8string_hash = string_hash<cl7::u8char_type>;
    using u16string_hash = string_hash<cl7::u16char_type>;
    using u32string_hash = string_hash<cl7::u32char_type>;
    using wstring_hash = string_hash<cl7::wchar_type>;



    u8string to_string(bool val);

    u8string to_string(signed val);
    u8string to_string(signed long val);
    u8string to_string(signed long long val);
    u8string to_string(unsigned val);
    u8string to_string(unsigned long val);
    u8string to_string(unsigned long long val);
    u8string to_string(float val);
    u8string to_string(double val);
    u8string to_string(long double val);



} // namespace cl7

#endif // CL7_STRING_H
