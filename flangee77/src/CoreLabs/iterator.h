#ifndef CL7_ITERATOR_H
#define CL7_ITERATOR_H

#include <CoreLabs/root.h>

#include <vector>



namespace cl7 {



template <typename Titem, typename Tptr = std::unique_ptr<Titem>, typename Tcontainer = std::vector<Tptr>>
class const_ptr_iterator
{
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = const Titem;
    using pointer = const Titem*;
    using reference = const Titem&;

    const_ptr_iterator(Tcontainer::const_iterator it) : _it(it) {}

    reference operator*() const { return **_it; }
    pointer operator->() const { return _it->get(); }
    const_ptr_iterator& operator++() { ++_it; return *this; }
    const_ptr_iterator operator++(int) { const_ptr_iterator it = *this; ++(*this); return it; }
    friend bool operator==(const const_ptr_iterator& a, const const_ptr_iterator& b) noexcept { return a._it == b._it; }
    friend bool operator!=(const const_ptr_iterator& a, const const_ptr_iterator& b) noexcept { return a._it != b._it; }

private:
    Tcontainer::const_iterator _it;
}; // class const_ptr_iterator



} // namespace cl7

#endif // CL7_ITERATOR_H
