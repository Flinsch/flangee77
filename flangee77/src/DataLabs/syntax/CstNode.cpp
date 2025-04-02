#include "CstNode.h"



namespace dl7::syntax {



    /** Creates, adds, and returns a new child node. */
    CstNode* CstNode::add_child(const Token& token)
    {
        return add_child(std::unique_ptr<CstNode>(new CstNode(token)));
    }

    /** Adds (and returns) the given child node, taking ownership of it. */
    CstNode* CstNode::add_child(std::unique_ptr<CstNode> child)
    {
        assert(child->_parent == nullptr);
        child->_parent = this;
        _children.push_back(std::move(child));
        return _children.back().get();
    }

    /** Returns the child node at the specified index. */
    const CstNode* CstNode::get_child(size_t index) const
    {
        assert(index < _children.size());
        return _children[index].get();
    }

    /** Returns the child node at the specified index. */
    CstNode* CstNode::get_child(size_t index)
    {
        assert(index < _children.size());
        return _children[index].get();
    }



    /** Creates a new node representing the given token. */
    std::unique_ptr<CstNode> CstNode::create(const Token& token)
    {
        return std::unique_ptr<CstNode>(new CstNode(token));
    }



} // namespace dl7::syntax
