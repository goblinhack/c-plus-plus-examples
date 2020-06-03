How to use a range-based for loop with your own class iterator
==============================================================

By now you will have seen the following range-based for loop e.g.:
```C++
    for (i : container) { }
```
Now it would be nice to apply that to our own classes too. To do this,
you need to define something that can hold where we are in the processing
loop, called an iterator e.g.:
```C++
    class Iterator {
    public:
        Iterator(T* ptr) : ptr(ptr) {}
        Iterator operator++() { ++ptr; return *this; }
        bool operator!= (const Iterator& o) const {
            return ptr != o.ptr;
        }
        const T& operator*() const { return *ptr; }
   private:
       T* ptr;
   };
public:
   Iterator begin() const { return Iterator(data); }
   Iterator end() const { return Iterator(data + len); }
```
Notice that the iterator has some private data that we increment as
we walk.

Here is a full example:
```C++
#include <iostream>
#include <sstream>
#include <algorithm>

template<class T> class MyVector {
private:
    T *data {};
    size_t maxlen {};
    size_t currlen {};
public:
    MyVector<T> () : data (nullptr), maxlen(0), currlen(0) {
        std::cout << "default constructor " << to_string() << std::endl;
    }
    MyVector<T> (int maxlen) : data (new T [maxlen]),
                               maxlen(maxlen),
                               currlen(0) {
        std::cout << "new " << to_string() << std::endl;
    }
    MyVector<T> (const MyVector& o) {
        std::cout << "copy constructor called for " << o.to_string() << std::endl;
        data = new T [o.maxlen];
        maxlen = o.maxlen;
        currlen = o.currlen;
        std::copy(o.data, o.data + o.maxlen, data);
        std::cout << "copy constructor result is  " << to_string() << std::endl;
    }
    MyVector<T> (MyVector<T>&& o) {
        std::cout << "std::move called for " << o.to_string() << std::endl;
        data = o.data;
        maxlen = o.maxlen;
        currlen = o.currlen;
        o.data = nullptr;
        o.maxlen = 0;
        o.currlen = 0;
        std::cout << "std::move result is  " << to_string() << std::endl;
    }
    ~MyVector<T> () {
        std::cout << "delete " << to_string() << std::endl;
    }
    void push_back (const T& i) {
        if (currlen >= maxlen) {
            maxlen *= 2;
            auto newdata = new T [maxlen];
            std::copy(data, data + currlen, newdata);
            if (data) {
                delete[] data;
            }
            data = newdata;
        }
        data[currlen++] = i;
        std::cout << "push_back called " << to_string() << std::endl;
    }
    friend std::ostream& operator<<(std::ostream &os, const MyVector<T>& o) {
        auto s = o.data;
        auto e = o.data + o.currlen;;
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
        auto s = data;
        auto e = data + currlen;;
        while (s < e) {
            elems += std::to_string(*s);
            s++;
            if (s < e) {
                elems += ",";
            }
        }

        return "MyVector(" + ss.str() +
                         ", currlen=" + std::to_string(currlen) +
                         ", maxlen=" + std::to_string(maxlen) +
                         " elems=[" + elems + "])";
    }
    class Iterator {
    public:
        Iterator(T* ptr) : ptr(ptr) {}
        Iterator operator++() { ++ptr; return *this; }
        bool operator!= (const Iterator& o) const {
            return ptr != o.ptr;
        }
        const T& operator*() const { return *ptr; }
   private:
       T* ptr;
   };
public:
   Iterator begin() const {
       return Iterator(data);
   }
   Iterator end() const {
       return Iterator(data + currlen);
   }
};

int main() {
    // Create a custom vector class:
    auto vec1 = MyVector<int>(1);
    vec1.push_back(10);
    vec1.push_back(11);
    vec1.push_back(12);

    // Walk the custom vector with our iterator:
    for (auto i : vec1) {
        std::cout << "vec1: walk " << i << std::endl;
    }

    // End, expect vec1 destroy:
}
```
To build:
<pre>
cd range_based_for_loop_custom_begin_end
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# Create a custom vector class:
new MyVector(0x7ffee5db46f0, currlen=0, maxlen=1 elems=[])
push_back called MyVector(0x7ffee5db46f0, currlen=1, maxlen=1 elems=[10])
push_back called MyVector(0x7ffee5db46f0, currlen=2, maxlen=2 elems=[10,11])
push_back called MyVector(0x7ffee5db46f0, currlen=3, maxlen=4 elems=[10,11,12])

# Walk the custom vector with our iterator:
vec1: walk 10
vec1: walk 11
vec1: walk 12

# End, expect vec1 destroy:
delete MyVector(0x7ffee5db46f0, currlen=3, maxlen=4 elems=[10,11,12])
</pre>
