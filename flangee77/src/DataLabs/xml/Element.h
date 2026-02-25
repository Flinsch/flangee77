#ifndef DL7_XML_ELEMENT_H
#define DL7_XML_ELEMENT_H
#include "./Node.h"

#include "./Attribute.h"

#include <vector>



namespace dl7::xml {



class Text;



class Element
    : public Node
{

public:
    Element() noexcept : Node(Type::Element) {}
    Element(cl7::u8string name, Element* parent_element = nullptr) noexcept : Node(Type::Element, parent_element), _name(std::move(name)) {}

    Element(const Element& other, Element* parent_element);
    Element(Element&& other, Element* parent_element);

    Element(const Element&) = delete;
    Element& operator=(const Element&) = delete;
    Element(Element&&) noexcept = default;
    Element& operator=(Element&&) noexcept = default;

    ~Element() noexcept override = default;



    /** Returns the tag name of this element. */
    const cl7::u8string& get_name() const { return _name; }

    /** Sets the tag name of this element. */
    template <cl7::string_constructible<cl7::u8string> Tstring>
    void set_name(Tstring&& name) { _name = cl7::u8string(std::forward<Tstring>(name)); }

    /** Returns the attributes of this element. */
    const std::vector<Attribute>& attributes() const { return _attributes; }
    /** Returns the attributes of this element. */
    std::vector<Attribute>& attributes() { return _attributes; }



    /**
     * Checks whether an attribute with the specified name exists.
     */
    bool has_attribute(cl7::u8string_view name) const;

    /**
     * Searches for an attribute with the specified name and returns it if found,
     * otherwise returns NULL. If there are multiple attributes with that name, the
     * first one is returned.
     */
    const Attribute* find_attribute(cl7::u8string_view name) const;

    /**
     * Searches for an attribute with the specified name and returns it if found,
     * otherwise returns NULL. If there are multiple attributes with that name, the
     * first one is returned.
     */
    Attribute* find_attribute(cl7::u8string_view name);

    /**
     * Ensures that an attribute with the specified name is defined and returns it,
     * after creating it if not found.
     */
    Attribute* ensure_attribute(cl7::u8string_view name);

    /**
     * Ensures that an attribute with the specified name and value is defined and
     * returns it, after creating it if not found.
     */
    Attribute* ensure_attribute(cl7::u8string_view name, cl7::u8string value);



    /**
     * Checks whether a child element with the specified name exists.
     */
    bool has_child_element(cl7::u8string_view name) const;

    /**
     * Searches for a child element with the specified name and returns it if found,
     * otherwise returns NULL. If there are multiple elements with that name, the
     * first one is returned.
     */
    const Element* find_child_element(cl7::u8string_view name) const;

    /**
     * Searches for a child element with the specified name and returns it if found,
     * otherwise returns NULL. If there are multiple elements with that name, the
     * first one is returned.
     */
    Element* find_child_element(cl7::u8string_view name);



    /** Returns a "list" of all child nodes. */
    std::vector<const Node*> find_child_nodes() const;
    /** Returns a "list" of all child nodes of the specified type. */
    std::vector<const Node*> find_child_nodes(Type type) const;
    /** Returns a "list" of all child elements. */
    std::vector<const Element*> find_child_elements() const;
    /** Returns a "list" of all text nodes. */
    std::vector<const Text*> find_text_nodes() const;

    /** Returns a "list" of all child nodes. */
    std::vector<Node*> find_child_nodes();
    /** Returns a "list" of all child nodes of the specified type. */
    std::vector<Node*> find_child_nodes(Type type);
    /** Returns a "list" of all child elements. */
    std::vector<Element*> find_child_elements();
    /** Returns a "list" of all text nodes. */
    std::vector<Text*> find_text_nodes();



    /**
     * Returns true if this element has no child elements and no textual content;
     * returns false otherwise.
     */
    bool is_empty() const noexcept { return _child_nodes.empty(); }



    /**
     * Appends a new child element with the given tag name. The new element is
     * inserted after the specified node (if provided). If no node is specified, the
     * new element is added as the last child. Returns a pointer to the new element.
     */
    template <cl7::string_constructible<cl7::u8string> Tstring>
    Element* append_element(Tstring&& name, const Node* insert_after = nullptr)
    {
        return _append_element(cl7::u8string(std::forward<Tstring>(name)), insert_after);
    }

    /**
     * Prepends a new child element with the given tag name. The new element is
     * inserted before the specified node (if provided). If no node is specified, the
     * new element is added as the first child. Returns a pointer to the new element.
     */
    template <cl7::string_constructible<cl7::u8string> Tstring>
    Element* prepend_element(Tstring&& name, const Node* insert_before = nullptr)
    {
        return _prepend_element(cl7::u8string(std::forward<Tstring>(name)), insert_before);
    }



    /**
     * Appends a new text node with the given content. The new text node is inserted
     * after the specified node (if provided). If no node is specified, the new text
     * node is added as the last child. Returns a pointer to the new text node.
     */
    template <cl7::string_constructible<cl7::u8string> Tstring>
    Text* append_text(Tstring&& content, const Node* insert_after = nullptr)
    {
        return _append_text(cl7::u8string(std::forward<Tstring>(content)), insert_after);
    }

    /**
     * Prepends a new text node with the given content. The new text node is inserted
     * before the specified node (if provided). If no node is specified, the new text
     * node is added as the first child. Returns a pointer to the new text node.
     */
    template <cl7::string_constructible<cl7::u8string> Tstring>
    Text* prepend_text(Tstring&& content, const Node* insert_before = nullptr)
    {
        return _prepend_text(cl7::u8string(std::forward<Tstring>(content)), insert_before);
    }



    /**
     * Computes and returns the textual content of this element: all text node
     * contents concatenated without any whitespace between each.
     */
    cl7::u8string concatenate_text_content() const;

    /**
     * Sets the textual content of this element. This removes all existing text
     * nodes and appends a new text node with the specified content.
     */
    template <cl7::string_constructible<cl7::u8string> Tstring>
    void set_text_content(Tstring&& content) { _set_text_content(cl7::u8string(std::forward<Tstring>(content))); }



    bool operator==(const Element& other) const { return _is_equal(other); }
    bool operator!=(const Element& other) const = default;



private:
    void _set_text_content(cl7::u8string&& content);

    Element* _append_element(cl7::u8string&& name, const Node* insert_after);
    Element* _prepend_element(cl7::u8string&& name, const Node* insert_before);

    Text* _append_text(cl7::u8string&& content, const Node* insert_after);
    Text* _prepend_text(cl7::u8string&& content, const Node* insert_before);

    void _append_node(std::unique_ptr<Node>&& node_ptr, const Node* insert_after);
    void _prepend_node(std::unique_ptr<Node>&& node_ptr, const Node* insert_before);

    std::vector<std::unique_ptr<Node>>::iterator _find_node(const Node* node);

    bool _is_equal(const Element& other) const;

    cl7::u8string _name;
    std::vector<Attribute> _attributes;
    std::vector<std::unique_ptr<Node>> _child_nodes;

}; // class Element



} // namespace dl7::xml

#endif // DL7_XML_ELEMENT_H
