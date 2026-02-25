#include "Element.h"

#include "Text.h"

#include <CoreLabs/sstream.h>
#include <CoreLabs/utilities.h>

#include <algorithm>
#include <utility>



namespace dl7::xml {



    Element::Element(const Element& other, Element* parent_element)
        : Element(other._name, parent_element)
    {
        _attributes = other._attributes;

        _child_nodes.reserve(other._child_nodes.size());

        for (const auto& node_ptr : other._child_nodes)
        {
            switch (node_ptr->get_type())
            {
            case Type::Element:
            {
                const auto* child_element = reinterpret_cast<const Element*>(node_ptr.get());
                _child_nodes.emplace_back(std::make_unique<Element>(*child_element, this));
                break;
            }
            case Type::Text:
            {
                const auto* child_text = reinterpret_cast<const Text*>(node_ptr.get());
                _child_nodes.emplace_back(std::make_unique<Text>(*child_text, this));
                break;
            }
            default:
                assert(false);
            }
        }
    }

    Element::Element(Element&& other, Element* parent_element)
        : Element(std::move(other._name), parent_element)
    {
        _attributes = std::move(other._attributes);

        _child_nodes.reserve(other._child_nodes.size());

        for (auto& node_ptr : other._child_nodes)
            _child_nodes.push_back(std::move(node_ptr));
    }



    /**
     * Checks whether an attribute with the specified name exists.
     */
    bool Element::has_attribute(cl7::u8string_view name) const
    {
        return find_attribute(name) != nullptr;
    }

    /**
     * Searches for an attribute with the specified name and returns it if found,
     * otherwise returns NULL. If there are multiple attributes with that name, the
     * first one is returned.
     */
    const Attribute* Element::find_attribute(cl7::u8string_view name) const
    {
        for (const auto& attribute : _attributes)
        {
            if (attribute.get_name() != name)
                continue;

            return &attribute;
        }

        return nullptr;
    }

    /**
     * Searches for an attribute with the specified name and returns it if found,
     * otherwise returns NULL. If there are multiple attributes with that name, the
     * first one is returned.
     */
    Attribute* Element::find_attribute(cl7::u8string_view name)
    {
        return const_cast<Attribute*>(std::as_const(*this).find_attribute(name)); // NOLINT(cppcoreguidelines-pro-type-const-cast)
    }

    /**
     * Ensures that an attribute with the specified name is defined and returns it,
     * after creating it if not found.
     */
    Attribute* Element::ensure_attribute(cl7::u8string_view name)
    {
        Attribute* attribute = find_attribute(name);
        if (attribute != nullptr)
            return attribute;

        _attributes.emplace_back(cl7::u8string{name}, u8"");
        return &_attributes.back();
    }

    /**
     * Ensures that an attribute with the specified name and value is defined and
     * returns it, after creating it if not found.
     */
    Attribute* Element::ensure_attribute(cl7::u8string_view name, cl7::u8string value)
    {
        Attribute* attribute = find_attribute(name);
        if (attribute != nullptr)
            return attribute;

        _attributes.emplace_back(cl7::u8string{name}, std::move(value));
        return &_attributes.back();
    }



    /**
     * Checks whether a child element with the specified name exists.
     */
    bool Element::has_child_element(cl7::u8string_view name) const
    {
        return find_child_element(name) != nullptr;
    }

    /**
     * Searches for an element with the specified name and returns it if found,
     * otherwise returns NULL. If there are multiple elements with that name, the
     * first one is returned.
     */
    const Element* Element::find_child_element(cl7::u8string_view name) const
    {
        for (const auto& node_ptr : _child_nodes)
        {
            if (node_ptr->get_type() != Type::Element)
                continue;

            const Element* element = reinterpret_cast<Element*>(node_ptr.get());
            if (element->get_name() != name)
                continue;

            return element;
        }

        return nullptr;
    }

    /**
     * Searches for an element with the specified name and returns it if found,
     * otherwise returns NULL. If there are multiple elements with that name, the
     * first one is returned.
     */
    Element* Element::find_child_element(cl7::u8string_view name)
    {
        return const_cast<Element*>(std::as_const(*this).find_child_element(name)); // NOLINT(cppcoreguidelines-pro-type-const-cast)
    }



    template <class Tvector, typename Tnode = std::conditional_t<std::is_const_v<Tvector>, const Node, Node>>
    static std::vector<Tnode*> _find_child_nodes(Tvector& child_nodes)
    {
        std::vector<Tnode*> result;
        result.reserve(child_nodes.size());

        for (const auto& node_ptr : child_nodes)
        {
            result.push_back(node_ptr.get());
        }

        return result;
    }

    template <class Tvector, typename Tnode = std::conditional_t<std::is_const_v<Tvector>, const Node, Node>>
    static std::vector<Tnode*> _find_child_nodes(Tvector& child_nodes, Node::Type type)
    {
        std::vector<Tnode*> result;
        result.reserve(child_nodes.size());

        for (const auto& node_ptr : child_nodes)
        {
            if (node_ptr->get_type() == type)
                result.push_back(node_ptr.get());
        }

        return result;
    }

