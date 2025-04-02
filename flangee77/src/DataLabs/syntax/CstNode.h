#ifndef DL7_SYNTAX_CSTNODE_H
#define DL7_SYNTAX_CSTNODE_H

#include "./Token.h"

#include <vector>



namespace dl7::syntax {



/**
 * Represents a node in a concrete syntax tree (CST). A CST node corresponds
 * directly to a token from the source text, maintaining the full syntactic
 * structure as parsed. Each node stores its associated token, a reference to
 * its parent, and a list of child nodes.
 */
class CstNode
{

public:
    CstNode() = delete;



    /** Returns the token associated with this node. */
    const Token& get_token() const { return _token; }

    /** Returns the parent node. */
    const CstNode* get_parent() const { return _parent; }

    /** Returns the parent node. */
    CstNode* get_parent() { return _parent; }

    /** Creates, adds, and returns a new child node. */
    CstNode* add_child(const Token& token);

    /** Adds (and returns) the given child node, taking ownership of it. */
    CstNode* add_child(std::unique_ptr<CstNode> child);

    /** Returns the number of child nodes. */
    size_t get_child_count() const { return _children.size(); }

    /** Returns the child node at the specified index. */
    const CstNode* get_child(size_t index) const;

    /** Returns the child node at the specified index. */
    CstNode* get_child(size_t index);



    /** Creates a new node representing the given token. */
    static std::unique_ptr<CstNode> create(const Token& token);



private:
    CstNode(const Token& token) : _token(token) {}

    /** The token associated with this node. */
    Token _token;
    /** The parent node (or NULL if this is the root). */
    CstNode* _parent = nullptr;
    /** The child nodes. */
    std::vector<std::unique_ptr<CstNode>> _children;

}; // class CstNode



} // namespace dl7::syntax

#endif // DL7_SYNTAX_CSTNODE_H
