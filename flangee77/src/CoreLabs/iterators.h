#ifndef CL7_ITERATOR_H
#define CL7_ITERATOR_H

#include <CoreLabs/root.h>

#include <vector>



// Some good "guides" on iterators:
// https://cplusplus.com/reference/iterator/
// https://stackoverflow.com/a/8054856/480304
// https://www.studyplan.dev/pro-cpp/iterator-concepts
// https://stackoverflow.com/a/66050521/480304



namespace cl7 {



template <bool is_const, typename Titem, typename Tbase = Titem>
    requires(sizeof(Titem) == sizeof(Tbase) && alignof(Titem) == alignof(Tbase)) // Is this "enough"?
class contiguous_iterator
{
public:
    using iterator_category = std::contiguous_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Titem;
    using reference = std::conditional_t<is_const, const Titem&, Titem&>;
    using pointer = std::conditional_t<is_const, const Titem*, Titem*>;

    using Tptr = std::conditional_t<is_const, const Tbase*, Tbase*>;

    contiguous_iterator(Tptr ptr = nullptr) : _ptr(ptr) {}

    friend void swap(contiguous_iterator& a, contiguous_iterator& b) noexcept { std::swap(a._ptr, b._ptr); }

    contiguous_iterator& operator++() { ++_ptr; return *this; }
    contiguous_iterator operator++(int) { contiguous_iterator it = *this; ++(*this); return it; }
    contiguous_iterator& operator--() { --_ptr; return *this; }
    contiguous_iterator operator--(int) { contiguous_iterator it = *this; --(*this); return it; }

    friend bool operator==(contiguous_iterator a, contiguous_iterator b) noexcept { return a._ptr == b._ptr; }
    friend auto operator<=>(contiguous_iterator a, contiguous_iterator b) noexcept { return a._ptr <=> b._ptr; }

    friend contiguous_iterator operator+(difference_type n, contiguous_iterator it) noexcept { return {it._ptr + n}; }
    friend contiguous_iterator operator+(contiguous_iterator it, difference_type n) noexcept { return {it._ptr + n}; }
    friend contiguous_iterator operator-(contiguous_iterator it, difference_type n) noexcept { return {it._ptr - n}; }
    friend difference_type operator-(contiguous_iterator a, contiguous_iterator b) noexcept { return a._ptr - b._ptr; }
    contiguous_iterator& operator+=(difference_type n) noexcept { _ptr += n; return *this; }
    contiguous_iterator& operator-=(difference_type n) noexcept { _ptr -= n; return *this; }

    reference operator[](size_t n) const { return *reinterpret_cast<pointer>(_ptr + n); }
    reference operator*() const { return *reinterpret_cast<pointer>(_ptr); }
    pointer operator->() const { return reinterpret_cast<pointer>(_ptr); }

    operator contiguous_iterator<true, Titem, Tbase>() const { return {_ptr}; }

private:
    Tptr _ptr;
}; // class contiguous_iterator

static_assert(std::contiguous_iterator<contiguous_iterator<false, int>>);
static_assert(std::contiguous_iterator<contiguous_iterator<true, int>>);



template <typename Titem, typename Tptr = std::unique_ptr<Titem>, typename Tcontainer = std::vector<Tptr>>
class const_ptr_forward_iterator
{
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = const Titem;
    using reference = const Titem&;
    using pointer = const Titem*;

    const_ptr_forward_iterator() = default;
    const_ptr_forward_iterator(Tcontainer::const_iterator it) : _it(it) {}

    friend void swap(const_ptr_forward_iterator& a, const_ptr_forward_iterator& b) noexcept { a._it.swap(b._it); }

    const_ptr_forward_iterator& operator++() { ++_it; return *this; }
    const_ptr_forward_iterator operator++(int) { const_ptr_forward_iterator it = *this; ++(*this); return it; }

    friend bool operator==(const const_ptr_forward_iterator& a, const const_ptr_forward_iterator& b) noexcept { return a._it == b._it; }

    reference operator*() const { return **_it; }
    pointer operator->() const { return _it->get(); }

private:
    Tcontainer::const_iterator _it;
}; // class const_ptr_forward_iterator

static_assert(std::forward_iterator<const_ptr_forward_iterator<int>>);



} // namespace cl7

#endif // CL7_ITERATOR_H
