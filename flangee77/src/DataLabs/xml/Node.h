#ifndef DL7_XML_NODE_H
#define DL7_XML_NODE_H

#include <CoreLabs/string.h>



namespace dl7::xml {



class Node
{

public:
    enum struct Type
    {
        Element,
        Text,
    };



    Node() = delete;

    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
    Node(Node&&) noexcept = default;
    Node& operator=(Node&&) noexcept = default;

    virtual ~Node() noexcept = default;



    Type get_type() const noexcept { return _type; }



protected:
    Node(Type type, Node* parent_node = nullptr) noexcept : _type(type), _parent_node(parent_node) {}



private:
    Type _type;
    Node* _parent_node = nullptr;

}; // class Node



} // namespace dl7::xml

#endif // DL7_XML_NODE_H
