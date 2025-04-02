#include "ParseStage.h"



namespace dl7::syntax {



    ParseStage::ParseStage(Diagnostics* diagnostics)
        : _diagnostics(diagnostics)
    {
        assert(diagnostics);
    }



} // namespace dl7::syntax
