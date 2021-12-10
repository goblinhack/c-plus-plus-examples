#include "../common/common.h"
#include <iostream>
#include <list>
#include <string>
#include <vector>

struct Foo {
  Foo(int v) : value(v) {}

public:
  int value;
};

namespace std
{
std::string to_string(const struct Foo &f) { return std::to_string(f.value); }

template < typename T > std::string to_string(const T &v) { return std::to_string(v); }

template < class T, template < typename Elem, typename Allocator = std::allocator< Elem > > class C >
static inline const std::string to_string(const C< T > &elems)
{
  std::string out = "[";
  for (const auto &elem : elems) {
    if (out.size() > 1) {
      out += ", ";
    }
    out += std::to_string< T >(elem);
  }
  out += "]";
  return out;
}
} // namespace std

int main(void)
{
  DOC("Print Foo");
  Foo foo(99);
  std::cout << "Single foo: " << std::to_string(foo) << std::endl;

  DOC("Print std::list<Foo>");
  std::list< Foo > list_of_foo;
  list_of_foo.push_back(Foo(42));
  list_of_foo.push_back(Foo(43));
  std::cout << "List of foo: " << std::to_string(list_of_foo) << std::endl;

  DOC("Print std::vector<Foo>");
  std::vector< Foo > vector_of_foo;
  vector_of_foo.push_back(Foo(6));
  vector_of_foo.push_back(Foo(7));
  vector_of_foo.push_back(Foo(8));
  std::cout << "Vector of foo: " << std::to_string(vector_of_foo) << std::endl;

  DOC("End");
}
