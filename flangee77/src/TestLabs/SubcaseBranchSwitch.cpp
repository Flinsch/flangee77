#include "SubcaseBranchSwitch.h"



namespace tl7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    SubcaseBranchSwitch::SubcaseBranchSwitch(Context& ctx, cl7::u8string_view name, const char* file_path, unsigned line_number, const char* function_name, signed iteration_number)
        : _meta{cl7::u8string(name), file_path, line_number, function_name, iteration_number}
        , _ctx(ctx)
        , _has_been_entered(_ctx.subcases.try_push(_meta))
    {
    }

    SubcaseBranchSwitch::~SubcaseBranchSwitch()
    {
        if (_has_been_entered)
        {
            const bool b = _ctx.subcases.try_pop(_meta);
            assert(b);
        }
        else
        {
            assert(!_ctx.subcases.try_pop(_meta));
        }
    }



} // namespace tl7
