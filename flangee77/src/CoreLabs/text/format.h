#ifndef CL7_TEXT_FORMAT_H
#define CL7_TEXT_FORMAT_H

#include "./codec.h"
#include "./inspect.h"
#include "./transform.h"

#include <CoreLabs/string.h>

#include <algorithm>
#include <charconv>
#include <cstring>



namespace cl7::text::format {



enum struct NumberBase : unsigned
{
    Decimal = 10,
    Hexadecimal = 16,
    Octal = 8,
    Binary = 2,
};



enum struct HexCase : unsigned
{
    Lower = 0x61,
    Upper = 0x41,
};



enum struct FloatFormat
{
    /** Allow fixed and scientific notations (depending on the value to be represented). */
    General,
    /** Force fixed notation. */
    Fixed,
    /** Force scientific notation. */
    Scientific,
};

constexpr unsigned MAX_FLOAT_PRECISION = 34;



struct IntFormatOptions
{
    /** If true, always adds a sign ('+' or '-') to the output, even for zero or positive numbers. */
    bool force_sign = false;
    /** If true, adds a prefix to the output: "0x" for hex, "0o" for octal, or "0b" for binary. */
    bool with_prefix = false;
    /** The minimum width of the output string. If shorter, pads with leading zeros. */
    unsigned zero_pad_width = 0;
    /** Controls the letter case for hexadecimal output (e.g., "A1B2" vs. "a1b2"). */
    HexCase hex_case = HexCase::Upper;
};

struct FloatFormatOptions
{
    /** If true, always adds a sign ('+' or '-') to the output, even for zero or positive numbers. */
    bool force_sign = false;
    /** Controls whether to use fixed, scientific, or automatic notation. */
    FloatFormat format = FloatFormat::General;
    /** The minimum number of fractional digits (pads with zeros if necessary). */
    unsigned min_precision = 0;
    /** The maximum number of fractional digits (truncates and rounds half-up if exceeded). */
    unsigned max_precision = MAX_FLOAT_PRECISION;
};



    /**
     * Converts an integer value to a (decimal) string representation.
     */
    template <any_string_or_view Tstring_or_view, std::integral Tval>
    Tstring_or_view to_string(Tval val, IntFormatOptions options);


    /**
     * Converts a string representation to an integer value.
     * This function can parse not only decimal strings, but also hexadecimal,
     * octal, and binary strings. However, these must be prefixed accordingly
     * ("0x", "0o", or "0b") to be recognized correctly.
     */
    template <std::integral Tval = signed, any_string_or_view Tstring_or_view>
    Tval parse_int(Tstring_or_view&& s);

    /**
     * Converts a hexadecimal string representation to an integer value.
     */
    template <std::integral Tval = unsigned, any_string_or_view Tstring_or_view>
    Tval parse_hex(Tstring_or_view&& s);

    /**
     * Converts an octal string representation to an integer value.
     */
    template <std::integral Tval = unsigned, any_string_or_view Tstring_or_view>
    Tval parse_octal(Tstring_or_view&& s);

    /**
     * Converts a binary string representation to an integer value.
     */
    template <std::integral Tval = unsigned, any_string_or_view Tstring_or_view>
    Tval parse_binary(Tstring_or_view&& s);



