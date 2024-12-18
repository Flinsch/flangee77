#include "ChannelOrder.h"



namespace cl7 {
    cl7::string to_string(xl7::graphics::ChannelOrder channel_order)
    {
        switch (channel_order)
        {
#define DEFINE_CASE(name) case xl7::graphics::ChannelOrder::name: return TEXT(#name)
            DEFINE_CASE(RGBA);
            DEFINE_CASE(ARGB);
            DEFINE_CASE(ABGR);
            DEFINE_CASE(BGRA);
#undef DEFINE_CASE
        default:
            assert( false );
        }

        return TEXT("UNKNOWN");
    }
}
