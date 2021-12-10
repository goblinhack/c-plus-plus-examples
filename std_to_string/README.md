How to use (std::) to_string with containers and custom classes
===============================================================

There are two options for printing your own class.

One option is to take advantage of ADL (argument dependent lookup) via
"using std::to_string;" in your own code. This will cause the compiler
to pick either std::to_string or your own to_string() for your own class.

The other option is to extend the std namespace.

The first option is cleaner, but you can get into trouble with type promotion
with enum types. In such cases I prefer to know which namespace I am really
pulling to_string from. This code below follows the second approach.

Here we add template container support in addition to to_string for a custom
class. This allows us to easily print a vector or list of custom classes for
example.

Here is a full example:
```C++
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
  // Print Foo
  Foo foo(99);
  std::cout << "Single foo: " << std::to_string(foo) << std::endl;

  // Print std::list<Foo>
  std::list< Foo > list_of_foo;
  list_of_foo.push_back(Foo(42));
  list_of_foo.push_back(Foo(43));
  std::cout << "List of foo: " << std::to_string(list_of_foo) << std::endl;

  // Print std::vector<Foo>
  std::vector< Foo > vector_of_foo;
  vector_of_foo.push_back(Foo(6));
  vector_of_foo.push_back(Foo(7));
  vector_of_foo.push_back(Foo(8));
  std::cout << "Vector of foo: " << std::to_string(vector_of_foo) << std::endl;

  // End
}
```
To build:
<pre>
cd std_to_string
rm -f *.o example
g++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
g++ main.o  -o example
./example
</pre>
Expected output:
<pre>

[31;1;4mPrint Foo[0m
Single foo: 99

[31;1;4mPrint std::list<Foo>[0m
List of foo: [42, 43]

[31;1;4mPrint std::vector<Foo>[0m
Vector of foo: [6, 7, 8]

# End
</pre>
