#include "../common/common.h"
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <vector>

template < class T > class MyVector
{
private:
  T     *data {};
  size_t maxlen {};
  size_t currlen {};

public:
  MyVector() : data(nullptr), maxlen(0), currlen(0)
  {
    std::cout << "default constructor " << to_string() << std::endl;
  }
  MyVector(int maxlen) : data(new T[ maxlen ]), maxlen(maxlen), currlen(0)
  {
    std::cout << "new " << to_string() << std::endl;
  }
  MyVector(const MyVector &o)
  {
    std::cout << "copy constructor called for " << o.to_string() << std::endl;
    data    = new T[ o.maxlen ];
    maxlen  = o.maxlen;
    currlen = o.currlen;
    std::copy(o.data, o.data + o.maxlen, data);
    std::cout << "copy constructor result is  " << to_string() << std::endl;
  }
  MyVector(MyVector< T > &&o)
  {
    std::cout << "std::move called for " << o.to_string() << std::endl;
    data      = o.data;
    maxlen    = o.maxlen;
    currlen   = o.currlen;
    o.data    = nullptr;
    o.maxlen  = 0;
    o.currlen = 0;
    std::cout << "std::move result is  " << to_string() << std::endl;
  }
  ~MyVector() { std::cout << "delete " << to_string() << std::endl; }
  MyVector(std::initializer_list< T > &l)
  {
    for (auto i : l) {
      push_back(i);
    }
  }
  void push_back(const T &i)
  {
    if (! currlen) {
      maxlen = 1;
      data   = new T[ maxlen ];
    } else if (currlen >= maxlen) {
      maxlen *= 2;
      auto newdata = new T[ maxlen ];
      std::copy(data, data + currlen, newdata);
      if (data) {
        delete[] data;
      }
      data = newdata;
    }
    data[ currlen++ ] = i;
    std::cout << "push_back called " << to_string() << std::endl;
  }
  friend std::ostream &operator<<(std::ostream &os, const MyVector< T > &o)
  {
    auto s = o.data;
    auto e = o.data + o.currlen;
    ;
    while (s < e) {
      os << "[" << *s << "]";
      s++;
    }
    return os;
  }
  std::string to_string(void) const
  {
    auto              address = static_cast< const void              *>(this);
    std::stringstream ss;
    ss << address;

    std::string elems;
    auto        s = data;
    auto        e = data + currlen;
    ;
    while (s < e) {
      elems += *s;
      s++;
      if (s < e) {
        elems += ",";
      }
    }

    return "MyVector(" + ss.str() + ", currlen=" + std::to_string(currlen) + ", maxlen=" + std::to_string(maxlen) +
           " elems=[" + elems + "])";
  }
  class Iterator
  {
  public:
    Iterator(T *ptr) : ptr(ptr) {}
    Iterator operator++()
    {
      ++ptr;
      return *this;
    }
    bool     operator!=(const Iterator &o) const { return ptr != o.ptr; }
    const T &operator*() const { return *ptr; }

  private:
    T *ptr;
  };

public:
  Iterator begin() const { return Iterator(data); }
  Iterator end() const { return Iterator(data + currlen); }
};

int main()
{
  DOC("Create a std::initializer_list of std::string:");
  std::initializer_list< std::string > init1 = {"elem1", "elem2"};

  DOC("Assign this initializer_list to a vector:");
  MyVector< std::string > vec1(init1);

  DOC("Walk the vector with 'const auto &i': (to avoid copies)");
  for (const auto &i : vec1) {
    std::cout << i << std::endl;
  }

  DOC("End:");
}
