#ifndef DL7_XML_ATTRIBUTE_H
#define DL7_XML_ATTRIBUTE_H

#include <CoreLabs/string.h>



namespace dl7::xml {



class Attribute
{

public:
    Attribute() noexcept = default;
    explicit Attribute(cl7::u8string string);
    explicit Attribute(cl7::u8string_view string);
    explicit Attribute(const cl7::u8char_t* string);

    explicit Attribute(float number);
    explicit Attribute(double number);
    explicit Attribute(long double number);

    explicit Attribute(signed number);
    explicit Attribute(signed long number);
    explicit Attribute(signed long long number);

    explicit Attribute(unsigned number);
    explicit Attribute(unsigned long number);
    explicit Attribute(unsigned long long number);

    void swap(Attribute& other) noexcept;



    /**
     * Returns true if the attribute value represents an empty string; returns false
     * otherwise (i.e., strings like "0" etc. are also considered non-empty).
     */
    bool is_empty() const noexcept;



    const cl7::u8string& get_value() const { return _value; }

    template <cl7::string_constructible<cl7::u8string> Tstring>
    void set_value(Tstring&& value) { _value = cl7::u8string(std::forward<Tstring>(value)); }



private:
    cl7::u8string _value;

}; // class Attribute



} // namespace dl7::xml

#endif // DL7_XML_ATTRIBUTE_H
