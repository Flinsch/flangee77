#ifndef XL7_DETAIL_WINDOWS_CLIPBOARDIMPL_H
#define XL7_DETAIL_WINDOWS_CLIPBOARDIMPL_H
#include "../IClipboardImpl.h"
#if F77_IS_WINDOWS



namespace xl7::detail::windows {



class ClipboardImpl final
    : public IClipboardImpl
{
public:
    ~ClipboardImpl() override = default;



    cl7::u32string get_text() override;
    bool set_text(cl7::u32string_view text) override;

}; // ClipboardImpl



} // namespace xl7::detail::windows

#endif // F77_IS_WINDOWS
#endif // XL7_DETAIL_WINDOWS_CLIPBOARDIMPL_H
