How to use std::move to avoid the cost of temporaries
=====================================================

std::move is a nifty way to avoid the use of temporaries when doing copies
of data. For example if you do:

```C++
template<class T> void simpleswap(T& a, T& b) {
    T tmp = b;
    b = a;
    a = b;
}
```

You will see the copy constructor invoked. Not a huge deal for small classes,
but if you are dealing with a vector, that could be very expensive.

A better approach is to use std::move e.g.

```C++
template<class T> void simpleswap(T& a, T& b) {
    T tmp { std::move(a) };
    a = std::move(b);
    b = std::move(tmp);
}
```

Now the question is, how do we implement std::move on our own classes.
To use this you will see a new form of constructor with the "&&" syntax.
The example below will create our own vector class and then implement
the move constructor to transfer elements across. The old vector will
be left empty.

Note that std::vector already does this approach; we're just applying
the same ideas to a custom class.
```C++
#include <iostream>
#include <string>
#include <sstream> // std::stringstream
#include <algorithm> // std::move

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
};

int main() {
    // Create a custom vector class:
    auto vec1 = new MyVector<int>(1);
    vec1->push_back(10);
    vec1->push_back(11);
    std::cout << "vec1: " << *vec1 << std::endl;

    // Create a new copy of vec1, vec2 via copy constructor (&):
    auto vec2 = *vec1;
    std::cout << "vec2: " << vec2 << std::endl;

    // Check we can append onto the copied vector:
    vec2.push_back(12);
    vec2.push_back(13);
    std::cout << "vec2: " << vec2 << std::endl;

    // Create a new vector from vec1, vec3 via the move constructor (&&):
    auto vec3 = std::move(*vec1);
    std::cout << "vec3: " << vec3 << std::endl;

    // Check we can append onto the std:move'd vector:
    vec3.push_back(14);
    vec3.push_back(15);
    std::cout << "vec3: " << vec3 << std::endl;

    // Destroy the old vector, vec1. It has no invalid elems:
    delete vec1;

    // End, expect vec2 and vec3 destroy:
}
```
To build:
<pre>
cd std_move
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# Create a custom vector class:
new MyVector(0x7fb301c02990, currlen=0, maxlen=1 elems=[])
push_back called MyVector(0x7fb301c02990, currlen=1, maxlen=1 elems=[10])
push_back called MyVector(0x7fb301c02990, currlen=2, maxlen=2 elems=[10,11])
vec1: [10][11]

# Create a new copy of vec1, vec2 via copy constructor (&):
copy constructor called for MyVector(0x7fb301c02990, currlen=2, maxlen=2 elems=[10,11])
copy constructor result is  MyVector(0x7ffee77c8668, currlen=2, maxlen=2 elems=[10,11])
vec2: [10][11]

# Check we can append onto the copied vector:
push_back called MyVector(0x7ffee77c8668, currlen=3, maxlen=4 elems=[10,11,12])
push_back called MyVector(0x7ffee77c8668, currlen=4, maxlen=4 elems=[10,11,12,13])
vec2: [10][11][12][13]

# Create a new vector from vec1, vec3 via the move constructor (&&):
std::move called for MyVector(0x7fb301c02990, currlen=2, maxlen=2 elems=[10,11])
std::move result is  MyVector(0x7ffee77c8648, currlen=2, maxlen=2 elems=[10,11])
vec3: [10][11]

# Check we can append onto the std:move'd vector:
push_back called MyVector(0x7ffee77c8648, currlen=3, maxlen=4 elems=[10,11,14])
push_back called MyVector(0x7ffee77c8648, currlen=4, maxlen=4 elems=[10,11,14,15])
vec3: [10][11][14][15]

# Destroy the old vector, vec1. It has no invalid elems:
delete MyVector(0x7fb301c02990, currlen=0, maxlen=0 elems=[])

# End, expect vec2 and vec3 destroy:
delete MyVector(0x7ffee77c8648, currlen=4, maxlen=4 elems=[10,11,14,15])
delete MyVector(0x7ffee77c8668, currlen=4, maxlen=4 elems=[10,11,12,13])
</pre>
