#ifndef FL7_GUI_FACES_LABEL_H
#define FL7_GUI_FACES_LABEL_H
#include "../Static.h"
#include "../HasText.h"

#include <FaceLabs/fonts/Font.h>



namespace fl7::gui::faces {



/**
 * A display-only piece of text, single-line or multi-line, laid out within this
 * face's bounds. No wrapping/alignment logic of its own, that's entirely up to the
 * renderer's text-drawing backend (see `AbstractRenderer::draw_text_in_box`).
 */
class Label
    : public Static
    , public HasText
{

public:
    Label() = default;

    explicit Label(cl7::u32string text)
        : _text(std::move(text))
    {
    }

    Label(const Label&) = delete;
    Label& operator=(const Label&) = delete;
    Label(Label&&) = delete;
    Label& operator=(Label&&) = delete;

    ~Label() override = default;



    // #############################################################################
    // HasText Implementations
    // #############################################################################

    /** Returns this label's text to be displayed. */
    const cl7::u32string& get_display_text() const override { return _text; }



    // #############################################################################
    // Properties
    // #############################################################################

    /** Returns this label's text. */
    const cl7::u32string& get_text() const { return _text; }

    /** Sets this label's text. */
    void set_text(cl7::u32string text) { _text = std::move(text); }



protected:

    // #############################################################################
    // Face Implementations
    // #############################################################################

    /** Returns this label's theme role. */
    cl7::u8string_view _get_theme_key() const override { return u8"label"; }

    /**
     * Returns this label's text's own natural (unwrapped) size, under its
     * effective font/text style; see _calculate_text_size().
     */
    ml7::Vector2f _calculate_preferred_size() const override;

    /**
     * Returns this label's text's own natural (unwrapped) size, under its
     * effective font/text style; see _calculate_text_size().
     */
    ml7::Vector2f _calculate_minimum_size() const override;



private:

    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Returns this label's text's own natural (unwrapped) size: however wide its
     * single widest (explicit-break-delimited) line is, times however many such
     * lines there are, under its effective font/text style. {0, 0} if it currently
     * has no effective font (e.g., not yet part of a themed Shell). Cached, as
     * long as neither the text nor the effective font have changed since.
     */
    ml7::Vector2f _calculate_text_size() const;



    // #############################################################################
    // Attributes
    // #############################################################################

    cl7::u32string _text;

    /** The inputs/result of the last _calculate_text_size() call; see there. */
    mutable fonts::Font* _cached_font = nullptr;
    mutable cl7::u32string _cached_text;
    mutable ml7::Vector2f _cached_text_size;

}; // class Label



} // namespace fl7::gui::faces

#endif // FL7_GUI_FACES_LABEL_H
