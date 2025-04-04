#ifndef DL7_SYNTAX_DIRECTASTBUILDER_H
#define DL7_SYNTAX_DIRECTASTBUILDER_H
#include "./ParseStage.h"

#include "./TokenReader.h"
#include "./AstNode.h"



namespace dl7::syntax {



/**
 * An abstract AST builder responsible for constructing an abstract syntax tree (AST)
 * directly from a sequence of tokens, bypassing the construction of a concrete
 * syntax tree (CST). An AST builder is an intermediate stage after tokenization and
 * before AST processing. It analyzes a sequence of tokens and builds a compact and
 * semantically meaningful AST representation.
 *
 * This serves as an alternative to the traditional two-step approach, where a CST
 * is first built and then transformed into an AST. Direct AST construction can be
 * beneficial for performance or when a CST is not necessary.
 */
template <class TAstNode = AstNode<>>
class DirectAstBuilder
    : public ParseStage
{

public:
    using AstNode = TAstNode;



    using ParseStage::ParseStage;



    /**
     * Parses a sequence of tokens, constructs an abstract syntax tree directly
     * (without generating an intermediate concrete syntax tree), and returns the
     * root node of this tree.
     */
    virtual std::unique_ptr<AstNode> build(TokenReader& token_reader) = 0;

}; // class DirectAstBuilder



} // namespace dl7::syntax

#endif // DL7_SYNTAX_DIRECTASTBUILDER_H
