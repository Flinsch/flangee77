#ifndef CL7_ORDEREDMAP_H
#define CL7_ORDEREDMAP_H

#include <CoreLabs/iterators.h>

#include <set>
#include <vector>



namespace cl7 {



template <typename Key, typename T, typename KeyView = Key, typename Compare = std::less<KeyView>, typename Allocator = std::allocator<std::pair<Key, T>>>
class ordered_map
{

public:
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<const Key, T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using key_compare = Compare;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;

    using vector_type = std::vector<std::pair<Key, T>, Allocator>;

    using iterator = cl7::continuous_forward_iterator<false, value_type, typename vector_type::value_type>;
    using const_iterator = cl7::continuous_forward_iterator<true, value_type, typename vector_type::value_type>;



    ordered_map()
        : _vector()
        , _compare(&_vector, Compare())
        , _table(_compare)
    {}

    ordered_map(const Compare& comp, const Allocator& alloc = Allocator())
        : _vector(alloc)
        , _compare(&_vector, comp)
        , _table(_compare)
    {}

    explicit ordered_map(const Allocator& alloc)
        : _vector(alloc)
        , _compare(&_vector, Compare())
        , _table(_compare)
    {}

    template <class It>
    ordered_map(It first, It last, const Compare& comp = Compare(), const Allocator& alloc = Allocator())
        : _vector(first, last, alloc)
        , _compare(&_vector, comp)
        , _table(_compare)
    {
        _rebuild_lookup_table();
    }

    template <class It>
    ordered_map(It first, It last, const Allocator& alloc)
        : ordered_map(first, last, Compare(), alloc)
    {}

    ordered_map(const ordered_map& other)
        : ordered_map(other, Allocator())
    {}

    ordered_map(const ordered_map& other, const Allocator& alloc)
        : _vector(other._vector, alloc)
        , _compare(&_vector, Compare())
        , _table(_compare)
    {
        _rebuild_lookup_table();
    }

    ordered_map(ordered_map&& other) //  NOLINT(*-noexcept-move-*)
        : ordered_map(std::move(other), Allocator())
    {}

    ordered_map(ordered_map&& other, const Allocator& alloc) // Is a noexcept variant possible, i.e. without having to rebuild the table?
        : _vector(std::move(other._vector), alloc)
        , _compare(&_vector, other._compare.comp)
        , _table(_compare)
    {
        _rebuild_lookup_table();
    }

    ordered_map(std::initializer_list<typename vector_type::value_type> ilist, const Compare& comp = Compare(), const Allocator& alloc = Allocator())
        : _vector(ilist, alloc)
        , _compare(&_vector, comp)
        , _table(_compare)
    {
        _rebuild_lookup_table();
    }

    ordered_map(std::initializer_list<typename vector_type::value_type> ilist, const Allocator& alloc)
        : ordered_map(ilist, Compare(), alloc)
    {}

    ordered_map& operator=(const ordered_map& other)
    {
        _vector = other._vector;
        _compare.comp = other._compare.comp;
        _table = table_type(_compare);
        _rebuild_lookup_table();
        return *this;
    }

    ordered_map& operator=(ordered_map&& other) // NOLINT(*-noexcept-move-*)
    {
        _vector = std::move(other._vector);
        _compare.comp = other._compare.comp;
        _table = table_type(_compare);
        _rebuild_lookup_table();
        return *this;
    }

    ordered_map& operator=(std::initializer_list<typename vector_type::value_type> ilist)
    {
        _vector = ilist;
        _rebuild_lookup_table();
        return *this;
    }

    ~ordered_map() noexcept = default;

    void swap(ordered_map& other) // NOLINT(*-noexcept-swap)
    {
        _vector.swap(other._vector);
        std::swap(_compare.comp, other._compare.comp);
        _rebuild_lookup_table();
        other._rebuild_lookup_table();
    }



    /** Returns the function that compares keys. */
    key_compare key_comp() const { return _compare.comp; }

    /** Returns the allocator associated with the container. */
    allocator_type get_allocator() const { return _vector.get_allocator(); }



    /** Checks whether the container is empty. */
    bool empty() const noexcept
    {
        assert(_vector.empty() == _table.empty());
        return _vector.empty();
    }

    /** Returns the number of elements. */
    size_type size() const noexcept
    {
        assert(_vector.size() == _table.size());
        return _vector.size();
    }

    /** Returns the maximum possible number of elements. */
    size_type max_size() const noexcept
    {
        return (std::min)((std::min)(_vector.max_size(), _table.max_size()), Lookup::max_index);
    }

    /** Returns the number of elements that can be held in currently allocated storage. */
    size_type capacity() const noexcept
    {
        return _vector.capacity();
    }

