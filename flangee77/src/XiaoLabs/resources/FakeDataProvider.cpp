#include "FakeDataProvider.h"



namespace xl7::resources {



    FakeDataProvider::FakeDataProvider(size_t capacity)
        : _capacity(capacity)
    {
    }



    /**
     * As a fake data provider, nothing happens here.
     */
    void FakeDataProvider::_fill(cl7::byte_span target) const
    {
        assert(target.size() >= _capacity);
    }



} // namespace xl7::resources