    /**
     * Adjusts a fixed-notation floating-point string to enforce minimum/maximum
     * fractional digits.
     */
    template <any_string_or_view Tstring_or_view_out, any_string_or_view Tstring_or_view_in>
        requires(std::same_as<typename std::remove_cvref_t<Tstring_or_view_out>::value_type, typename std::remove_cvref_t<Tstring_or_view_in>::value_type>)
    Tstring_or_view_out adjust_fixed_precision(Tstring_or_view_in&& s, unsigned min_precision, unsigned max_precision = MAX_FLOAT_PRECISION)
    {
        using Tchar = std::remove_cvref_t<Tstring_or_view_in>::value_type;
        using Tstring_view = std::basic_string_view<Tchar>;
        Tstring_view sv{std::forward<Tstring_or_view_in>(s)};

        // Fixed notation: sign (1) + digits (34) + decimal point (1) + digits (34) = 70 characters.
        // However, for a safe buffer size, 128 characters is often recommended
        // in practice to account for any formatting overhead or future-proofing.
        static constexpr size_t BUFFER_SIZE = 128;
        static Tchar buffer[BUFFER_SIZE];

        transform::trim_left(sv);

        if (sv.empty())
            return Tstring_or_view_out{};

        // Skip any sign character.
        size_t p = 0;
        if (sv[0] == Tchar{'-'} || sv[0] == Tchar{'+'})
            ++p;

        // If, apart from a possible sign, there is
        // only one character, it must be a digit.
        if (sv.size() == p + 1 && !inspect::is_digit(sv[p]))
            return Tstring_or_view_out{};

        // Apart from a possible sign, the actual number must
        // start with either a decimal point or a digit.
        if (sv.size() > p + 1 && sv[p] != Tchar{'.'} && !inspect::is_digit(sv[p]))
            return Tstring_or_view_out{};

        // If the first character after an optional sign is a decimal
        // point, then the following character must be a digit.
        if (sv.size() > p + 1 && sv[p] == Tchar{'.'} && !inspect::is_digit(sv[p + 1]))
            return Tstring_or_view_out{};

        // Locate the decimal point (or its theoretical position)
        // by skipping all digits after the optional sign.
        size_t dot_pos = p;
        while (dot_pos < sv.size() && inspect::is_digit(sv[dot_pos]))
            ++dot_pos;

        // Determine the total length of the (valid) number.
        // Iterate over fractional digits, if there is a decimal point.
        // Ignore possible decimal point if there are no fractional digits at all.
        size_t len = dot_pos;
        if (dot_pos + 1 < sv.size() && sv[dot_pos] == Tchar{'.'} && inspect::is_digit(sv[dot_pos + 1]))
            for (len += 2; len < sv.size() && inspect::is_digit(sv[len]); ++len)
                (void)0;

        assert(len <= BUFFER_SIZE);

        // Transfer the data to our own
        // buffer for further processing.
        std::memcpy(buffer, sv.data(), len);

        // No digits before decimal point?
        // Insert a leading zero.
        if (dot_pos == p)
        {
            std::memmove(buffer + p + 1, buffer + p, len - p);
            buffer[p] = Tchar{'0'};
            ++dot_pos;
            ++len;
        }

        // Determine the length of the fractional part.
        size_t frac_count = len > dot_pos ? len - dot_pos - 1 : 0;

        // Check the length of the fractional
        // part against the minimum precision.
        if (frac_count < static_cast<size_t>(min_precision))
        {
            // No fractional digits means there is no decimal point.
            // Therefore, we need to add one.
            if (frac_count == 0)
            {
                assert(len + 1 + min_precision <= BUFFER_SIZE);
                buffer[len++] = Tchar{'.'};
            }

            // Now pad with zeros.
            assert(len - frac_count + min_precision <= BUFFER_SIZE);
            while (frac_count++ < static_cast<size_t>(min_precision))
                buffer[len++] = Tchar{'0'};
        } // ensure minimum precision

        // "Early" out if the fractional part is
        // not longer than the maximum precision.
        if (frac_count <= static_cast<size_t>(max_precision))
            return Tstring_or_view_out(Tstring_view(buffer, len));

        // Determine new effective length after shortening.
        len = dot_pos + 1 + static_cast<size_t>(max_precision);

        // If we don't need to round half-up, we
        // just need to truncate, and we're done.
        if (buffer[len] < Tchar{'5'})
        {
            sv = Tstring_view(buffer, len);
            transform::trim_right(sv, Tchar{'.'}); // Trim trailing decimal point.
            return Tstring_or_view_out(sv);
        }

        for (size_t i = len - 1; true; --i)
        {
            // Digit less than 9? Then simply
            // increase by 1 and we're done.
            if (buffer[i] >= Tchar{'0'} && buffer[i] < Tchar{'9'})
            {
                ++buffer[i];
                break;
            }

            // Do we have a 9 here? Then we have a
            // carry-over, so we are not done yet.
            if (buffer[i] == Tchar{'9'})
                buffer[i] = Tchar{'0'};

            // We just skip/ignore the decimal point.

            // Have we reached the beginning of all the digits?
            // If so, we need to insert a 1 here to account for the last carry-over.
            // This will increase the length of the result by one. And that's it.
            if (i == p)
            {
                std::memmove(buffer + p + 1, buffer + p, len - p);
                buffer[p] = Tchar{'1'};
                //++dot_pos; // We no longer need to maintain this information.
                ++len;
                break;
            }
        } // while round half-up

        sv = Tstring_view(buffer, len);
        transform::trim_right(sv, Tchar{'.'}); // Trim trailing decimal point.
        return Tstring_or_view_out(sv);
    }



