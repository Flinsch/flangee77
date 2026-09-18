#ifndef XL7_CLIPBOARD_H
#define XL7_CLIPBOARD_H
#include "./Component.h"

#include "./detail/IClipboardImpl.h"

#include <CoreLabs/string.h>

#include <memory>



namespace xl7 {



/**
 * Wraps the OS clipboard's text content. UTF-32, per the framework's "user
 * interaction" convention (see xl7::input::Keyboard::get_text_input()), so
 * callers get 1-to-1 code point indexing for free; converted to/from whatever
 * native encoding the OS clipboard actually uses under the hood (e.g., UTF-16
 * via CF_UNICODETEXT on Windows).
 */
class Clipboard final
    : public Component<Clipboard>
{
    friend class cl7::creational::Singleton<Clipboard>;



public:
    Clipboard(const Clipboard&) = delete;
    Clipboard& operator=(const Clipboard&) = delete;
    Clipboard(Clipboard&&) = delete;
    Clipboard& operator=(Clipboard&&) = delete;



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Returns the clipboard's current text content, or an empty string if the
     * clipboard is empty, holds non-text content, or couldn't be accessed.
     */
    cl7::u32string get_text() const;

    /**
     * Replaces the clipboard's content with the given text.
     * Returns whether that succeeded.
     */
    bool set_text(cl7::u32string_view text);



private:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    Clipboard() = default;
    ~Clipboard() override = default;



    // #############################################################################
    // Component Implementations
    // #############################################################################

    bool _init() override;
    bool _shutdown() override;



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * Our "pointer to implementation" of the platform-dependent operations.
     */
    std::unique_ptr<detail::IClipboardImpl> _pimpl;

}; // class Clipboard



    inline Clipboard& clipboard() { return Clipboard::instance(); }



} // namespace xl7

#endif // XL7_CLIPBOARD_H
