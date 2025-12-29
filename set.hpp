#ifndef SET_HPP
#define SET_HPP

#include <iostream>  // cout, ostream
#include <ostream>   // ostream
#include <istream>   // istream
#include <fstream>   // ofstream, ifstream
#include <algorithm> // swap
#include <iterator>  // std::forward_iterator_tag
#include <cstddef>   // std::ptrdiff_t
#include <stdexcept> // std::logic_error, std::runtime_error
#include <cassert>   // assert
#include <string>

template <typename T, typename Eql>
class set
{
private:
    T *_set;
    unsigned int _size;

    Eql _eql;

public:
    set() : _set(nullptr), _size(0) {}

    set(const set &other) : _set(nullptr), _size(0)
    {
        if (other._size > 0)
        {
            try
            {
                _set = new T[other._size];
                for (unsigned int i = 0; i < other._size; ++i)
                {
                    _set[i] = other._set[i];
                }
                _size = other._size;
            }
            catch (...)
            {
                clear();
                throw;
            }
        }
    }

    template <typename IterT>
    set(IterT begin, IterT end) : _set(nullptr), _size(0)
    {
        try
        {
            while (begin != end)
            {
                const T &value = static_cast<T>(*begin);
                add(value);
                ++begin;
            }
        }
        catch (...)
        {
            clear();
            throw;
        }
    }

    ~set()
    {
        clear();
    }

    unsigned int size() const
    {
        return _size;
    }

    void add(const T &element)
    {
        if (contains(element))
        {
            return;
        }

        T *copySet = new T[_size + 1];

        try
        {
            for (unsigned int i = 0; i < _size; ++i)
            {
                copySet[i] = _set[i];
            }

            copySet[_size] = element;
        }
        catch (...)
        {
            delete[] copySet;
            throw;
        }

        delete[] _set;

        _set = copySet;
        ++_size;
    }

    void remove(const T &element)
    {
        if (!contains(element))
        {
            return;
        }

        if (_size == 1)
        {
            clear();
            return;
        }

        T *copySet = new T[_size - 1];

        try
        {
            for (unsigned int i = 0, j = 0; i < _size; ++i)
            {
                if (_eql(_set[i], element))
                {
                    continue;
                }

                copySet[j] = _set[i];
                ++j;
            }
        }
        catch (...)
        {
            delete[] copySet;
            throw;
        }

        delete[] _set;

        _set = copySet;
        --_size;
    }

    bool contains(const T &element) const
    {
        for (unsigned int i = 0; i < _size; ++i)
        {
            if (_eql(_set[i], element))
            {
                return true;
            }
        }

        return false;
    }

    set &operator=(const set &other)
    {
        if (this != &other)
        {
            set tmp(other);

            std::swap(_set, tmp._set);
            std::swap(_size, tmp._size);
        }

        return *this;
    }

    const T &operator[](unsigned int i) const
    {
        assert(i < _size);
        return _set[i];
    }

    bool operator==(const set &other) const
    {
        if (_size != other._size)
        {
            return false;
        }

        for (unsigned int i = 0; i < _size; ++i)
        {
            if (!other.contains(_set[i]))
            {
                return false;
            }
        }

        return true;
    }

    set operator-(const set &other) const
    {
        set result;

        for (unsigned int i = 0; i < _size; ++i)
        {
            if (other.contains(_set[i]))
            {
                result.add(_set[i]);
            }
        }

        return result;
    }

    class const_iterator
    {
    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef const T *pointer;
        typedef const T &reference;

        const_iterator() : _t(nullptr) {}

        const_iterator(const const_iterator &other) : _t(other._t) {}

        const_iterator &operator=(const const_iterator &other)
        {
            _t = other._t;
            return *this;
        }

        ~const_iterator() {}

        reference operator*() const
        {
            return *_t;
        }

        pointer operator->() const
        {
            return _t;
        }

        const_iterator operator++(int)
        {
            const_iterator tmp(*this);
            ++_t;
            return tmp;
        }

        const_iterator &operator++()
        {
            ++_t;
            return *this;
        }

        bool operator==(const const_iterator &other) const
        {
            return _t == other._t;
        }

        bool operator!=(const const_iterator &other) const
        {
            return _t != other._t;
        }

    private:
        const T *_t;

        friend class set;

        const_iterator(const T *t) : _t(t) {}
    }; // const_iterator

    const_iterator begin() const
    {
        return const_iterator(_set);
    }

    const_iterator end() const
    {
        return const_iterator(_set + _size);
    }

private:
    void clear()
    {
        delete[] _set;
        _set = nullptr;
        _size = 0;
    }
}; // set

template <typename T, typename Eql>
std::ostream &operator<<(std::ostream &os, const set<T, Eql> &s)
{
    typename set<T, Eql>::const_iterator i, ie;

    i = s.begin();
    ie = s.end();

    os << "{";

    while (i != ie)
    {
        os << *i;
        i++;

        if (i != ie)
        {
            os << ", ";
        }
    }

    os << "}";

    return os;
}

template <typename T, typename Eql, typename P>
set<T, Eql> filter_out(const set<T, Eql> &S, P pred)
{
    set<T, Eql> result;

    typename set<T, Eql>::const_iterator i, ie;
    i = S.begin();
    ie = S.end();

    while (i != ie)
    {
        if (pred(*i))
        {
            result.add(*i);
        }

        i++;
    }
    return result;
}

template <typename T, typename Eql>
set<T, Eql> operator+(const set<T, Eql> &left, const set<T, Eql> &right)
{
    set<T, Eql> result = left;

    typename set<T, Eql>::const_iterator i, ie;
    i = right.begin();
    ie = right.end();

    while (i != ie)
    {
        result.add(*i);
        i++;
    }

    return result;
}

template <typename T, typename Eql>
void save(const set<T, Eql> &s, const std::string &filename)
{
    std::ofstream ofs(filename);
    if (!ofs.is_open())
    {
        throw std::runtime_error("File can't be opened!");
    }

    ofs << s.size() << std::endl;

    typename set<T, Eql>::const_iterator i, ie;
    i = s.begin();
    ie = s.end();

    while (i != ie)
    {
        ofs << *i << std::endl;
        i++;
    }
    ofs.close();
}

template <typename T, typename Eql>
void load(const std::string &filename, set<T, Eql> &s)
{
    std::ifstream ifs(filename);
    if (!ifs.is_open())
    {
        throw std::runtime_error("File can't be opened!");
    }

    set<T, Eql> temp;
    unsigned int count;
    ifs >> count;

    T val;
    while (count > 0)
    {
        ifs >> val;
        temp.add(val);

        --count;
    }

    s = temp;
    ifs.close();
}
#endif