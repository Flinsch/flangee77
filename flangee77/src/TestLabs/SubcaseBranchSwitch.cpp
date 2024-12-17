#include "SubcaseBranchSwitch.h"



namespace tl7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    SubcaseBranchSwitch::SubcaseBranchSwitch(Context& ctx, cl7::string_view name, const cl7::char_type* file_path, unsigned line_number, signed iteration_number)
        : _meta{cl7::string(name), file_path, line_number, iteration_number}
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