    template <class Tvector, typename Telement = std::conditional_t<std::is_const_v<Tvector>, const Element, Element>>
    static std::vector<Telement*> _find_child_elements(Tvector& child_nodes)
    {
        std::vector<Telement*> result;
        result.reserve(child_nodes.size());

        for (const auto& node_ptr : child_nodes)
        {
            if (node_ptr->get_type() == Node::Type::Element)
                result.push_back(reinterpret_cast<Element*>(node_ptr.get()));
        }

        return result;
    }

    template <class Tvector, typename Ttext = std::conditional_t<std::is_const_v<Tvector>, const Text, Text>>
    static std::vector<Ttext*> _find_text_nodes(Tvector& child_nodes)
    {
        std::vector<Ttext*> result;
        result.reserve(child_nodes.size());

        for (const auto& node_ptr : child_nodes)
        {
            if (node_ptr->get_type() == Node::Type::Text)
                result.push_back(reinterpret_cast<Text*>(node_ptr.get()));
        }

        return result;
    }

    /** Returns a "list" of all child nodes. */
    std::vector<const Node*> Element::find_child_nodes() const { return _find_child_nodes(_child_nodes); }
    /** Returns a "list" of all child nodes of the specified type. */
    std::vector<const Node*> Element::find_child_nodes(Type type) const { return _find_child_nodes(_child_nodes, type); }
    /** Returns a "list" of all child elements. */
    std::vector<const Element*> Element::find_child_elements() const { return _find_child_elements(_child_nodes); }
    /** Returns a "list" of all text nodes. */
    std::vector<const Text*> Element::find_text_nodes() const { return _find_text_nodes(_child_nodes); }

    /** Returns a "list" of all child nodes. */
    std::vector<Node*> Element::find_child_nodes() { return _find_child_nodes(_child_nodes); }
    /** Returns a "list" of all child nodes of the specified type. */
    std::vector<Node*> Element::find_child_nodes(Type type) { return _find_child_nodes(_child_nodes, type); }
    /** Returns a "list" of all child elements. */
    std::vector<Element*> Element::find_child_elements() { return _find_child_elements(_child_nodes); }
    /** Returns a "list" of all text nodes. */
    std::vector<Text*> Element::find_text_nodes() { return _find_text_nodes(_child_nodes); }



    /**
     * Computes and returns the textual content of this element: all text node
     * contents concatenated without any whitespace between each.
     */
    cl7::u8string Element::concatenate_text_content() const
    {
        cl7::u8osstream oss;

        for (const auto* text_node : find_text_nodes())
            oss << text_node->get_content();

        return oss.str();
    }

    void Element::_set_text_content(cl7::u8string&& content)
    {
        std::erase_if(_child_nodes, [](auto& node_ptr) {
            return node_ptr->get_type() == Type::Text;
        });

        append_text(std::move(content));
    }



    Element* Element::_append_element(cl7::u8string&& name, const Node* insert_after)
    {
        auto element_ptr = std::make_unique<Element>(std::move(name), this);
        Element* element = element_ptr.get();

        _append_node(std::move(element_ptr), insert_after);

        return element;
    }

    Element* Element::_prepend_element(cl7::u8string&& name, const Node* insert_before)
    {
        auto element_ptr = std::make_unique<Element>(std::move(name), this);
        Element* element = element_ptr.get();

        _prepend_node(std::move(element_ptr), insert_before);

        return element;
    }



    Text* Element::_append_text(cl7::u8string&& content, const Node* insert_after)
    {
        auto text_ptr = std::make_unique<Text>(std::move(content), this);
        Text* text = text_ptr.get();

        _append_node(std::move(text_ptr), insert_after);

        return text;
    }

    Text* Element::_prepend_text(cl7::u8string&& content, const Node* insert_before)
    {
        auto text_ptr = std::make_unique<Text>(std::move(content), this);
        Text* text = text_ptr.get();

        _prepend_node(std::move(text_ptr), insert_before);

        return text;
    }



    void Element::_append_node(std::unique_ptr<Node>&& node_ptr, const Node* insert_after)
    {
        const auto it = _find_node(insert_after);

        if (it == _child_nodes.end())
            _child_nodes.emplace_back(std::move(node_ptr));
        else
            _child_nodes.emplace(it + 1, std::move(node_ptr));
    }

    void Element::_prepend_node(std::unique_ptr<Node>&& node_ptr, const Node* insert_before)
    {
        const auto it = _find_node(insert_before);

        if (it == _child_nodes.end())
            _child_nodes.emplace(_child_nodes.begin(), std::move(node_ptr));
        else
            _child_nodes.emplace(it, std::move(node_ptr));
    }



    std::vector<std::unique_ptr<Node>>::iterator Element::_find_node(const Node* node)
    {
        if (!node)
            return _child_nodes.end();

        return std::ranges::find_if(_child_nodes, [node](auto& node_ptr) {
            return node_ptr.get() == node;
        });
    }



    bool Element::_is_equal(const Element& other) const
    {
        if (_name != other._name)
            return false;

        if (!cl7::unordered_equal(_attributes, other._attributes))
            return false;

        if (!std::ranges::equal(_child_nodes, other._child_nodes, [](const auto& a, const auto& b) { return *a == *b; }))
            return false;

        return true;
    }



} // namespace dl7::xml
