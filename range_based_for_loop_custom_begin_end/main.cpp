#include "../common/common.h"
#include <algorithm>
#include <iostream>
#include <sstream>

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
  void push_back(const T &i)
  {
    if (currlen >= maxlen) {
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
      elems += std::to_string(*s);
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
  DOC("Create a custom vector class:");
  auto vec1 = MyVector< int >(1);
  vec1.push_back(10);
  vec1.push_back(11);
  vec1.push_back(12);

  DOC("Walk the custom vector with our iterator:");
  for (auto i : vec1) {
    std::cout << "vec1: walk " << i << std::endl;
  }

  DOC("End, expect vec1 destroy:");
}
