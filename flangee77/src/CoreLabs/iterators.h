#ifndef CL7_ITERATOR_H
#define CL7_ITERATOR_H

#include <CoreLabs/root.h>

#include <vector>



namespace cl7 {



template <bool is_const, typename Titem, typename Tbase = Titem>
    requires(sizeof(Titem) == sizeof(Tbase) && alignof(Titem) == alignof(Tbase)) // Is this "enough"?
class continuous_forward_iterator
{
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Titem;
    using pointer = std::conditional_t<is_const, const Titem*, Titem*>;
    using reference = std::conditional_t<is_const, const Titem&, Titem&>;

    using Tptr = std::conditional_t<is_const, const Tbase*, Tbase*>;

    continuous_forward_iterator(Tptr ptr) : _ptr(ptr) {}

    reference operator*() const { return *reinterpret_cast<pointer>(_ptr); }
    pointer operator->() const { return reinterpret_cast<pointer>(_ptr); }
    continuous_forward_iterator& operator++() { ++_ptr; return *this; }
    continuous_forward_iterator operator++(int) { continuous_forward_iterator it = *this; ++(*this); return it; }
    friend bool operator==(const continuous_forward_iterator& a, const continuous_forward_iterator& b) noexcept { return a._ptr == b._ptr; }
    friend bool operator!=(const continuous_forward_iterator& a, const continuous_forward_iterator& b) noexcept { return !(a == b); }

private:
    Tptr _ptr;
}; // class continuous_forward_iterator



template <typename Titem, typename Tptr = std::unique_ptr<Titem>, typename Tcontainer = std::vector<Tptr>>
class const_ptr_forward_iterator
{
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = const Titem;
    using pointer = const Titem*;
    using reference = const Titem&;

    const_ptr_forward_iterator(typename Tcontainer::const_iterator it) : _it(it) {}

    reference operator*() const { return **_it; }
    pointer operator->() const { return _it->get(); }
    const_ptr_forward_iterator& operator++() { ++_it; return *this; }
    const_ptr_forward_iterator operator++(int) { const_ptr_forward_iterator it = *this; ++(*this); return it; }
    friend bool operator==(const const_ptr_forward_iterator& a, const const_ptr_forward_iterator& b) noexcept { return a._it == b._it; }
    friend bool operator!=(const const_ptr_forward_iterator& a, const const_ptr_forward_iterator& b) noexcept { return !(a == b); }

private:
    typename Tcontainer::const_iterator _it;
}; // class const_ptr_forward_iterator



} // namespace cl7

#endif // CL7_ITERATOR_H
