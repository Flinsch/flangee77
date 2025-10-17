#ifndef DL7_XML_TEXT_H
#define DL7_XML_TEXT_H
#include "./Node.h"



namespace dl7::xml {



class Text
    : public Node
{

public:
    Text() noexcept : Node(Type::Text) {}
    Text(cl7::u8string value, Node* parent_node = nullptr) noexcept : Node(Type::Text, parent_node), _value(std::move(value)) {}

    Text(const Text&) = delete;
    Text& operator=(const Text&) = delete;
    Text(Text&&) noexcept = default;
    Text& operator=(Text&&) noexcept = default;

    ~Text() noexcept override = default;



    const cl7::u8string& get_value() const { return _value; }

    template <cl7::string_constructible<cl7::u8string> Tstring>
    void set_value(Tstring&& value) { _value = cl7::u8string(std::forward<Tstring>(value)); }



private:
    cl7::u8string _value;

}; // class Text



} // namespace dl7::xml

#endif // DL7_XML_TEXT_H
