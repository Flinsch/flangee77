#ifndef DL7_SYNTAX_PARSESTAGE_H
#define DL7_SYNTAX_PARSESTAGE_H

#include "./Diagnostics.h"



namespace dl7::syntax {



/**
 * An abstract base class that provides a common interface and diagnostic support
 * for all parsing-related stages, including lexical analysis, syntax tree
 * construction/transformation, and AST processing (e.g., code generation). It is
 * designed to be inherited by specific parsing components.
 */
class ParseStage
{

public:
    ParseStage() = delete;

    ParseStage(const ParseStage&) = delete;
    ParseStage& operator=(const ParseStage&) = delete;
    ParseStage(ParseStage&&) = delete;
    ParseStage& operator=(ParseStage&&) = delete;

    virtual ~ParseStage() = default;



    /**
     * Returns the diagnostics object.
     */
    Diagnostics* get_diagnostics() const { return _diagnostics; }



protected:
    explicit ParseStage(Diagnostics* diagnostics);



private:
    /**
     * The diagnostics object.
     */
    Diagnostics* _diagnostics;

}; // class ParseStage



} // namespace dl7::syntax

#endif // DL7_SYNTAX_PARSESTAGE_H
