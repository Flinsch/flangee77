
# Strings and Encodings

To all clients of this code, including myself, let me drop a few words about Unicode and character encodings:

Every programmer, software developer, IT person, and perhaps even every human using electronic devices should have dealt with
&ldquo;[The Absolute Minimum Every Software Developer Absolutely, Positively Must Know About Unicode and Character Sets (No Excuses!)](https://www.joelonsoftware.com/2003/10/08/the-absolute-minimum-every-software-developer-absolutely-positively-must-know-about-unicode-and-character-sets-no-excuses/)&rdquo;
by Joel Spolsky at least once.
This was recently taken up again in a blog post by Nikita Prokopov (Niki Tonsky), which is certainly also worth a look:
[The Absolute Minimum Every Software Developer Must Know About Unicode in 2023 (Still No Excuses!)](https://tonsky.me/blog/unicode/).
And even then you have only scratched the surface when it comes to the entire topic.

Then Salik Tariq also has a few interesting things to say, including some best practices regarding what encoding should be used when, which I have found confirmed in a similar way in other places (and what indirectly corresponds to what Joel Spolsky said):
[Mastering Unicode in Modern C++: A Comprehensive Guide to Wide Characters, Encodings, and Best Practices](https://www.linkedin.com/pulse/mastering-unicode-modern-c-comprehensive-guide-wide-characters-tariq).

And then there is the &ldquo;[UTF-8 Everywhere](https://utf8everywhere.org/)&rdquo; manifesto by Pavel Radzivilovsky, Yakov Galka, and Slava Novgorodov.
UTF-8 remains the most widely supported and efficient encoding for applications.

In summary, what this means for us here: The framework provides 5 string types of
different encodings (as well as corresponding character and string-view types):
* `cl7::astring`: ASCII
* `cl7::u8string`: UTF-8
* `cl7::u16string`: UTF-16
* `cl7::u32string`: UTF-32
* `cl7::wstring`: UTF-16 (Windows) or UTF-32 (non-Windows), I guess

These types are simple &ldquo;typedefs&rdquo; of corresponding types from the C++ Standard Library.

The intention of these types is to emphasize the respective encoding a little more semantically, especially with regard to `cl7::astring` and `cl7::u8string`, as a contrast to `std::string`, which can contain any one-byte character encoding, ASCII, ANSI, UTF-8, ISO 8859, etc.
If you need the flexibility of `std::string`, you can still use it, even in parallel with the types of this framework (which are just typedefs anyway).
This framework also sometimes uses `std::string` etc. here and there (internally), but more for utility and compatibility purposes.

Regarding &ldquo;best practices&rdquo;, the different string types are used within this framework as follows (which you can follow in your own code, but of course you don't have to):
* **Platform-specific stuff**:
  &ldquo;native&rdquo; `std::string` and/or `std::wstring`
* **Internal processing**:
  whatever serves, preferably `cl7::u8string`, following &ldquo;UTF-8 Everywhere&rdquo;,
  even though the recommendations regarding UTF-32 also make a lot of sense
  (and even if `cl7::astring` would be &ldquo;enough&rdquo; in case of rather language-independent technical stuff)
* **Storage and transmission**:
  `cl7::u8string` (UTF-8)
* **User interaction**:
  `cl7::u32string` (UTF-32)

ASCII and UTF-32 have the great advantage that one character (code unit) corresponds to exactly one (representable) code point.
This makes string manipulations easy or even possible (let's ignore things like graphemes or normalization for now).
In addition, UTF-32 has the advantage that &ldquo;all&rdquo; symbols in the human world are supported (good for user interaction);
ASCII has very limited coverage but the advantage of being space-saving, simple, and &ldquo;close to code&rdquo; (good for language-independent technical stuff).
UTF-8 tends to being space-saving as well, but at the same time has the ability to &ldquo;somehow&rdquo; represent &ldquo;all&rdquo; characters (good for storage and transmission of language-dependent stuff and (other) special characters; as a superset of ASCII, it is also suitable for language-independent technical stuff as well).
UTF-16 is a kind of hybrid: it covers &ldquo;all&rdquo; characters, is okay in terms of memory requirements, and allows for some string manipulations because many or even most common code points can be represented with one single code unit (good/okay for almost everything, but loses to UTF-8 in storage and transmission).
However, UTF-16 is rarely used within this framework (tends to be used only in the case of platform-specific things).

Despite the presence of the eighth bit of `cl7::astring`, we deliberately do not support &ldquo;extended ASCII&rdquo;, ANSI, ISO 8859, etc., not even Latin-1, although it corresponds 1-to-1 to the first 256 Unicode characters.
If you need these encodings (for whatever reason, for example under Windows), you have to take care of them yourself.
At best, you avoid ANSI, ISO 8859, etc. as much as possible and stick to standard ASCII and Unicode, with Unicode being a superset of ASCII.
And then you might end up with UTF-8 as a &ldquo;technical&rdquo; superset of standard ASCII as well (or UTF-32 if you need the quasi-1-to-1 correspondence between code units and code points).

So you see: UTF-8 (almost) everywhere.
As mentioned before, you may have to take care of ISO 8859, ANSI, etc. yourself, but to avoid this as much as possible, we are forcing the `UNICODE`/`_UNICODE` definition, also as suggested by the &ldquo;UTF-8 Everywhere&rdquo; manifesto.

Within the given framework, we want to provide some basic functionality to deal with different encodings and Unicode transformation formats and to convert between them.
We do not want to reinvent the wheel or to push it further than necessary, so we really only limit ourselves to selected aspects that the framework uses internally itself, without having to include external libraries.
If you need more extensive or robust functionality, you could/should include and use 3rd-party libraries in your projects yourself, like [International Components for Unicode (ICU)](https://icu.unicode.org/) or [Boost.Locale](https://www.boost.org/doc/libs/release/libs/locale/) or maybe [ztd.text](https://github.com/soasis/text/), for example.

I'm already taking cover, but despite everything, don't be afraid to use different character encodings or string types within your software.
If you look at the different ways strings are used within a project, you will see that in most cases different types don't even touch each other.
But where there are points of contact, you should of course be as uniform as possible and conversions should be minimized.
Okay, at least when it comes to logging, all possible encodings potentially come into contact with each other.
Anyway, &ldquo;most&rdquo; important thing is that you always know which encoding a given string (or file, etc.) has.
