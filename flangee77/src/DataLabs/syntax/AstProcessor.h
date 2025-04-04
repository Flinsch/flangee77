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
template <class TAstNode = AstNode<>>
class AstProcessor
    : public ParseStage
{

public:
    using AstNode = TAstNode;



    using ParseStage::ParseStage;



    /**
     * Processes an abstract syntax tree (AST) to generate an output representation.
     * Returns true if operation succeeds, false otherwise.
     */
    virtual bool process(const AstNode& root) = 0;

}; // class AstProcessor



} // namespace dl7::syntax

#endif // DL7_SYNTAX_ASTPROCESSOR_H