    /**
     * Converts a floating-point value to a (decimal) string representation
     * (in fixed or scientific notation).
     */
    template <any_string_or_view Tstring_or_view, std::floating_point Tval>
    Tstring_or_view to_string(Tval val, FloatFormatOptions options)
    {
        using Tchar = Tstring_or_view::value_type;
        using Tstring_view = std::basic_string_view<Tchar>;

        // Fixed notation: sign (1) + digits (34) + decimal point (1) + digits (34) = 70 characters.
        // Scientific notation: sign (1) + digit (1) + decimal point (1) + digits (34) + exponent (5) = 42 characters.
        // However, for a safe buffer size, 128 characters is often recommended
        // in practice to account for any formatting overhead or future-proofing.
        static constexpr size_t BUFFER_SIZE = 128;
        static char chars[BUFFER_SIZE];
        static Tchar buffer[BUFFER_SIZE];

        std::chars_format fmt;
        switch (options.format)
        {
        case FloatFormat::General:      fmt = std::chars_format::general;       break;
        case FloatFormat::Fixed:        fmt = std::chars_format::fixed;         break;
        case FloatFormat::Scientific:   fmt = std::chars_format::scientific;    break;
        }

        // The precision parameter of `std::to_chars_result` doesn't quite behave in
        // the way we understand precision here. In fixed/scientific modes, yes, it
        // does mean digits after the decimal point. But in general mode, it means
        // significant digits (total across integer plus fractional). So to control
        // "decimal places after the dot" uniformly across formats, we need to
        // post-process the string ourselves (see below).
        std::to_chars_result result = std::to_chars(&chars[0], &chars[BUFFER_SIZE], val, fmt);

        if (result.ec != std::errc{})
            return Tstring_or_view{};

        auto len = static_cast<size_t>(result.ptr - chars);

        // Transfer character data to our buffer.
        if constexpr (sizeof(Tchar) == sizeof(char))
            std::memcpy(buffer, chars, len * sizeof(char));
        else
            std::ranges::copy(chars, chars + len, buffer);

        // Always add a sign to the output?
        if (options.force_sign && buffer[0] != Tchar{'-'} && buffer[0] != Tchar{'+'})
        {
            // Make space for the sign, insert
            // it, and increase the length.
            std::memmove(buffer + 1, buffer, len);
            buffer[0] = Tchar{'+'};
            ++len;
        }

        auto sv = Tstring_view(buffer, len);

        // Determine the length of the fixed notation part.
        auto fix_len = std::min(len, sv.find(Tchar{'e'}));

        // Let the "helper" function perform any precision adjustments.
        auto sv_tmp = adjust_fixed_precision<Tstring_view>(Tstring_view(buffer, fix_len), options.min_precision, options.max_precision);

        // Shift the scientific notation part (if present)
        // and copy the adjusted fixed notation part back.
        std::memmove(buffer + sv_tmp.size(), buffer + fix_len, len - fix_len);
        std::memcpy(buffer, sv_tmp.data(), sv_tmp.size());

        // Adjust our new length(s) accordingly.
        len -= fix_len;
        fix_len = sv_tmp.size();
        len += fix_len;

        // There is our precision-adjusted string.
        sv = Tstring_view(buffer, len);

        // Now we only have one last (potential) problem: if an overflow occurs,
        // resulting in a "10" at the beginning of the number in scientific notation.
        // In that case, we need to change it to "1" and increment the exponent by 1.

        // Fortunately, in this case, all the other digits are 0 anyway,
        // so we don't need to worry about any further precision issues
        // or other things related to the remaining digits.

        // Fixed notation? Then the problem is not relevant.
        if (len == fix_len)
            return Tstring_or_view(sv);

        // Identify the digit(s) located between the
        // possible sign and a possible decimal point.
        size_t p = buffer[0] == Tchar{'-'} || buffer[0] == Tchar{'+'} ? 1 : 0;
        size_t dot_pos = std::min(fix_len, sv.find(Tchar{'.'}));

        // One single digit is absolutely perfect.
        if (dot_pos - p == 1)
            return Tstring_or_view(sv);

        // So, at this point we are dealing with
        // said 10 in scientific notation format.
        assert(dot_pos - p == 2);
        assert(buffer[p + 0] == Tchar{'1'});
        assert(buffer[p + 1] == Tchar{'0'});
        assert(len - fix_len >= 4); // e.g., "e+02"

        // Turn the 10 into a 1 by removing the 0.
        assert(len > p + 2);
        if (len > p + 2) // always true, but avoids underflow suspicion
        {
            std::memmove(buffer + p + 1, buffer + p + 2, len - (p + 2));
            --fix_len;
            --len;
        }

        // Update.
        sv = Tstring_view(buffer, len);

        // Now parse the exponent, increase it by 1,
        // and write it back to our buffer.
        auto exp = parse_int<signed>(sv.substr(fix_len + 1));
        ++exp;
        sv_tmp = to_string<Tstring_view>(exp, {.force_sign = true, .zero_pad_width = 2});
        std::memcpy(buffer + fix_len + 1, sv_tmp.data(), sv_tmp.size());

        // Adjust our new length accordingly.
        len = fix_len + 1 + sv_tmp.size();

        // We're finally there!
        sv = Tstring_view(buffer, len);

        return Tstring_or_view(sv);
    }



    /**
     * Converts an integer value to a (decimal) string representation.
     */
    template <any_string_or_view Tstring_or_view, std::integral Tval>
    Tstring_or_view to_string(Tval val, IntFormatOptions options)
    {
        using Tchar = Tstring_or_view::value_type;
        using Tstring_view = std::basic_string_view<Tchar>;

        // Optional sign (1), up to 64-bit decimal number (20) = 21 characters.
        static constexpr size_t BUFFER_SIZE = 1 + 20;
        static Tchar buffer[BUFFER_SIZE];

        assert(options.zero_pad_width <= 20);
        options.zero_pad_width = std::min(options.zero_pad_width, 20u);

        size_t len = 0;
        uint64_t v;

        if constexpr (std::is_unsigned_v<Tval>)
            v = static_cast<uint64_t>(val);
        else if (val >= 0)
            v = static_cast<uint64_t>(val);
        else
        {
            v = static_cast<uint64_t>(-val);
            buffer[len++] = Tchar{'-'};
        }

        if (options.force_sign && len == 0)
            buffer[len++] = Tchar{'+'};

        // No prefix to consider.

        const auto p = len;

        do
        {
            auto x = static_cast<Tchar>(v % 10);
            v /= 10;
            Tchar c = Tchar{'0'} + x;
            buffer[len++] = c;
        } while (v);

        while (len - p < options.zero_pad_width)
            buffer[len++] = Tchar{'0'};

        std::reverse(&buffer[p], &buffer[len]);

        return Tstring_or_view(Tstring_view(buffer, len));
    }

