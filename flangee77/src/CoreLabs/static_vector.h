#ifndef CL7_STATICVECTOR_H
#define CL7_STATICVECTOR_H

#include <CoreLabs/iterators.h>

#include <array>
#include <initializer_list>
#include <stdexcept>



namespace cl7 {



template<typename T, std::size_t N>
class static_vector
{

public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    using iterator = cl7::contiguous_iterator<false, value_type>;
    using const_iterator = cl7::contiguous_iterator<true, value_type>;

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;



    static_vector() = default;

    template <class It>
    static_vector(It first, It last)
    {
        for (It it = first; it != last; ++it)
            push_back(*it);
    }

    static_vector(std::initializer_list<T> ilist)
    {
        if (ilist.size() > N)
            throw std::length_error("static_vector initializer_list too large");
        for (const T& val : ilist)
            _data[_size++] = val;
    }

    static_vector& operator=(std::initializer_list<T> ilist)
    {
        if (ilist.size() > N)
            throw std::length_error("static_vector initializer_list too large");
        clear();
        for (const T& val : ilist)
            _data[_size++] = val;
        return *this;
    }

    void swap(static_vector& other) noexcept
    {
        _data.swap(other._data);
        std::swap(_size, other._size);
    }



    /** Checks whether the container is empty. */
    constexpr bool empty() const noexcept { return _size == 0; }

    /** Checks whether the container is full. */
    constexpr bool full() const noexcept { return _size == N; }

    /** Returns the number of elements. */
    constexpr size_type size() const noexcept { return _size; }

    /** Returns the maximum possible number of elements. */
    constexpr size_type max_size() const noexcept { return _data.max_size(); }

    /** Returns the number of elements that can be held in storage. */
    static constexpr size_type capacity() noexcept { return N; }


    /** Clears the contents. */
    void clear() noexcept
    {
        while (_size > 0)
            _data[--_size] = {};
    }

    /** Adds an element to the end. */
    void push_back(const T& value)
    {
        if (full())
            throw std::out_of_range("static_vector::push_back() full");
        _data[_size++] = value;
    }

    /** Adds an element to the end. */
    void push_back(T&& value)
    {
        if (full())
            throw std::out_of_range("static_vector::push_back() full");
        _data[_size++] = std::move(value);
    }

    /** Constructs an element in-place at the end. */
    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        if (full())
            throw std::out_of_range("static_vector::emplace_back() full");
        _data[_size++] = T(std::forward<Args>(args)...);
    }

    /** Removes the last element. */
    void pop_back()
    {
        if (empty())
            throw std::out_of_range("static_vector::pop_back() empty");
        _data[--_size] = {};
    }

    /** Inserts elements. */
    iterator insert(const_iterator pos, const T& value)
    {
        if (full())
            throw std::out_of_range("static_vector::insert() full");
        const size_t index = reinterpret_cast<const value_type*>(pos.operator->()) - _data.data();

        for (size_type i = _size; i > index; --i)
            _data[i] = std::move(_data[i - 1]);

        _data[index] = value;
        ++_size;
        return _data.begin() + index;
    }

    /** Inserts elements. */
    iterator insert(const_iterator pos, T&& value)
    {
        if (full())
            throw std::out_of_range("static_vector::insert() full");
        const size_t index = reinterpret_cast<const value_type*>(pos.operator->()) - _data.data();
        if (index > _size)
            throw std::out_of_range("static_vector::insert() out of range");

        for (size_type i = _size; i > index; --i)
            _data[i] = std::move(_data[i - 1]);

        _data[index] = std::move(value);
        ++_size;
        return begin() + index;
    }

    /** Erases elements. */
    iterator erase(const_iterator pos)
    {
        const size_t index = reinterpret_cast<const value_type*>(pos.operator->()) - _data.data();
        if (index >= _size)
            throw std::out_of_range("static_vector::erase() out of range");

        for (size_type i = index; i + 1 < _size; ++i)
            _data[i] = std::move(_data[i + 1]);

        _data[--_size] = {};
        return begin() + index;
    }

    /** Changes the number of elements stored. */
    void resize(size_type new_size, const T& default_value = T())
    {
        if (new_size > N)
            throw std::out_of_range("static_vector::resize() too large");

        if (new_size > _size)
        {
            for (size_type i = _size; i < new_size; ++i)
                _data[i] = default_value;
        }
        else if (new_size < _size)
        {
            for (size_type i = _size; i > new_size; --i)
                _data[i - 1] = {};
        }
        _size = new_size;
    }


    /** Access the first element. */
    T& front()
    {
        if (empty())
            throw std::out_of_range("static_vector::front()");
        return _data[0];
    }

    /** Access the first element. */
    const T& front() const
    {
        if (empty())
            throw std::out_of_range("static_vector::front()");
        return _data[0];
    }

    /** Access the last element. */
    T& back()
    {
        if (empty())
            throw std::out_of_range("static_vector::back()");
        return _data[_size - 1];
    }

    /** Access the last element. */
    const T& back() const
    {
        if (empty())
            throw std::out_of_range("static_vector::back()");
        return _data[_size - 1];
    }



    /** Direct access to the underlying contiguous storage. */
    T* data()
    {
        return _data.data();
    }

    /** Direct access to the underlying contiguous storage. */
    const T* data() const
    {
        return _data.data();
    }



    iterator begin() noexcept { return {_data.data()}; }
    const_iterator begin() const noexcept { return {_data.data()}; }
    const_iterator cbegin() const noexcept { return {_data.data()}; }

    iterator end() noexcept { return {_data.data() + _size}; }
    const_iterator end() const noexcept { return {_data.data() + _size}; }
    const_iterator cend() const noexcept { return {_data.data() + _size}; }

    reverse_iterator rbegin() noexcept { return std::reverse_iterator<iterator>(end()); }
    const_reverse_iterator rbegin() const noexcept { return std::reverse_iterator<const_iterator>(end()); }
    const_reverse_iterator crbegin() const noexcept { return std::reverse_iterator<const_iterator>(end()); }

    reverse_iterator rend() noexcept { return std::reverse_iterator<iterator>(begin()); }
    const_reverse_iterator rend() const noexcept { return std::reverse_iterator<const_iterator>(begin()); }
    const_reverse_iterator crend() const noexcept { return std::reverse_iterator<const_iterator>(begin()); }



    /** Access specified element with bounds checking. */
    T& at(size_type i)
    {
        if (i >= _size)
            throw std::out_of_range("static_vector::at()");
        return _data[i];
    }

    /** Access specified element with bounds checking. */
    const T& at(size_type i) const
    {
        if (i >= _size)
            throw std::out_of_range("static_vector::at()");
        return _data[i];
    }



    /** Access specified element. */
    T& operator[](size_type i)
    {
        return _data[i];
    }

    /** Access specified element. */
    const T& operator[](size_type i) const
    {
        return _data[i];
    }



    bool operator==(const static_vector& other) const noexcept
    {
        if (_size != other._size)
            return false;
        for (size_type i = 0; i < _size; ++i)
        {
            if (_data[i] != other._data[i])
                return false;
        }
        return true;
    }

    bool operator!=(const static_vector& other) const noexcept { return !(*this == other); }



private:
    std::array<T, N> _data;
    size_type _size = 0;

}; // class static_vector



} // namespace cl7

#endif // CL7_STATICVECTOR_H
