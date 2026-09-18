#ifndef XL7_DETAIL_ICLIPBOARDIMPL_H
#define XL7_DETAIL_ICLIPBOARDIMPL_H

#include <CoreLabs/string.h>



namespace xl7::detail {



class IClipboardImpl
{
public:
    virtual ~IClipboardImpl() = default;



    virtual cl7::u32string get_text() = 0;
    virtual bool set_text(cl7::u32string_view text) = 0;

}; // IClipboardImpl



} // namespace xl7::detail

#endif // XL7_DETAIL_ICLIPBOARDIMPL_H
