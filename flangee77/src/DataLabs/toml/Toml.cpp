#include "Toml.h"



namespace dl7::toml {



    void Toml::swap(Toml& other) noexcept
    {
        _root.swap(other._root);
    }



} // namespace dl7::toml
