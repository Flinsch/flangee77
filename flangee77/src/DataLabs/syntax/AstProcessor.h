#ifndef DL7_SYNTAX_ASTPROCESSOR_H
#define DL7_SYNTAX_ASTPROCESSOR_H
#include "./ParseStage.h"

#include "./AstNode.h"



namespace dl7::syntax {



/**
 * An abstract AST processor responsible for generating an output representation. An
 * AST processor is the final stage in the parsing process. It takes an AST as input
 * and performs semantic analysis, optimization, and/or code generation, etc.
 */
template <class TAstNode = AstNode<>, typename Tresult = bool>
class AstProcessor
    : public ParseStage
{

public:
    using AstNode = TAstNode;
    using result_type = Tresult;



    using ParseStage::ParseStage;



    /**
     * Processes an abstract syntax tree (AST) to generate an output representation.
     */
    virtual result_type process(const AstNode& root) = 0;

}; // class AstProcessor



} // namespace dl7::syntax

#endif // DL7_SYNTAX_ASTPROCESSOR_H
