#include "Element.h"

#include "Text.h"


namespace dl7::xml {



    /** Returns a "list" of all child nodes. */
    std::vector<Node*> Element::find_child_nodes() const
    {
        std::vector<Node*> result;

        for (const auto& node_ptr : _child_nodes)
        {
            result.push_back(node_ptr.get());
        }

        return result;
    }

    /** Returns a "list" of all child nodes of the specified type. */
    std::vector<Node*> Element::find_child_nodes(Type type) const
    {
        std::vector<Node*> result;

        for (const auto& node_ptr : _child_nodes)
        {
            if (node_ptr->get_type() == type)
                result.push_back(node_ptr.get());
        }

        return result;
    }

    /** Returns a "list" of all child elements. */
    std::vector<Element*> Element::find_child_elements() const
    {
        std::vector<Element*> result;

        for (const auto& node_ptr : _child_nodes)
        {
            if (node_ptr->get_type() == Type::Element)
                result.push_back(reinterpret_cast<Element*>(node_ptr.get()));
        }

        return result;
    }

    /** Returns a "list" of all text nodes. */
    std::vector<Text*> Element::find_text_nodes() const
    {
        std::vector<Text*> result;

        for (const auto& node_ptr : _child_nodes)
        {
            if (node_ptr->get_type() == Type::Text)
                result.push_back(reinterpret_cast<Text*>(node_ptr.get()));
        }

        return result;
    }



    /**
     * Returns the contained text of this element: the values of all text nodes
     * concatenated with a space between each.
     */
    cl7::u8string Element::get_value() const
    {
        cl7::u8string value;

        auto text_nodes = find_text_nodes();

        for (const auto* text_node : text_nodes)
        {
            if (!value.empty())
                value.append(1, u8' ');
            value.append(text_node->get_value());
        }

        return value;
    }

    void Element::_set_value(cl7::u8string&& value)
    {
        std::erase_if(_child_nodes, [](auto& node_ptr) {
            return node_ptr->get_type() == Type::Text;
        });

        append_text(std::move(value));
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



    Text* Element::_append_text(cl7::u8string&& value, const Node* insert_after)
    {
        auto text_ptr = std::make_unique<Text>(std::move(value), this);
        Text* text = text_ptr.get();

        _append_node(std::move(text_ptr), insert_after);

        return text;
    }

    Text* Element::_prepend_text(cl7::u8string&& value, const Node* insert_before)
    {
        auto text_ptr = std::make_unique<Text>(std::move(value), this);
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



} // namespace dl7::xml