    /** Reserves storage. */
    void reserve(size_type new_capacity)
    {
        _vector.reserve(new_capacity);
    }

    /** Reduces memory usage by freeing unused memory. */
    void shrink_to_fit()
    {
        _vector.shrink_to_fit();
    }


    /** Finds element with specific key. */
    iterator find(const KeyView& key)
    {
        auto it = _table.find(Lookup{key});
        if (it == _table.end())
            return end();
        return {_vector.data() + it->index};
    }

    /** Finds element with specific key. */
    const_iterator find(const KeyView& key) const
    {
        auto it = _table.find(Lookup{key});
        if (it == _table.end())
            return cend();
        return {_vector.data() + it->index};
    }

    /** Checks if the container contains element with specific key. */
    bool contains(const KeyView& key) const
    {
        return _table.contains(Lookup{key});
    }



    /** Clears the contents. */
    void clear() noexcept
    {
        _vector.clear();
        _table.clear();
    }

    /** Inserts elements. */
    template <typename Key_, typename T_>
    std::pair<iterator, bool> insert(Key_&& key, T_&& value)
    {
        Key tmp{std::forward<Key_>(key)};
        Lookup lookup{tmp};
        auto it = _table.lower_bound(lookup);
        if (it != _table.end() && !_table.key_comp()(lookup, it->index))
            return _update_and_return_iterator_false(it, std::forward<T_>(value));
        _vector.emplace_back(std::move(tmp), std::forward<T_>(value));
        return _map_after_insert_and_return_iterator_true(it);
    }

    /** Inserts elements. */
    std::pair<iterator, bool> insert(const value_type& value)
    {
        Lookup lookup{value.first};
        auto it = _table.lower_bound(lookup);
        if (it != _table.end() && !_table.key_comp()(lookup, it->index))
            return _update_and_return_iterator_false(it, value.second);
        _vector.emplace_back(value);
        return _map_after_insert_and_return_iterator_true(it);
    }

    /** Inserts elements. */
    std::pair<iterator, bool> insert(value_type&& value)
    {
        Key tmp{std::move(value.first)};
        Lookup lookup{tmp};
        auto it = _table.lower_bound(lookup);
        if (it != _table.end() && !_table.key_comp()(lookup, it->index))
            return _update_and_return_iterator_false(it, std::move(value.second));
        _vector.emplace_back(std::move(tmp), std::move(value.second));
        return _map_after_insert_and_return_iterator_true(it);
    }

    /** Constructs element in-place. */
    template <typename... Args>
        requires(sizeof...(Args) == 1 || sizeof...(Args) == 2)
    std::pair<iterator, bool> emplace(Args&&... args)
    {
        if constexpr (sizeof...(Args) == 2)
        {
            if constexpr (std::is_same_v<allocator_type, std::allocator<std::pair<Key, T>>>)
            {
                value_type value{std::forward<Args>(args)...};
                Lookup lookup{value.first};
                auto it = _table.lower_bound(lookup);
                if (it != _table.end() && !_table.key_comp()(lookup, it->index))
                    return _update_and_return_iterator_false(it, std::move(value.second));
                _vector.emplace_back(std::move(value));
                return _map_after_insert_and_return_iterator_true(it);
            }
        }
        return insert(std::forward<Args>(args)...);
    }

    /** Erases elements. */
    iterator erase(iterator pos)
    {
        const size_t index = reinterpret_cast<typename vector_type::value_type*>(pos.operator->()) - _vector.data();
        if (index >= _vector.size())
            return end();
        _erase(index);
        return {_vector.data() + index};
    }

    /** Erases elements. */
    iterator erase(const_iterator pos)
    {
        const size_t index = reinterpret_cast<const typename vector_type::value_type*>(pos.operator->()) - _vector.data();
        if (index >= _vector.size())
            return end();
        _erase(index);
        return {_vector.data() + index};
    }

    /** Erases elements. */
    size_type erase(const KeyView& key)
    {
        auto it = _table.find(Lookup{key});
        if (it == _table.end())
            return 0;
        _erase(it->index);
        return 1;
    }



    iterator begin() noexcept { return {_vector.data()}; }
    iterator end() noexcept { return {_vector.data() + _vector.size()}; }

    const_iterator begin() const noexcept { return {_vector.data()}; }
    const_iterator end() const noexcept { return {_vector.data() + _vector.size()}; }

    const_iterator cbegin() const noexcept { return {_vector.data()}; }
    const_iterator cend() const noexcept { return {_vector.data() + _vector.size()}; }



    /** Access specified element with bounds checking. */
    T& at(const KeyView& key)
    {
        Lookup lookup{key};
        auto it = _table.find(lookup);
        size_t index = it == _table.end() ? lookup.index : it->index;
        return _vector.at(index).second;
    }

