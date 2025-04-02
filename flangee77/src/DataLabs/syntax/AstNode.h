#ifndef DL7_SYNTAX_ASTNODE_H
#define DL7_SYNTAX_ASTNODE_H

#include "./CstNode.h"



namespace dl7::syntax {



/**
 * Represents a node in an abstract syntax tree (AST). Unlike a CST node, an AST
 * node represents the semantic structure of parsed code, omitting unnecessary
 * syntactic details. AST nodes are typically derived from CST nodes but focus on
 * representing program logic.
 */
template <typename Ttype = unsigned>
class AstNode
{

public:
    using Type = Ttype;



    AstNode() = delete;

    AstNode(Type type) : _type(type) {}

    virtual ~AstNode() = default;



    /** Returns the specific type of this node. */
    Type get_type() const { return _type; }



private:
    /** The specific type of this node. */
    Type _type;

}; // class AstNode



} // namespace dl7::syntax

#endif // DL7_SYNTAX_ASTNODE_H
