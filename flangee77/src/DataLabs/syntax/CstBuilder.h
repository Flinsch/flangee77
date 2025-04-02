#ifndef DL7_SYNTAX_CSTBUILDER_H
#define DL7_SYNTAX_CSTBUILDER_H
#include "./ParseStage.h"

#include "./Token.h"
#include "./CstNode.h"

#include <vector>



namespace dl7::syntax {



/**
 * An abstract CST builder responsible for constructing a concrete syntax tree (CST).
 * A CST builder is an intermediate stage after tokenization and before AST
 * transformation/processing. It analyzes a sequence of tokens and build a full
 * syntax tree representation that preserves all syntactic elements from the source.
 */
class CstBuilder
    : public ParseStage
{

public:
    using ParseStage::ParseStage;



    /**
     * Parses a sequence of tokens, constructs a parse tree (concrete syntax tree),
     * and returns the root node of this tree.
     */
    virtual std::unique_ptr<CstNode> build(const std::vector<Token>& tokens) = 0;

}; // class CstBuilder



} // namespace dl7::syntax

#endif // DL7_SYNTAX_CSTBUILDER_H
