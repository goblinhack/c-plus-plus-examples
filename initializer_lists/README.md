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
#include <iostream>
#include <algorithm>
#include <vector>
#include <initializer_list>

class MyString {
private:
    std::string s;
public:
    MyString() {
        auto address = static_cast<const void*>(this);
        std::cout << address << " MyString() " << std::endl;
    }
    ~MyString() {
        auto address = static_cast<const void*>(this);
        std::cout << address << " ~MyString() " << s << std::endl;
    }
    MyString(const std::string &s) : s(s) {
        auto address = static_cast<const void*>(this);
        std::cout << address << " MyString(std::string &) " << s << std::endl;
    }
    MyString(const std::string &&s) : s(s) {
        auto address = static_cast<const void*>(this);
        std::cout << address << " MyString(std::string &&) " << s << std::endl;
    }
    MyString(const MyString & o) : s(o.s) { // copy constructor
        auto address = static_cast<const void*>(this);
        std::cout << address << " MyString(const std::string &) " << s << std::endl;
    }
    MyString(const MyString && o) : s(o.s) { // move constructor
        auto address = static_cast<const void*>(this);
        std::cout << address << " MyString(const std::string &&) " << s << std::endl;
    }
    friend std::ostream& operator<<(std::ostream &os, const MyString& o) {
        return os << o.s;
    }
};

int main() {
    // Create a std::initializer_list of MyString:
    std::initializer_list< MyString > init1 = {
        MyString(std::string("elem1")), MyString(std::string("elem2"))
    };

    // Assign this initializer_list to a vector:
    std::vector< MyString > vec1(init1);

    // Walk the vector with 'const auto i': (this will involve copies)
    for (const auto i : vec1) {
        std::cout << i << std::endl;
    }

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
    std::vector< MyString > vec2 = {
        MyString(std::string("elem3")), MyString(std::string("elem4"))
    };

    // End:
}
```
To build:
<pre>
cd initializer_lists
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# Create a std::initializer_list of MyString:
0x7ffeed1c8728 MyString(std::string &&) elem1
0x7ffeed1c8740 MyString(std::string &&) elem2

# Assign this initializer_list to a vector:
0x7faf8e402990 MyString(const std::string &) elem1
0x7faf8e4029a8 MyString(const std::string &) elem2

# Walk the vector with 'const auto i': (this will involve copies)
0x7ffeed1c8068 MyString(const std::string &) elem1
elem1
0x7ffeed1c8068 ~MyString() elem1
0x7ffeed1c8068 MyString(const std::string &) elem2
elem2
0x7ffeed1c8068 ~MyString() elem2

# Walk the vector with 'const auto &i': (should see no copies)
elem1
elem2

# Walk the vector with forward reference 'auto &&i': (should see no copies)
elem1
elem2

# Create another vector with an inline initializer list
0x7ffeed1c86f8 MyString(std::string &&) elem3
0x7ffeed1c8710 MyString(std::string &&) elem4
0x7faf8e4029c0 MyString(const std::string &) elem3
0x7faf8e4029d8 MyString(const std::string &) elem4
0x7ffeed1c8710 ~MyString() elem4
0x7ffeed1c86f8 ~MyString() elem3

# End:
0x7faf8e4029d8 ~MyString() elem4
0x7faf8e4029c0 ~MyString() elem3
0x7faf8e4029a8 ~MyString() elem2
0x7faf8e402990 ~MyString() elem1
0x7ffeed1c8740 ~MyString() elem2
0x7ffeed1c8728 ~MyString() elem1
</pre>
