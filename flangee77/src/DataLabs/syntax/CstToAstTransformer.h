#ifndef DL7_SYNTAX_CSTTOASTTRANSFORMER_H
#define DL7_SYNTAX_CSTTOASTTRANSFORMER_H
#include "./ParseStage.h"

#include "./CstNode.h"
#include "./AstNode.h"



namespace dl7::syntax {



/**
 * An abstract class responsible for transforming a concrete syntax tree (CST) into
 * an abstract syntax tree (AST). A syntax tree transformer is an intermediate stage
 * after CST creation and before AST processing. It converts a fully detailed CST
 * into a more compact and semantically meaningful AST representation.
 */
template <class TAstNode = AstNode>
class CstToAstTransformer
    : public ParseStage
{

public:
    using AstNode = TAstNode;



    using ParseStage::ParseStage;



    /**
     * Transforms a concrete syntax tree (CST) into an abstract syntax tree (AST).
     */
    virtual std::unique_ptr<AstNode> transform(const CstNode& root) = 0;

}; // class CstToAstTransformer



} // namespace dl7::syntax

#endif // DL7_SYNTAX_CSTTOASTTRANSFORMER_H
