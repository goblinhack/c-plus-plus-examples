#include <iostream>
#include <string>
#include <sstream> // std::stringstream
#include <algorithm> // std::move
#include "../common/common.h"

template<class T> class MyVector {
private:
    T *data;
    size_t maxlen;
    size_t currlen;
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
    DOC("Create a custom vector class:");
    auto vec1 = new MyVector<int>(1);
    vec1->push_back(10);
    vec1->push_back(11);
    std::cout << "vec1: " << *vec1 << std::endl;

    DOC("Create a new copy of vec1, vec2 via copy constructor (&):");
    auto vec2 = *vec1;
    std::cout << "vec2: " << vec2 << std::endl;

    DOC("Check we can append onto the copied vector:");
    vec2.push_back(12);
    vec2.push_back(13);
    std::cout << "vec2: " << vec2 << std::endl;

    DOC("Create a new vector from vec1, vec3 via the move constructor (&&):");
    auto vec3 = std::move(*vec1);
    std::cout << "vec3: " << vec3 << std::endl;

    DOC("Check we can append onto the std:move'd vector:");
    vec3.push_back(14);
    vec3.push_back(15);
    std::cout << "vec3: " << vec3 << std::endl;

    DOC("Destroy the old vector, vec1. It has no invalid elems:");
    delete vec1;

    DOC("End, expect vec2 and vec3 destroy:");
    DOC("End");
}
