
#include <TestLabs/TestSuite.h>

#include <XiaoLabs/Clipboard.h>

#include <CoreLabs/string.h>

#include "./shared.h"



namespace {

    /**
     * Restores the clipboard's original text content once this test case ends
     * (success, a failed CHECK, or a thrown ASSERT unwinding the stack): this
     * test necessarily touches the real OS clipboard (there's no fake
     * IClipboardImpl to swap in), so whatever the developer running the suite
     * had copied beforehand must come back afterward.
     */
    struct ClipboardRestoreGuard
    {
        cl7::u32string original_text;
        ~ClipboardRestoreGuard() { xl7::clipboard().set_text(original_text); }
    };

} // namespace



TESTLABS_CASE( u8"XiaoLabs:  Clipboard:  set_text/get_text round trip" )
{
    TESTLABS_ASSERT( xl7::clipboard().init() );

    const ClipboardRestoreGuard restore_guard{ xl7::clipboard().get_text() };

    // Includes a non-BMP code point (an emoji, outside the Basic Multilingual
    // Plane), to actually exercise the UTF-16 surrogate-pair round trip the
    // real OS clipboard (CF_UNICODETEXT) involves under the hood, not just
    // plain ASCII.
    const cl7::u32string text = U"Hello, \U0001F600 Clipboard!";

    TESTLABS_ASSERT( xl7::clipboard().set_text(text) );
    TESTLABS_CHECK_EQ( xl7::clipboard().get_text(), text );
}

TESTLABS_CASE( u8"XiaoLabs:  Clipboard:  set_text/get_text round trip with an empty string" )
{
    TESTLABS_ASSERT( xl7::clipboard().init() );

    const ClipboardRestoreGuard restore_guard{ xl7::clipboard().get_text() };

    // First seed it with something non-empty, so a no-op set_text() couldn't
    // accidentally pass by just leaving stale content behind.
    TESTLABS_ASSERT( xl7::clipboard().set_text(U"not empty") );

    TESTLABS_ASSERT( xl7::clipboard().set_text(U"") );
    TESTLABS_CHECK( xl7::clipboard().get_text().empty() );
}