    /**
     * Converts an integer value to a hexadecimal string representation.
     */
    template <any_string_or_view Tstring_or_view, std::integral Tval>
    Tstring_or_view to_hex(Tval val, IntFormatOptions options)
    {
        using Tchar = Tstring_or_view::value_type;
        using Tstring_view = std::basic_string_view<Tchar>;

        // Optional sign (1), possible "0x" prefix (2), up to 64-bit hexadecimal number (16) = 19 characters.
        static constexpr size_t BUFFER_SIZE = 1 + 2 + 16;
        static Tchar buffer[BUFFER_SIZE];

        assert(options.zero_pad_width <= 16);
        options.zero_pad_width = std::min(options.zero_pad_width, 16u);

        size_t len = 0;
        uint64_t v;

        if constexpr (std::is_unsigned_v<Tval>)
            v = static_cast<uint64_t>(val);
        else if (val >= 0)
            v = static_cast<uint64_t>(val);
        else
        {
            v = static_cast<uint64_t>(-val);
            buffer[len++] = Tchar{'-'};
        }

        if (options.force_sign && len == 0)
            buffer[len++] = Tchar{'+'};

        if (options.with_prefix)
        {
            buffer[len++] = Tchar{'0'};
            buffer[len++] = Tchar{'x'};
        }

        const auto p = len;

        do
        {
            auto x = static_cast<Tchar>(v & 0xf); // v % 16
            v >>= 4; // v /= 16
            Tchar c = x < 0xa ? Tchar{'0'} + x : static_cast<Tchar>(options.hex_case) + x - 0xa;
            buffer[len++] = c;
        } while (v);

        while (len - p < options.zero_pad_width)
            buffer[len++] = Tchar{'0'};

        std::reverse(&buffer[p], &buffer[len]);

        return Tstring_or_view(Tstring_view(buffer, len));
    }

    /**
     * Converts an integer value to an octal string representation.
     */
    template <any_string_or_view Tstring_or_view, std::integral Tval>
    Tstring_or_view to_octal(Tval val, IntFormatOptions options)
    {
        using Tchar = Tstring_or_view::value_type;
        using Tstring_view = std::basic_string_view<Tchar>;

        // Optional sign (1), possible "0o" prefix (2), up to 64-bit octal number (22) = 25 characters.
        static constexpr size_t BUFFER_SIZE = 1 + 2 + 22;
        static Tchar buffer[BUFFER_SIZE];

        assert(options.zero_pad_width <= 22);
        options.zero_pad_width = std::min(options.zero_pad_width, 22u);

        size_t len = 0;
        uint64_t v;

        if constexpr (std::is_unsigned_v<Tval>)
            v = static_cast<uint64_t>(val);
        else if (val >= 0)
            v = static_cast<uint64_t>(val);
        else
        {
            v = static_cast<uint64_t>(-val);
            buffer[len++] = Tchar{'-'};
        }

        if (options.force_sign && len == 0)
            buffer[len++] = Tchar{'+'};

        if (options.with_prefix)
        {
            buffer[len++] = Tchar{'0'};
            buffer[len++] = Tchar{'o'};
        }

        const auto p = len;

        do
        {
            auto x = static_cast<Tchar>(v & 0x7); // v % 8
            v >>= 3; // v /= 8
            Tchar c = Tchar{'0'} + x;
            buffer[len++] = c;
        } while (v);

        while (len - p < options.zero_pad_width)
            buffer[len++] = Tchar{'0'};

        std::reverse(&buffer[p], &buffer[len]);

        return Tstring_or_view(Tstring_view(buffer, len));
    }

    /**
     * Converts an integer value to a binary string representation.
     */
    template <any_string_or_view Tstring_or_view, std::integral Tval>
    Tstring_or_view to_binary(Tval val, IntFormatOptions options)
    {
        using Tchar = Tstring_or_view::value_type;
        using Tstring_view = std::basic_string_view<Tchar>;

        // Optional sign (1), possible "0b" prefix (2), up to 64-bit octal number (64) = 67 characters.
        static constexpr size_t BUFFER_SIZE = 1 + 2 + 64;
        static Tchar buffer[BUFFER_SIZE];

        assert(options.zero_pad_width <= 64);
        options.zero_pad_width = std::min(options.zero_pad_width, 64u);

        size_t len = 0;
        uint64_t v;

        if constexpr (std::is_unsigned_v<Tval>)
            v = static_cast<uint64_t>(val);
        else if (val >= 0)
            v = static_cast<uint64_t>(val);
        else
        {
            v = static_cast<uint64_t>(-val);
            buffer[len++] = Tchar{'-'};
        }

        if (options.force_sign && len == 0)
            buffer[len++] = Tchar{'+'};

        if (options.with_prefix)
        {
            buffer[len++] = Tchar{'0'};
            buffer[len++] = Tchar{'b'};
        }

        const auto p = len;

        do
        {
            auto x = static_cast<Tchar>(v & 0x1); // v % 2
            v >>= 1; // v /= 2
            Tchar c = Tchar{'0'} + x;
            buffer[len++] = c;
        } while (v);

        while (len - p < options.zero_pad_width)
            buffer[len++] = Tchar{'0'};

        std::reverse(&buffer[p], &buffer[len]);

        return Tstring_or_view(Tstring_view(buffer, len));
    }



