How to use std::sort with a custom container and iterators
==========================================================

(Much of the below was learned from [this link](https://github.com/navyenzo/blIteratorAPI))

Implementing support for algorithms like std::sort is not as simple as might
be imagined. After all, it is C++ :)

For a start, you need to implement your own iterator that can be used to
walk your container. There are 4 types of iterators that we need to support
std::sort

- normal, random access
- constant, random access
- reverse, random access
- reverse constant, random access

Each iterator needs its own implementation, but to avoid this we can use
a templatized iterator e.g.:
```C++
    template<typename T> class MyIterator { ... }
    template<typename T> class MyReverseIterator { ... }
```
And then later we can save some typing by having the const and non const
versions be instantiations of this template e.g.:
```C++
    typedef MyIterator<T>              iterator;
    typedef MyIterator<const T>        const_iterator;
    typedef MyReverseIterator<T>       reverse_iterator;
    typedef MyReverseIterator<const T> const_reverse_iterator;
```
Note that we have one template each for forward and reverse iterators.
The only real difference is that operations like ++ will increment in the
forward container and decrement in the reverse container e.g.:
```C++
    MyIterator<T>        operator++ (int) { auto temp(*this); ++dataptr; return temp; }
    MyReverseIterator<T> operator++ (int) { auto temp(*this); --dataptr; return temp; }
```
We must also provide all operators for moving around the 
container, +,-,++,--,==.!= etc...:
```C++
    MyReverseIterator<T> operator+ (const difference_type& delta) { auto oldptr = dataptr; dataptr-=delta; auto temp(*this); dataptr = oldptr; return temp; }
    MyReverseIterator<T> operator+ (int delta) { auto oldptr = dataptr; dataptr-=delta; auto temp(*this); dataptr = oldptr; return temp; }
    MyReverseIterator<T> operator++ (int) { auto temp(*this); --dataptr; return temp; }
    MyReverseIterator<T> operator- (const difference_type& delta) { auto oldptr = dataptr; dataptr+=delta; auto temp(*this); dataptr = oldptr; return temp; }
    MyReverseIterator<T> operator- (int delta) { auto oldptr = dataptr; dataptr+=delta; auto temp(*this); dataptr = oldptr; return temp; }
    MyReverseIterator<T> operator-- (int) { auto temp(*this); ++dataptr; return temp; }
    MyReverseIterator<T>& operator++ () { --dataptr; return (*this); }
    MyReverseIterator<T>& operator+= (const difference_type& delta) { dataptr -= delta; return (*this); }
    MyReverseIterator<T>& operator-- () { ++dataptr; return (*this); }
    MyReverseIterator<T>& operator-= (const difference_type& delta) { dataptr += delta; return (*this); }
    MyReverseIterator<T>& operator= (T* ptr) { dataptr = ptr; return (*this); }
    MyReverseIterator<T>& operator= (const MyReverseIterator<T>& it) = default;
    T& operator*() { return *dataptr; }
    T* operator->() { return dataptr; }
    bool operator!=(const MyReverseIterator<T>& it) const { return (dataptr != it.get_const_ptr()); }
    bool operator==(const MyReverseIterator<T>& it) const { return (dataptr == it.get_const_ptr()); }
```
One other note, is that the STL will expect the following traits to be 
provided within your iterator:
```C++
template<typename T> class MyIterator {
    ...
    // typedef typename _Iter::difference_type   difference_type;
    // typedef typename _Iter::value_type        value_type;
    // typedef typename _Iter::pointer           pointer;
    // typedef typename _Iter::reference         reference;
    // typedef typename _Iter::iterator_category iterator_category;
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
```
Now we are able to use MyIterator in our own container. To do so we must
first instantiate the templates e.g.:
```C++
template<typename T> class MyVector {
    ...
    typedef MyIterator<T>              iterator;
    typedef MyIterator<const T>        const_iterator;
    typedef MyReverseIterator<T>       reverse_iterator;
    typedef MyReverseIterator<const T> const_reverse_iterator;
```
And then also provide the normal container traversing functions that we
expect:
```C++
    iterator begin() { return iterator(&dataptr[0]); }
    iterator end() { return iterator(&dataptr[len]); }
    const_iterator cbegin() { return const_iterator(&dataptr[0]); }
    const_iterator cend() { return const_iterator(&dataptr[len]); }
    reverse_iterator rbegin() { return reverse_iterator(&dataptr[len - 1]); }
    reverse_iterator rend() { return reverse_iterator(&dataptr[-1]); }
    const_reverse_iterator crbegin() { return const_reverse_iterator(&dataptr[len - 1]); }
    const_reverse_iterator crend() { return const_reverse_iterator(&dataptr[-1]); }
```
Now, finally, that we have done all that we are able to sort and walk our 
custom container e.g.:
```C++
    std::initializer_list< std::string > init1 = {
        "zaphod",
        "universe",
        "arthur",
        "marvin",
        "mice",
        "vogon"
    };

    MyVector< std::string > vec1(init1);

    std::sort(vec1.begin(), vec1.end(), [](const std::string &a, const std::string &b) {
        return a < b;
    });
```
Here is a full example.
```C++
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

template<typename T> class MyIterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    MyIterator(T* ptr = nullptr) { dataptr = ptr; }
    MyIterator(const MyIterator<T>& it) = default;
    MyIterator<T> base() { MyIterator<T> forwardIterator(this->dataptr); ++forwardIterator; return forwardIterator; }
    MyIterator<T> operator+ (const difference_type& delta) { auto oldptr = dataptr; dataptr+=delta; auto temp(*this); dataptr = oldptr; return temp; }
    MyIterator<T> operator+ (int delta) { auto oldptr = dataptr; dataptr+=delta; auto temp(*this); dataptr = oldptr; return temp; }
    MyIterator<T> operator++ (int) { auto temp(*this); ++dataptr; return temp; }
    MyIterator<T> operator- (const difference_type& delta) { auto oldptr = dataptr; dataptr-=delta; auto temp(*this); dataptr = oldptr; return temp; }
    MyIterator<T> operator- (int delta) { auto oldptr = dataptr; dataptr-=delta; auto temp(*this); dataptr = oldptr; return temp; }
    MyIterator<T> operator-- (int) { auto temp(*this); --dataptr; return temp; }
    MyIterator<T>& operator++ () { ++dataptr; return (*this); }
    MyIterator<T>& operator+= (const difference_type& delta) { dataptr += delta; return (*this); }
    MyIterator<T>& operator-- () { --dataptr; return (*this); }
    MyIterator<T>& operator-= (const difference_type& delta) { dataptr -= delta; return (*this); }
    MyIterator<T>& operator= (T* ptr) { dataptr = ptr; return (*this); }
    MyIterator<T>& operator= (const MyIterator<T>& it) = default;
    T& operator*() { return *dataptr; }
    T* get_ptr() const { return dataptr; }
    T* operator->() { return dataptr; }
    bool operator!=(const MyIterator<T>& it) const { return (dataptr != it.get_const_ptr()); }
    bool operator==(const MyIterator<T>& it) const { return (dataptr == it.get_const_ptr()); }
    const T& operator*() const { return *dataptr; }
    const T* get_const_ptr() const { return dataptr; }
    difference_type operator-(const MyIterator<T>& it) { return std::distance(it.get_ptr(), this->get_ptr()); }
    operator bool() const { return dataptr != nullptr; }
    ~MyIterator() { }
protected:
    T* dataptr; // can be used by parent class, but not publically
};

template<typename T> class MyReverseIterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    MyReverseIterator(T* ptr = nullptr) { dataptr = ptr; }
    MyReverseIterator(const MyReverseIterator<T>& it) = default;
    MyReverseIterator<T> base() { MyReverseIterator<T> forwardIterator(this->dataptr); ++forwardIterator; return forwardIterator; }
    MyReverseIterator<T> operator+ (const difference_type& delta) { auto oldptr = dataptr; dataptr-=delta; auto temp(*this); dataptr = oldptr; return temp; }
    MyReverseIterator<T> operator+ (int delta) { auto oldptr = dataptr; dataptr-=delta; auto temp(*this); dataptr = oldptr; return temp; }
    MyReverseIterator<T> operator++ (int) { auto temp(*this); --dataptr; return temp; }
    MyReverseIterator<T> operator- (const difference_type& delta) { auto oldptr = dataptr; dataptr+=delta; auto temp(*this); dataptr = oldptr; return temp; }
    MyReverseIterator<T> operator- (int delta) { auto oldptr = dataptr; dataptr+=delta; auto temp(*this); dataptr = oldptr; return temp; }
    MyReverseIterator<T> operator-- (int) { auto temp(*this); ++dataptr; return temp; }
    MyReverseIterator<T>& operator++ () { --dataptr; return (*this); }
    MyReverseIterator<T>& operator+= (const difference_type& delta) { dataptr -= delta; return (*this); }
    MyReverseIterator<T>& operator-- () { ++dataptr; return (*this); }
    MyReverseIterator<T>& operator-= (const difference_type& delta) { dataptr += delta; return (*this); }
    MyReverseIterator<T>& operator= (T* ptr) { dataptr = ptr; return (*this); }
    MyReverseIterator<T>& operator= (const MyReverseIterator<T>& it) = default;
    T& operator*() { return *dataptr; }
    T* get_ptr() const { return dataptr; }
    T* operator->() { return dataptr; }
    bool operator!=(const MyReverseIterator<T>& it) const { return (dataptr != it.get_const_ptr()); }
    bool operator==(const MyReverseIterator<T>& it) const { return (dataptr == it.get_const_ptr()); }
    const T& operator*() const { return *dataptr; }
    const T* get_const_ptr() const { return dataptr; }
    difference_type operator-(const MyReverseIterator<T>& it) { return std::distance(it.get_ptr(), this->get_ptr()); }
    operator bool() const { return dataptr != nullptr; }
    ~MyReverseIterator() { }
protected:
    T* dataptr; // can be used by parent class, but not publically
};

template<typename T> class MyVector {
private:
    T *dataptr {};
    size_t maxlen {};
    size_t len {};
public:
    typedef MyIterator<T>        iterator;
    typedef MyIterator<const T>  const_iterator;
    typedef MyReverseIterator<T>       reverse_iterator;
    typedef MyReverseIterator<const T> const_reverse_iterator;

    iterator begin() { return iterator(&dataptr[0]); }
    iterator end() { return iterator(&dataptr[len]); }
    const_iterator cbegin() { return const_iterator(&dataptr[0]); }
    const_iterator cend() { return const_iterator(&dataptr[len]); }
    reverse_iterator rbegin() { return reverse_iterator(&dataptr[len - 1]); }
    reverse_iterator rend() { return reverse_iterator(&dataptr[-1]); }
    const_reverse_iterator crbegin() { return const_reverse_iterator(&dataptr[len - 1]); }
    const_reverse_iterator crend() { return const_reverse_iterator(&dataptr[-1]); }

    MyVector<T> () : dataptr (nullptr), maxlen(0), len(0) {
        std::cout << "default constructor " << to_string() << std::endl;
    }
    MyVector<T> (int maxlen) : dataptr (new T [maxlen]),
                               maxlen(maxlen),
                               len(0) {
        std::cout << "new " << to_string() << std::endl;
    }
    MyVector<T> (const MyVector& o) {
        std::cout << "copy constructor called for " << o.to_string() << std::endl;
        dataptr = new T [o.maxlen];
        maxlen = o.maxlen;
        len = o.len;
        std::copy(o.dataptr, o.dataptr + o.maxlen, dataptr);
        std::cout << "copy constructor result is  " << to_string() << std::endl;
    }
    MyVector<T> (MyVector<T>&& o) {
        std::cout << "std::move called for " << o.to_string() << std::endl;
        dataptr = o.dataptr;
        maxlen = o.maxlen;
        len = o.len;
        o.dataptr = nullptr;
        o.maxlen = 0;
        o.len = 0;
        std::cout << "std::move result is  " << to_string() << std::endl;
    }
    ~MyVector<T> () {
        std::cout << "delete " << to_string() << std::endl;
    }
    MyVector(std::initializer_list<T> &l) {
        for (auto i : l) {
            push_back(i);
        }
    }
    void push_back (const T& i) {
        if (!len) {
            maxlen = 1;
            dataptr = new T [maxlen];
        } else if (len >= maxlen) {
            maxlen *= 2;
            auto newdata = new T [maxlen];
            std::copy(dataptr, dataptr + len, newdata);
            if (dataptr) {
                delete[] dataptr;
            }
            dataptr = newdata;
        }
        dataptr[len++] = i;
        std::cout << "push_back called " << to_string() << std::endl;
    }
    friend std::ostream& operator<<(std::ostream &os, const MyVector<T>& o) {
        auto s = o.dataptr;
        auto e = o.dataptr + o.len;;
        while (s < e) {
            os << "[" << *s << "]";
            s++;
        }
        return os;
    }
    std::string to_string (void) const {
        auto address = static_cast<const void*>(this);
        std::stringstream ss;
        ss << address;

        std::string elems;
        auto s = dataptr;
        auto e = dataptr + len;;
        while (s < e) {
            elems += *s;
            s++;
            if (s < e) {
                elems += ",";
            }
        }

        return "MyVector(" + ss.str() +
                         ", len=" + std::to_string(len) +
                         ", maxlen=" + std::to_string(maxlen) +
                         " elems=[" + elems + "])";
    }
};

int main() {
    // Create a std::initializer_list of std::string:
    std::initializer_list< std::string > init1 = {
        "zaphod",
        "universe",
        "arthur",
        "marvin",
        "mice",
        "vogon"
    };

    // Assign this initializer_list to a vector:
    MyVector< std::string > vec1(init1);

    // Pre sort:
    for (const auto &i : vec1) {
        std::cout << i << std::endl;
    }

    std::sort(vec1.begin(), vec1.end(), [](const std::string &a, const std::string &b) {
        return a < b;
    });

    // Post sort:
    for (const auto &i : vec1) {
        std::cout << i << std::endl;
    }

    std::sort(vec1.rbegin(), vec1.rend(), [](const std::string &a, const std::string &b) {
        return a < b;
    });

    // Reverse sort:
    for (const auto &i : vec1) {
        std::cout << i << std::endl;
    }

    // End:
}
```
To build:
<pre>
cd std_sort_with_custom_iterators
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# Create a std::initializer_list of std::string:

# Assign this initializer_list to a vector:
push_back called MyVector(0x7ffeedc733e0, len=1, maxlen=1 elems=[zaphod])
push_back called MyVector(0x7ffeedc733e0, len=2, maxlen=2 elems=[zaphod,universe])
push_back called MyVector(0x7ffeedc733e0, len=3, maxlen=4 elems=[zaphod,universe,arthur])
push_back called MyVector(0x7ffeedc733e0, len=4, maxlen=4 elems=[zaphod,universe,arthur,marvin])
push_back called MyVector(0x7ffeedc733e0, len=5, maxlen=8 elems=[zaphod,universe,arthur,marvin,mice])
push_back called MyVector(0x7ffeedc733e0, len=6, maxlen=8 elems=[zaphod,universe,arthur,marvin,mice,vogon])

# Pre sort:
zaphod
universe
arthur
marvin
mice
vogon

# Post sort:
arthur
marvin
mice
universe
vogon
zaphod

# Reverse sort:
zaphod
vogon
universe
mice
marvin
arthur

# End:
delete MyVector(0x7ffeedc733e0, len=6, maxlen=8 elems=[zaphod,vogon,universe,mice,marvin,arthur])
</pre>
