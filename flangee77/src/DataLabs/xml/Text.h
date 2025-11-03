#ifndef DL7_XML_TEXT_H
#define DL7_XML_TEXT_H
#include "./Node.h"



namespace dl7::xml {



class Text
    : public Node
{

public:
    Text() noexcept : Node(Type::Text) {}
    Text(cl7::u8string content, Element* parent_element = nullptr) noexcept : Node(Type::Text, parent_element), _content(std::move(content)) {}

    Text(const Text& other, Element* parent_element);
    Text(Text&& other, Element* parent_element) noexcept;

    Text(const Text&) = delete;
    Text& operator=(const Text&) = delete;
    Text(Text&&) noexcept = default;
    Text& operator=(Text&&) noexcept = default;

    ~Text() noexcept override = default;



    const cl7::u8string& get_content() const { return _content; }

    template <cl7::string_constructible<cl7::u8string> Tstring>
    void set_content(Tstring&& content) { _content = cl7::u8string(std::forward<Tstring>(content)); }



    bool operator==(const Text& other) const noexcept { return _is_equal(other); }
    bool operator!=(const Text& other) const noexcept = default;



private:
    bool _is_equal(const Text& other) const noexcept { return _content == other._content; }

    cl7::u8string _content;

}; // class Text



} // namespace dl7::xml

#endif // DL7_XML_TEXT_H
