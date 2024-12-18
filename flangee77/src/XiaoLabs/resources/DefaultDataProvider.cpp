#include "DefaultDataProvider.h"

#include <algorithm>



namespace xl7::resources {



    DefaultDataProvider::DefaultDataProvider(cl7::byte_view data, size_t offset)
        : _data(data)
        , _offset(offset)
    {
    }



    /**
     * Fills the given byte span with the provided data. Any specified offset of
     * this data provider is not taken into account here; this must have been
     * applied beforehand when defining the byte span.
     */
    void DefaultDataProvider::_fill(cl7::byte_span target) const
    {
        assert(target.size() >= _data.size());
        std::ranges::copy(_data, target.begin());
    }



} // namespace xl7::resources