    /** Access specified element with bounds checking. */
    const T& at(const KeyView& key) const
    {
        Lookup lookup{key};
        auto it = _table.find(lookup);
        size_t index = it == _table.end() ? lookup.index : it->index;
        return _vector.at(index).second;
    }



    /** Access or insert specified element. */
    T& operator[](const Key& key)
    {
        Lookup lookup{key};
        auto it = _table.lower_bound(lookup);
        if (it != _table.end() && !_table.key_comp()(lookup, it->index))
            return _vector[it->index].second;
        _vector.emplace_back(key, T{});
        return _map_after_insert_and_return_reference(it);
    }

    /** Access or insert specified element. */
    T& operator[](Key&& key)
    {
        Lookup lookup{key};
        auto it = _table.lower_bound(lookup);
        if (it != _table.end() && !_table.key_comp()(lookup, it->index))
            return _vector[it->index].second;
        _vector.emplace_back(std::move(key), T{});
        return _map_after_insert_and_return_reference(it);
    }



    bool operator==(const ordered_map& other) const noexcept
    {
        if (size() != other.size())
            return false;
        for (auto it = begin(); it != end(); ++it)
        {
            auto it1 = other.find(it->first);
            if (it1 == other.end())
                return false;
            if (it->second != it1->second)
                return false;
        }
        return true;
    }

    bool operator!=(const ordered_map& other) const noexcept { return !(*this == other); }



private:
    static_assert(std::is_same_v<typename Allocator::value_type, typename vector_type::value_type>, "cl7::ordered_map must have the same value_type as its allocator");

    struct Lookup
    {
        static constexpr size_t invalid_index = -1;
        static constexpr size_t max_index = invalid_index - 1;

        static KeyView key;

        size_t index;

        Lookup(int dummy, size_t index) : index(index) {}
        Lookup(KeyView key) : index(invalid_index) { Lookup::key = key; }
    };

    struct LookupCompare
    {
        const vector_type* vector;
        Compare comp;

        LookupCompare(const vector_type* vector, Compare comp) : vector(vector), comp(comp) {}

        bool operator()(const Lookup& a, const Lookup& b) const
        {
            assert(a.index != Lookup::invalid_index || b.index != Lookup::invalid_index);
            KeyView a_key = a.index == Lookup::invalid_index ? a.key : vector->at(a.index).first;
            KeyView b_key = b.index == Lookup::invalid_index ? b.key : vector->at(b.index).first;
            return comp(a_key, b_key);
        }

        bool operator()(const Lookup& a, size_t b_index) const
        {
            assert(a.index == Lookup::invalid_index);
            KeyView b_key = vector->at(b_index).first;
            return comp(a.key, b_key);
        }
    };

    using table_type = std::set<Lookup, LookupCompare>;



    void _rebuild_lookup_table()
    {
        _table.clear();
        for (size_t i = 0; i < _vector.size(); ++i)
            _table.emplace(0, i);
    }

    void _dismantle_partial_table(size_t index)
    {
        for (size_t i = index; i < _vector.size(); ++i)
        {
            auto it = _table.find(Lookup(0, i));
            assert(it != _table.end());
            _table.erase(it);
        }
    }

    void _restore_partial_table(size_t index)
    {
        for (size_t i = index; i < _vector.size(); ++i)
        {
            assert(!_table.contains(Lookup(0, i)));
            _table.emplace(0, i);
        }
    }



    template <typename T_>
    std::pair<iterator, bool> _update_and_return_iterator_false(typename table_type::iterator it, T_&& value)
    {
        _vector[it->index].second = std::forward<T_>(value);
        return {{_vector.data() + it->index}, false};
    }

    std::pair<iterator, bool> _map_after_insert_and_return_iterator_true(typename table_type::iterator it)
    {
        _map_after_insert(it);
        return {{_vector.data() + _vector.size() - 1}, true};
    }

    T& _map_after_insert_and_return_reference(typename table_type::iterator it)
    {
        _map_after_insert(it);
        return _vector.back().second;
    }

    void _map_after_insert(typename table_type::iterator it)
    {
        _table.insert(it, Lookup(0, _vector.size() - 1));
        assert(_vector.size() == _table.size());
    }



    void _erase(size_t index)
    {
        _dismantle_partial_table(index);
        _vector.erase(_vector.begin() + index);
        _restore_partial_table(index);
        assert(_vector.size() == _table.size());
    }



    vector_type _vector;
    LookupCompare _compare;
    table_type _table;

}; // class ordered_map



    template <typename Key, typename T, typename KeyView, typename Compare, typename Allocator>
    KeyView ordered_map<Key, T, KeyView, Compare, Allocator>::Lookup::key = {};



} // namespace cl7

#endif // CL7_ORDEREDMAP_H
