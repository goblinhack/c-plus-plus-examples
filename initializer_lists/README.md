How to use std::initializer_list
================================

In days of old, to populate a list or a vector in C++ you had to do:
```C++
    #include <vector>
    #include <stream>
    #include <initializer_list>

    std::vector<std::string> orbiting;
    orbiting.push_back(std::string("space"));
    orbiting.push_back(std::string("station"));
```
Which is rather tedious.

C++11 introduces initializer lists which are lightweight proxies to help
in the construction of containers. They use the bracketed { } syntax. For
example:
```C++
    std::initializer_list<std::string> objects = { std::string("space"), std::string("station") };
    std::vector<std::string> orbiting(objects);
```
That is not too bad, but it can be shorter with inline syntax for the
initializer list e.g.:
```C++
    std::vector<std::string> orbiting = { std::string("space"), std::string("station") };
```
For most containers as they have a constructor for std::string you can
even just do:
```C++
    std::vector<std::string> orbiting = { "space", "station" };
```
Which is nice and concise.

However there can be some hidden costs. When the initializer list is
assigned to a container, the copy constructor is invoked. So you are
still doing a push_back onto the container for each element.
```C++
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <vector>

class MyString
{
private:
  std::string s;

public:
  MyString()
  {
    auto address = static_cast< const void * >(this);
    std::cout << address << " MyString() " << std::endl;
  }
  ~MyString()
  {
    auto address = static_cast< const void * >(this);
    std::cout << address << " ~MyString() " << s << std::endl;
  }
  MyString(const std::string &s) : s(s)
  {
    auto address = static_cast< const void * >(this);
    std::cout << address << " MyString(std::string &) " << s << std::endl;
  }
  MyString(const std::string &&s) : s(s)
  {
    auto address = static_cast< const void * >(this);
    std::cout << address << " MyString(std::string &&) " << s << std::endl;
  }
  MyString(const MyString &o) : s(o.s)
  { // copy constructor
    auto address = static_cast< const void * >(this);
    std::cout << address << " MyString(const std::string &) " << s << std::endl;
  }
  MyString(const MyString &&o) : s(o.s)
  { // move constructor
    auto address = static_cast< const void * >(this);
    std::cout << address << " MyString(const std::string &&) " << s << std::endl;
  }
  friend std::ostream &operator<<(std::ostream &os, const MyString &o) { return os << o.s; }
};

int main()
{
  // Create a std::initializer_list of MyString:
  std::initializer_list< MyString > init1 = {MyString(std::string("elem1")), MyString(std::string("elem2"))};

  // Assign this initializer_list to a vector:
  std::vector< MyString > vec1(init1);

  //
  // Commented out as compilers are now smart and complain about this intentional copy
  //
  // // Walk the vector with 'const auto i': (this will involve copies)
  // for (const auto i : vec1) {
  //     std::cout << i << std::endl;
  // }

  // Walk the vector with 'const auto &i': (should see no copies)
  for (const auto &i : vec1) {
    std::cout << i << std::endl;
  }

  // Walk the vector with forward reference 'auto &&i': (should see no copies)
  for (auto &&i : vec1) {
    std::cout << i << std::endl;
  }

  // Create another vector with an inline initializer list
  // This will not work
  // std::vector< MyString > vec2 (
  //     MyString(std::string("elem3")), MyString(std::string("elem4"))
  // );
  std::vector< MyString > vec2 = {MyString(std::string("elem3")), MyString(std::string("elem4"))};

  // End:
}
```
To build:
<pre>
cd initializer_lists
rm -f *.o example
clang -std=c++2a -Werror -g -O3 -fstack-protector-all -ggdb3 -Wall -c -o main.o main.cpp
clang  main.o -lstdc++  -o example
./example
</pre>
Expected output:
<pre>

# Create a std::initializer_list of MyString:
0x16b08efb8 MyString(std::string &&) elem1
0x16b08efd0 MyString(std::string &&) elem2

# Assign this initializer_list to a vector:
0x6000030142d0 MyString(const std::string &) elem1
0x6000030142e8 MyString(const std::string &) elem2

# Walk the vector with 'const auto &i': (should see no copies)
elem1
elem2

# Walk the vector with forward reference 'auto &&i': (should see no copies)
elem1
elem2

# Create another vector with an inline initializer list
0x16b08ef88 MyString(std::string &&) elem3
0x16b08efa0 MyString(std::string &&) elem4
0x600003014300 MyString(const std::string &) elem3
0x600003014318 MyString(const std::string &) elem4
0x16b08efa0 ~MyString() elem4
0x16b08ef88 ~MyString() elem3

# End:
0x600003014318 ~MyString() elem4
0x600003014300 ~MyString() elem3
0x6000030142e8 ~MyString() elem2
0x6000030142d0 ~MyString() elem1
0x16b08efd0 ~MyString() elem2
0x16b08efb8 ~MyString() elem1
</pre>