    /**
     * Converts a floating-point value to a (decimal) string representation
     * (in fixed or scientific notation).
     *
     * You can specify the minimum and maximum number of fractional digits.
     */
    template <any_string_or_view Tstring_or_view, std::floating_point Tval>
    Tstring_or_view to_string(Tval val, unsigned min_precision = 0, unsigned max_precision = MAX_FLOAT_PRECISION)
    {
        return to_string<Tstring_or_view>(val, {.format = FloatFormat::General, .min_precision = min_precision, .max_precision = max_precision});
    }

    /**
     * Converts a floating-point value to a (decimal) string representation
     * (in fixed notation).
     *
     * You can specify the minimum and maximum number of fractional digits.
     */
    template <any_string_or_view Tstring_or_view, std::floating_point Tval>
    Tstring_or_view to_fixed(Tval val, unsigned min_precision = 0, unsigned max_precision = MAX_FLOAT_PRECISION)
    {
        return to_string<Tstring_or_view>(val, {.format = FloatFormat::Fixed, .min_precision = min_precision, .max_precision = max_precision});
    }

    /**
     * Converts a floating-point value to a (decimal) string representation
     * (in scientific notation).
     *
     * You can specify the minimum and maximum number of fractional digits.
     */
    template <any_string_or_view Tstring_or_view, std::floating_point Tval>
    Tstring_or_view to_scientific(Tval val, unsigned min_precision = 0, unsigned max_precision = MAX_FLOAT_PRECISION)
    {
        return to_string<Tstring_or_view>(val, {.format = FloatFormat::Scientific, .min_precision = min_precision, .max_precision = max_precision});
    }


    /**
     * Converts an integer value to a (decimal) string representation.
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     */
    template <any_string_or_view Tstring_or_view, std::integral Tval>
    Tstring_or_view to_string(Tval val, unsigned zero_pad_width = 0)
    {
        return to_string<Tstring_or_view>(val, {.zero_pad_width = zero_pad_width});
    }


    /**
     * Converts an integer value to a hexadecimal string representation.
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     *
     * You can also control the hexadecimal letter case (e.g., "A1B2" vs. "a1b2").
     */
    template <any_string_or_view Tstring_or_view, std::integral Tval>
    Tstring_or_view to_hex(Tval val, unsigned zero_pad_width = 0, HexCase hex_case = HexCase::Upper)
    {
        return to_hex<Tstring_or_view>(val, {.with_prefix = false, .zero_pad_width = zero_pad_width, .hex_case = hex_case});
    }

    /**
     * Converts an integer value to a hexadecimal string representation, prefixed
     * with "0x".
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     *
     * You can also control the hexadecimal letter case (e.g., "A1B2" vs. "a1b2").
     */
    template <any_string_or_view Tstring_or_view, std::integral Tval>
    Tstring_or_view to_0x_hex(Tval val, unsigned zero_pad_width = 0, HexCase hex_case = HexCase::Upper)
    {
        return to_hex<Tstring_or_view>(val, {.with_prefix = true, .zero_pad_width = zero_pad_width, .hex_case = hex_case});
    }


    /**
     * Converts an integer value to a lowercase hexadecimal string representation.
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     */
    template <any_string_or_view Tstring_or_view, std::integral Tval>
    Tstring_or_view to_hex_lc(Tval val, unsigned zero_pad_width = 0)
    {
        return to_hex<Tstring_or_view>(val, {.with_prefix = false, .zero_pad_width = zero_pad_width, .hex_case = HexCase::Lower});
    }

    /**
     * Converts an integer value to an uppercase hexadecimal string representation.
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     */
    template <any_string_or_view Tstring_or_view, std::integral Tval>
    Tstring_or_view to_hex_uc(Tval val, unsigned zero_pad_width = 0)
    {
        return to_hex<Tstring_or_view>(val, {.with_prefix = false, .zero_pad_width = zero_pad_width, .hex_case = HexCase::Upper});
    }

    /**
     * Converts an integer value to a lowercase hexadecimal string representation,
     * prefixed with "0x".
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     */
    template <any_string_or_view Tstring_or_view, std::integral Tval>
    Tstring_or_view to_0x_hex_lc(Tval val, unsigned zero_pad_width = 0)
    {
        return to_hex<Tstring_or_view>(val, {.with_prefix = true, .zero_pad_width = zero_pad_width, .hex_case = HexCase::Lower});
    }

