#ifndef XL7_RESOURCES_FAKEDATAPROVIDER_H
#define XL7_RESOURCES_FAKEDATAPROVIDER_H
#include "./DataProvider.h"



namespace xl7::resources {



class FakeDataProvider
    : public DataProvider
{

public:
    FakeDataProvider() = default;

    explicit FakeDataProvider(size_t capacity);

    ~FakeDataProvider() override = default;



    /**
     * Returns the target data offset, in bytes.
     * As a fake data provider, in order to provide a certain capacity, 0 is always
     * returned here
     */
    size_t get_offset() const override { return 0; }

    /**
     * Returns the provided data size, in bytes.
     * As a fake data provider, in order to provide a certain capacity, this
     * capacity is returned here.
     */
    size_t get_size() const override { return _capacity; }



private:

    /**
     * As a fake data provider, nothing happens here.
     */
    void _fill(cl7::byte_span target) const override;



    size_t _capacity;

}; // class FakeDataProvider



} // namespace xl7::resources

#endif // XL7_RESOURCES_FAKEDATAPROVIDER_H
