#pragma once
#ifndef CL7_BYTEVIEW_H
#define CL7_BYTEVIEW_H

#include <CoreLabs/root.h>

#include <span>



namespace cl7 {



    typedef std::span<const std::byte> byte_view;



} // namespace cl7

#endif // CL7_BYTEVIEW_H
