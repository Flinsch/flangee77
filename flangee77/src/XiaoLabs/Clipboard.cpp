#include "Clipboard.h"

#if F77_IS_WINDOWS
#include "./detail/windows/ClipboardImpl.h"
namespace {
namespace impl = xl7::detail::windows;
}
#endif

#include <CoreLabs/logging.h>



namespace xl7 {



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Returns the clipboard's current text content, or an empty string if the
     * clipboard is empty, holds non-text content, or couldn't be accessed.
     */
    cl7::u32string Clipboard::get_text() const
    {
        assert(_pimpl);
        if (!_pimpl)
            return {};

        return _pimpl->get_text();
    }

    /**
     * Replaces the clipboard's content with the given text.
     * Returns whether that succeeded.
     */
    bool Clipboard::set_text(cl7::u32string_view text)
    {
        assert(_pimpl);
        if (!_pimpl)
            return false;

        return _pimpl->set_text(text);
    }



    // #############################################################################
    // Component Implementations
    // #############################################################################

    bool Clipboard::_init()
    {
        _pimpl = std::make_unique<impl::ClipboardImpl>();

        LOG_SUCCESS(u8"The clipboard has been successfully initialized.");
        return true;
    }

    bool Clipboard::_shutdown()
    {
        _pimpl.reset();

        LOG_SUCCESS(u8"The clipboard has been shut down.");
        return true;
    }



} // namespace xl7
