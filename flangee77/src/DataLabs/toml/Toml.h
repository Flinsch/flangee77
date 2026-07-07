#ifndef DL7_TOML_TOML_H
#define DL7_TOML_TOML_H

#include "./Value.h"



namespace dl7::toml {



/**
 * Represents a TOML document. A TOML document is always a table at its root. This
 * class is a thin wrapper around that root table (which maps keys to values).
 */
class Toml
{

public:
    Toml() = default;

    template <typename Ttable>
        requires(std::constructible_from<table_t, Ttable>)
    Toml(Ttable&& root)
        : _root(std::forward<Ttable>(root))
    {
    }

    Toml(const Toml& other) = default;
    Toml& operator=(const Toml& other) = default;
    Toml(Toml&& other) noexcept = default;
    Toml& operator=(Toml&& other) noexcept = default;

    ~Toml() noexcept = default;

    void swap(Toml& other) noexcept;



    const table_t& root() const noexcept { return _root; }
    table_t& root() noexcept { return _root; }



    const Value& at(cl7::u8string_view key) const { return _root.at(key); }
    Value& at(cl7::u8string_view key) { return _root.at(key); }

    Value& operator[](const cl7::u8string& key) { return _root[key]; }
    Value& operator[](cl7::u8string&& key) { return _root[std::move(key)]; }



    bool operator==(const Toml& other) const { return _root == other._root; }
    bool operator!=(const Toml& other) const { return !(*this == other); }



private:
    table_t _root;

}; // class Toml



} // namespace dl7::toml

#endif // DL7_TOML_TOML_H