    /**
     * Converts an integer value to an uppercase hexadecimal string representation,
     * prefixed with "0x".
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     */
    template <any_string_or_view Tstring_or_view, std::integral Tval>
    Tstring_or_view to_0x_hex_uc(Tval val, unsigned zero_pad_width = 0)
    {
        return to_hex<Tstring_or_view>(val, {.with_prefix = true, .zero_pad_width = zero_pad_width, .hex_case = HexCase::Upper});
    }


    /**
     * Converts an integer value to an octal string representation.
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     */
    template <any_string_or_view Tstring_or_view, std::integral Tval>
    Tstring_or_view to_octal(Tval val, unsigned zero_pad_width = 0)
    {
        return to_octal<Tstring_or_view>(val, {.with_prefix = false, .zero_pad_width = zero_pad_width});
    }

    /**
     * Converts an integer value to an octal string representation, prefixed with
     * "0o".
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     */
    template <any_string_or_view Tstring_or_view, std::integral Tval>
    Tstring_or_view to_0o_octal(Tval val, unsigned zero_pad_width = 0)
    {
        return to_octal<Tstring_or_view>(val, {.with_prefix = true, .zero_pad_width = zero_pad_width});
    }


    /**
     * Converts an integer value to a binary string representation.
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     */
    template <any_string_or_view Tstring_or_view, std::integral Tval>
    Tstring_or_view to_binary(Tval val, unsigned zero_pad_width = 0)
    {
        return to_binary<Tstring_or_view>(val, {.with_prefix = false, .zero_pad_width = zero_pad_width});
    }

    /**
     * Converts an integer value to a binary string representation, prefixed with
     * "0b".
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     */
    template <any_string_or_view Tstring_or_view, std::integral Tval>
    Tstring_or_view to_0b_binary(Tval val, unsigned zero_pad_width = 0)
    {
        return to_binary<Tstring_or_view>(val, {.with_prefix = true, .zero_pad_width = zero_pad_width});
    }


    /**
     * Adjusts a UTF-8 encoded fixed-notation floating-point string to enforce
     * minimum/maximum fractional digits.
     */
    inline
    u8string_view adjust_fixed_precision(u8string_view s, unsigned min_precision, unsigned max_precision = MAX_FLOAT_PRECISION) { return adjust_fixed_precision<u8string_view>(s, min_precision, max_precision); }


    /**
     * Converts a floating-point value to a UTF-8 encoded (decimal) string
     * representation (in fixed or scientific notation).
     */
    template <std::floating_point Tval>
    u8string_view to_string(Tval val, FloatFormatOptions options) { return to_string<u8string_view>(val, options); }

    /**
     * Converts a floating-point value to a UTF-8 encoded (decimal) string
     * representation (in fixed or scientific notation).
     *
     * You can specify the minimum and maximum number of fractional digits.
     */
    template <std::floating_point Tval>
    u8string_view to_string(Tval val, unsigned min_precision = 0, unsigned max_precision = MAX_FLOAT_PRECISION) { return to_string<u8string_view>(val, min_precision, max_precision); }

    /**
     * Converts a floating-point value to a UTF-8 encoded (decimal) string
     * representation (in fixed notation).
     *
     * You can specify the minimum and maximum number of fractional digits.
     */
    template <std::floating_point Tval>
    u8string_view to_fixed(Tval val, unsigned min_precision = 0, unsigned max_precision = MAX_FLOAT_PRECISION) { return to_fixed<u8string_view>(val, min_precision, max_precision); }

    /**
     * Converts a floating-point value to a UTF-8 encoded (decimal) string
     * representation (in scientific notation).
     *
     * You can specify the minimum and maximum number of fractional digits.
     */
    template <std::floating_point Tval>
    u8string_view to_scientific(Tval val, unsigned min_precision = 0, unsigned max_precision = MAX_FLOAT_PRECISION) { return to_scientific<u8string_view>(val, min_precision, max_precision); }


    /**
     * Converts an integer value to a UTF-8 encoded (decimal) string representation.
     */
    template <std::integral Tval>
    u8string_view to_string(Tval val, IntFormatOptions options) { return to_string<u8string_view>(val, options); }

    /**
     * Converts an integer value to a UTF-8 encoded (decimal) string representation.
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     */
    template <std::integral Tval>
    u8string_view to_string(Tval val, unsigned zero_pad_width = 0) { return to_string<u8string_view>(val, zero_pad_width); }


    /**
     * Converts an integer value to a UTF-8 encoded hexadecimal string
     * representation.
     */
    template <std::integral Tval>
    u8string_view to_hex(Tval val, IntFormatOptions options) { return to_hex<u8string_view>(val, options); }

    /**
     * Converts an integer value to a UTF-8 encoded hexadecimal string
     * representation.
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     *
     * You can also control the hexadecimal letter case (e.g., "A1B2" vs. "a1b2").
     */
    template <std::integral Tval>
    u8string_view to_hex(Tval val, unsigned zero_pad_width = 0, HexCase hex_case = HexCase::Upper) { return to_hex<u8string_view>(val, zero_pad_width, hex_case); }

