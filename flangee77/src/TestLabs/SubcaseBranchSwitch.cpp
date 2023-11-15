#include "SubcaseBranchSwitch.h"



namespace tl7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    SubcaseBranchSwitch::SubcaseBranchSwitch(Context& ctx, const cl7::string_view& name, const cl7::char_type* file_path, unsigned line_number, signed iteration_number)
        : _ctx( ctx )
        , _has_been_entered( false )
        , meta{ cl7::string(name), file_path, line_number, iteration_number }
    {
        _has_been_entered = _ctx.subcases.try_push( meta );
    }

    /**
     * Destructor.
     */
    SubcaseBranchSwitch::~SubcaseBranchSwitch()
    {
        if ( _has_been_entered )
        {
            const bool b = _ctx.subcases.try_pop( meta );
            assert( b );
        }
        else
        {
            assert( !_ctx.subcases.try_pop( meta ) );
        }
    }



} // namespace tl7
