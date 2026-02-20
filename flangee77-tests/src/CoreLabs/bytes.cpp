
#include <TestLabs/TestSuite.h>

#include <CoreLabs/bytes.h>

#include "../shared.h"

#include <array>
#include <vector>



TESTLABS_CASE( u8"CoreLabs:  detail:  is_contiguous_without_gaps( raw C-style array )" )
{
    // Basic valid array
    int array1[4] = {1, 2, 3, 4};
    TESTLABS_CHECK(cl7::detail::is_contiguous_without_gaps(array1, 4));

    // Single element
    TESTLABS_CHECK(cl7::detail::is_contiguous_without_gaps(array1, 1));

    // Zero-length pointer
    TESTLABS_CHECK(cl7::detail::is_contiguous_without_gaps(static_cast<int*>(nullptr), 0));

    // Null pointer with nonzero count
    TESTLABS_CHECK(!cl7::detail::is_contiguous_without_gaps(static_cast<int*>(nullptr), 5));

    // Check with POD struct that has padding inside, should still be "tightly packed"
    struct S { char a; int b; }; // padding inside element is fine
    S array2[3];
    TESTLABS_CHECK(cl7::detail::is_contiguous_without_gaps(array2, 3));
}

TESTLABS_CASE( u8"CoreLabs:  detail:  is_contiguous_without_gaps( standard container )" )
{
    std::vector<int> v{1, 2, 3, 4};
    TESTLABS_CHECK(cl7::detail::is_contiguous_without_gaps(v));

    // Empty container
    std::vector<int> empty;
    TESTLABS_CHECK(cl7::detail::is_contiguous_without_gaps(empty));

    // Single element
    std::vector<int> one{42};
    TESTLABS_CHECK(cl7::detail::is_contiguous_without_gaps(one));

    // std::array
    std::array<double, 5> array = {1, 2, 3, 4, 5};
    TESTLABS_CHECK(cl7::detail::is_contiguous_without_gaps(array));

    // std::span over contiguous data
    std::span<int> sp(v);
    TESTLABS_CHECK(cl7::detail::is_contiguous_without_gaps(sp));

    // A fake "strided" span-like container to simulate misaligned memory
    struct FakeContiguous {
        using value_type = int;
        std::vector<std::byte> backing;
        FakeContiguous() { backing.resize(4 * 8); } // simulate 8-byte stride
        const int* data() const noexcept { return reinterpret_cast<const int*>(backing.data()); }
        size_t size() const noexcept { return 4; }
        const int& operator[](size_t i) const {
            return *reinterpret_cast<const int*>(&backing[i * 8]); // stride of 8 bytes
        }
        const int* begin() const noexcept { return &operator[](0); }
        const int* end() const noexcept { return &operator[](size()); }
    };

    FakeContiguous fake;
    fake.backing.resize(4 * 8);
    TESTLABS_CHECK(!cl7::detail::is_contiguous_without_gaps(fake));
}
