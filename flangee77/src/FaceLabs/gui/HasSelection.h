#ifndef FL7_GUI_HASSELECTION_H
#define FL7_GUI_HASSELECTION_H

#include <CoreLabs/root.h>



namespace fl7::gui {



/**
 * Interface: opts a face into having a text selection highlight drawn by the
 * renderer, as a code point range into its own text (see HasText, expected on the
 * same face; the highlight's pixel extent is measured against that text). The
 * renderer only actually draws it while has_selection() says there is one (see
 * TextField).
 */
class HasSelection
{

public:
    virtual ~HasSelection() = default;

    /**
     * Returns whether there's currently a non-empty selection to draw.
     */
    virtual bool has_selection() const = 0;

    /**
     * Returns the code point index (into the face's HasText text) the
     * selection starts at. Only meaningful while has_selection() is true.
     */
    virtual size_t get_selection_begin_codepoint_index() const = 0;

    /**
     * Returns the code point index (into the face's HasText text) the
     * selection ends at (exclusive). Only meaningful while has_selection() is
     * true.
     */
    virtual size_t get_selection_end_codepoint_index() const = 0;

}; // class HasSelection



} // namespace fl7::gui

#endif // FL7_GUI_HASSELECTION_H
