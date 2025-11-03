#ifndef DL7_XML_NODE_H
#define DL7_XML_NODE_H

#include <CoreLabs/string.h>



namespace dl7::xml {



class Element;



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
    bool is_element() const noexcept { return _type == Type::Element; }
    bool is_text() const noexcept { return _type == Type::Text; }

    const Element* get_parent_element() const noexcept { return _parent_element; }
    Element* get_parent_element() noexcept { return _parent_element; }



    bool operator==(const Node& other) const noexcept;
    bool operator!=(const Node& other) const noexcept = default;



protected:
    Node(Type type, Element* parent_element = nullptr) noexcept : _type(type), _parent_element(parent_element) {}



private:
    Type _type;
    Element* _parent_element = nullptr;

}; // class Node



} // namespace dl7::xml

#endif // DL7_XML_NODE_H