    /**
     * Converts an integer value to a UTF-8 encoded hexadecimal string
     * representation, prefixed with "0x".
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     *
     * You can also control the hexadecimal letter case (e.g., "A1B2" vs. "a1b2").
     */
    template <std::integral Tval>
    u8string_view to_0x_hex(Tval val, unsigned zero_pad_width = 0, HexCase hex_case = HexCase::Upper) { return to_0x_hex<u8string_view>(val, zero_pad_width, hex_case); }


    /**
     * Converts an integer value to a UTF-8 encoded lowercase hexadecimal string
     * representation.
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     */
    template <std::integral Tval>
    u8string_view to_hex_lc(Tval val, unsigned zero_pad_width = 0) { return to_hex(val, zero_pad_width, HexCase::Lower); }

    /**
     * Converts an integer value to a UTF-8 encoded uppercase hexadecimal string
     * representation.
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     */
    template <std::integral Tval>
    u8string_view to_hex_uc(Tval val, unsigned zero_pad_width = 0) { return to_hex(val, zero_pad_width, HexCase::Upper); }

    /**
     * Converts an integer value to a UTF-8 encoded lowercase hexadecimal string
     * representation, prefixed with "0x".
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     */
    template <std::integral Tval>
    u8string_view to_0x_hex_lc(Tval val, unsigned zero_pad_width = 0) { return to_0x_hex(val, zero_pad_width, HexCase::Lower); }

    /**
     * Converts an integer value to a UTF-8 encoded uppercase hexadecimal string
     * representation, prefixed with "0x".
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     */
    template <std::integral Tval>
    u8string_view to_0x_hex_uc(Tval val, unsigned zero_pad_width = 0) { return to_0x_hex(val, zero_pad_width, HexCase::Upper); }


    /**
     * Converts an integer value to a UTF-8 encoded octal string representation.
     */
    template <std::integral Tval>
    u8string_view to_octal(Tval val, IntFormatOptions options) { return to_octal<u8string_view>(val, options); }

    /**
     * Converts an integer value to a UTF-8 encoded octal string representation.
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     */
    template <std::integral Tval>
    u8string_view to_octal(Tval val, unsigned zero_pad_width = 0) { return to_octal<u8string_view>(val, zero_pad_width); }

    /**
     * Converts an integer value to a UTF-8 encoded octal string representation,
     * prefixed with "0o".
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     */
    template <std::integral Tval>
    u8string_view to_0o_octal(Tval val, unsigned zero_pad_width = 0) { return to_0o_octal<u8string_view>(val, zero_pad_width); }


    /**
     * Converts an integer value to a UTF-8 encoded binary string representation.
     */
    template <std::integral Tval>
    u8string_view to_binary(Tval val, IntFormatOptions options) { return to_binary<u8string_view>(val, options); }

    /**
     * Converts an integer value to a UTF-8 encoded binary string representation.
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     */
    template <std::integral Tval>
    u8string_view to_binary(Tval val, unsigned zero_pad_width = 0) { return to_binary<u8string_view>(val, zero_pad_width); }

    /**
     * Converts an integer value to a UTF-8 encoded binary string representation,
     * prefixed with "0b".
     *
     * You can specify the minimum width of the output string. If shorter, leading
     * zeros will be added.
     */
    template <std::integral Tval>
    u8string_view to_0b_binary(Tval val, unsigned zero_pad_width = 0) { return to_0b_binary<u8string_view>(val, zero_pad_width); }



    /**
     * Converts a (decimal) string representation (in fixed or scientific notation)
     * to a floating-point value.
     */
    template <std::floating_point Tval = float, any_string_or_view Tstring_or_view>
    Tval parse_float(Tstring_or_view&& s)
    {
        using Tchar = std::remove_cvref_t<Tstring_or_view>::value_type;
        using Tstring_view = std::basic_string_view<Tchar>;
        Tstring_view sv{std::forward<Tstring_or_view>(s)};

        transform::trim(sv);

        if (!sv.empty() && sv[0] == Tchar{'+'})
            sv.remove_prefix(1);

        Tval val = {};

        if constexpr (std::is_same_v<Tchar, char>)
        {
            auto sv_ = sv;
            std::from_chars(sv_.data(), sv_.data() + sv_.size(), val);
        }
        else if constexpr (std::is_same_v<Tchar, char8_t>)
        {
            auto sv_ = codec::reinterpret_utf8(sv);
            std::from_chars(sv_.data(), sv_.data() + sv_.size(), val);
        }
        else
        {
            auto s_ = codec::to_utf8(sv);
            auto sv_ = codec::reinterpret_utf8(make_string_view(s_));
            std::from_chars(sv_.data(), sv_.data() + sv_.size(), val);
        }

        return val;
    }

