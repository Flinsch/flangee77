#ifndef CL7_STRINGS_CODEC_H
#define CL7_STRINGS_CODEC_H

#include "./codepoint.h"

#include "CoreLabs/string.h"



namespace cl7::strings {



enum class CodecState
{
    /** Initial "undefined" state. */
    Uninitialized,
    /** We have a valid Unicode code point and its character encoding at hand. */
    Valid,
    /** We have a complete character encoding, but it is inconsistent and/or does not correspond to a valid Unicode code point. */
    Invalid,
    /** The character encoding processed so far is not yet complete and therefore does not correspond to a valid Unicode code point. */
    Incomplete,
};

struct CodecResult
{
    /** The (current) Unicode code point, which may be 0 and/or incomplete, depending on the current state. */
    codepoint codepoint = {0};
    /** The current state. */
    CodecState state = CodecState::Uninitialized;
};



template <typename Tchar, class TCodecImpl>
    requires requires (TCodecImpl& impl) {
        { impl.init(codepoint{}) } -> std::convertible_to<bool>;
        { impl.input(Tchar{}, CodecResult{}) } -> std::same_as<CodecResult>;
        { impl.get_encoded() } -> std::convertible_to<std::basic_string_view<Tchar>>;
    } && std::is_trivially_copy_assignable_v<TCodecImpl>
class Codec
{

public:
    /**
     * Initializes the current state with the specified Unicode code point (and
     * whether it is valid) and stores its corresponding character encoding.
     */
    void init(codepoint codepoint)
    {
        if (codepoint.value == 0)
        {
            _reset();
            return;
        }

        _result.codepoint = codepoint;
        _result.state = _impl.init(codepoint) ? CodecState::Valid : CodecState::Invalid;
    }

    /**
     * Processes the specified code unit and updates the current state accordingly.
     * After that we have either a valid, invalid, or incomplete Unicode code point.
     */
    void input(Tchar xc)
    {
        if (xc == Tchar{0})
        {
            _reset();
            return;
        }

        if (is_complete())
            _reset();
        _result = _impl.input(xc, _result);
        assert(_result.state != CodecState::Uninitialized);
    }

    /** Returns the (current) Unicode code point, which may be 0 and/or incomplete, depending on the current state. */
    codepoint get_codepoint() const { return _result.codepoint; }
    /** Returns the current state. */
    CodecState get_state() const { return _result.state; }

    /** Returns true if the current state is uninitialized. */
    bool is_uninitialized() const { return _result.state == CodecState::Uninitialized; }
    /** Returns true if the current state is initialized (and thus valid, invalid, or incomplete). */
    bool is_initialized() const { return _result.state != CodecState::Uninitialized; }
    /** Returns true if the current state is valid: we have a valid Unicode code point and its character encoding at hand. */
    bool is_valid() const { return _result.state == CodecState::Valid; }
    /** Returns true if the current state is invalid: we have a complete character encoding, but it is inconsistent and/or does not correspond to a valid Unicode code point. */
    bool is_invalid() const { return _result.state == CodecState::Invalid; }
    /** Returns true if the current state is incomplete: the character encoding processed so far is not yet complete and therefore does not correspond to a valid Unicode code point. */
    bool is_incomplete() const { return _result.state == CodecState::Incomplete; }
    /** Returns true if the current state is complete, be it valid or invalid. */
    bool is_complete() const { return is_valid() || is_invalid(); }

    /** Returns the (current) character encoding. */
    std::basic_string_view<Tchar> get_encoded() const { return is_uninitialized() ? std::basic_string_view<Tchar>{} : _impl.get_encoded(); }

private:
    void _reset()
    {
        _impl = {};
        _result = {};
    }

    TCodecImpl _impl{};

    CodecResult _result{};

}; // class Codec



} // namespace cl7::strings

#endif // CL7_STRINGS_CODEC_H
