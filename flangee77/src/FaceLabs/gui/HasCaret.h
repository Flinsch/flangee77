#ifndef FL7_GUI_HASCARET_H
#define FL7_GUI_HASCARET_H

#include <CoreLabs/root.h>



namespace fl7::gui {



/**
 * Interface: opts a face into having a text-insertion caret drawn by the renderer,
 * at a given code point index into its own text (see HasText, expected on the same
 * face, the caret's pixel position is measured against that text). The renderer
 * only actually draws it while is_caret_visible() says so (e.g., only while
 * focused; see TextField).
 */
class HasCaret
{

public:
    virtual ~HasCaret() = default;

    /**
     * Returns whether the caret is currently visible.
     */
    virtual bool is_caret_visible() const = 0;

    /**
     * Returns the code point index (into the face's HasText text) the caret is
     * currently positioned at, in [0, text.size()].
     */
    virtual size_t get_caret_codepoint_index() const = 0;

}; // class HasCaret



} // namespace fl7::gui

#endif // FL7_GUI_HASCARET_H
