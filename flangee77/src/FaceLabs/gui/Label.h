#ifndef FL7_GUI_LABEL_H
#define FL7_GUI_LABEL_H
#include "./Static.h"

#include <CoreLabs/string.h>



namespace fl7::gui {



/**
 * A display-only piece of text, single-line or multi-line, laid out within this
 * face's bounds. No wrapping/alignment logic of its own, that's entirely up to the
 * renderer's text-drawing backend (see `AbstractRenderer::draw_text_in_box`).
 */
class Label
    : public Static
{

public:
    Label() = default;

    explicit Label(cl7::u8string text)
        : _text(std::move(text))
    {
    }

    Label(const Label&) = delete;
    Label& operator=(const Label&) = delete;
    Label(Label&&) = delete;
    Label& operator=(Label&&) = delete;

    ~Label() override = default;



    // #############################################################################
    // Properties
    // #############################################################################

    /** Returns this label's text. */
    const cl7::u8string& get_text() const { return _text; }

    /** Sets this label's text. */
    void set_text(cl7::u8string text) { _text = std::move(text); }



private:

    // #############################################################################
    // Attributes
    // #############################################################################

    cl7::u8string _text;

}; // class Label



} // namespace fl7::gui

#endif // FL7_GUI_LABEL_H
