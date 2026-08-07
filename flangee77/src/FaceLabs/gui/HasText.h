#ifndef FL7_GUI_HASTEXT_H
#define FL7_GUI_HASTEXT_H

#include <CoreLabs/string.h>



namespace fl7::gui {



/**
 * Interface: opts a face into having a piece of text drawn by the renderer.
 */
class HasText
{

public:
    virtual ~HasText() = default;

    /**
     * Returns the text to display.
     */
    virtual const cl7::u32string& get_display_text() const = 0;

}; // class HasText



} // namespace fl7::gui

#endif // FL7_GUI_HASTEXT_H