    /**
     * Converts a string representation to an integer value.
     * This function can parse not only decimal strings, but also hexadecimal,
     * octal, and binary strings. However, these must be prefixed accordingly
     * ("0x", "0o", or "0b") to be recognized correctly.
     */
    template <std::integral Tval, any_string_or_view Tstring_or_view>
    Tval parse_int(Tstring_or_view&& s)
    {
        using Tchar = std::remove_cvref_t<Tstring_or_view>::value_type;
        using Tstring_view = std::basic_string_view<Tchar>;
        Tstring_view sv{std::forward<Tstring_or_view>(s)};

        transform::trim(sv);

        size_t p = 0;
        bool sign = false;

        if (p < sv.size() && (sv[p] == Tchar{'-'} || sv[p] == Tchar{'+'}))
            sign = sv[p++] == Tchar{'-'};

        if (sv.size() > p + 2 && sv[p] == Tchar{'0'})
        {
            if (sv[p + 1] == Tchar{'x'}) return parse_hex<Tval>(sv);
            if (sv[p + 1] == Tchar{'o'}) return parse_octal<Tval>(sv);
            if (sv[p + 1] == Tchar{'b'}) return parse_binary<Tval>(sv);
        }

        Tval val = 0;

        for (; p < sv.size() && inspect::is_digit(sv[p]); ++p)
        {
            val *= 10;
            val += static_cast<Tval>(sv[p]) - 0x30; // '0' = 48 = 0x30
        }

        return sign ? -val : val;
    }

    /**
     * Converts a hexadecimal string representation to an integer value.
     */
    template <std::integral Tval, any_string_or_view Tstring_or_view>
    Tval parse_hex(Tstring_or_view&& s)
    {
        using Tchar = std::remove_cvref_t<Tstring_or_view>::value_type;
        using Tstring_view = std::basic_string_view<Tchar>;
        Tstring_view sv{std::forward<Tstring_or_view>(s)};

        transform::trim(sv);

        size_t p = 0;
        bool sign = false;

        if (p < sv.size() && (sv[p] == Tchar{'-'} || sv[p] == Tchar{'+'}))
            sign = sv[p++] == Tchar{'-'};

        if (p + 1 < sv.size() && sv[p] == Tchar{'0'} && (sv[p + 1] == Tchar{'x'} || sv[p + 1] == Tchar{'X'}))
            p += 2;

        Tval val = 0;

        for (; p < sv.size() && inspect::is_hex_digit(sv[p]); ++p)
        {
            val *= 0x10;
            val += inspect::is_digit(sv[p])
                ? static_cast<Tval>(sv[p]) - 0x30 // '0' = 48 = 0x30
                : (static_cast<Tval>(sv[p]) | 0x20) - 0x57; // 'a' = 97 = 0x61; set off 10: 87 = 0x57
        }

        return sign ? -val : val;
    }

    /**
     * Converts an octal string representation to an integer value.
     */
    template <std::integral Tval, any_string_or_view Tstring_or_view>
    Tval parse_octal(Tstring_or_view&& s)
    {
        using Tchar = std::remove_cvref_t<Tstring_or_view>::value_type;
        using Tstring_view = std::basic_string_view<Tchar>;
        Tstring_view sv{std::forward<Tstring_or_view>(s)};

        transform::trim(sv);

        size_t p = 0;
        bool sign = false;

        if (p < sv.size() && (sv[p] == Tchar{'-'} || sv[p] == Tchar{'+'}))
            sign = sv[p++] == Tchar{'-'};

        if (p + 1 < sv.size() && sv[p] == Tchar{'0'} && (sv[p + 1] == Tchar{'o'} || sv[p + 1] == Tchar{'O'}))
            p += 2;

        Tval val = 0;

        for (; p < sv.size() && sv[p] >= Tchar{'0'} && sv[p] <= Tchar{'7'}; ++p)
        {
            val *= 010;
            val += static_cast<Tval>(sv[p]) - 0x30; // '0' = 48 = 0x30
        }

        return sign ? -val : val;
    }

    /**
     * Converts a binary string representation to an integer value.
     */
    template <std::integral Tval, any_string_or_view Tstring_or_view>
    Tval parse_binary(Tstring_or_view&& s)
    {
        using Tchar = std::remove_cvref_t<Tstring_or_view>::value_type;
        using Tstring_view = std::basic_string_view<Tchar>;
        Tstring_view sv{std::forward<Tstring_or_view>(s)};

        transform::trim(sv);

        size_t p = 0;
        bool sign = false;

        if (p < sv.size() && (sv[p] == Tchar{'-'} || sv[p] == Tchar{'+'}))
            sign = sv[p++] == Tchar{'-'};

        if (p + 1 < sv.size() && sv[p] == Tchar{'0'} && (sv[p + 1] == Tchar{'b'} || sv[p + 1] == Tchar{'B'}))
            p += 2;

        Tval val = 0;

        for (; p < sv.size() && sv[p] >= Tchar{'0'} && sv[p] <= Tchar{'1'}; ++p)
        {
            val <<= 1; // val *= 0b10
            val += static_cast<Tval>(sv[p]) - 0x30; // '0' = 48 = 0x30
        }

        return sign ? -val : val;
    }



} // namespace cl7::text::format

#endif // CL7_TEXT_FORMAT_H
