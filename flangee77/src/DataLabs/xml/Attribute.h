#ifndef DL7_XML_ATTRIBUTE_H
#define DL7_XML_ATTRIBUTE_H

#include <CoreLabs/string.h>



namespace dl7::xml {



class Attribute
{

public:
    Attribute() noexcept = default;
    explicit Attribute(cl7::u8string name, cl7::u8string string);

    void swap(Attribute& other) noexcept;



    /**
     * Returns true if the attribute value represents an empty string; returns false
     * otherwise (i.e., strings like "0" etc. are also considered non-empty).
     */
    bool is_empty() const noexcept { return _value.empty(); }



    /** Returns the name of this attribute. */
    const cl7::u8string& get_name() const { return _name; }

    /** Sets the name of this attribute. */
    template <cl7::string_constructible<cl7::u8string> Tstring>
    void set_name(Tstring&& name) { _name = cl7::u8string(std::forward<Tstring>(name)); }



    /** Returns the string value of this attribute. */
    const cl7::u8string& get_value() const { return _value; }

    /** Sets the string value of this attribute. */
    template <cl7::string_constructible<cl7::u8string> Tstring>
    void set_value(Tstring&& value) { _value = cl7::u8string(std::forward<Tstring>(value)); }



private:
    cl7::u8string _name;
    cl7::u8string _value;

}; // class Attribute



} // namespace dl7::xml

#endif // DL7_XML_ATTRIBUTE_H
