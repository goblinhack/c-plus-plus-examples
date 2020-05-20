How to use a range based for loop
=================================

A nice extension in C++ 11 is range based for loops. They are both
readable and safer as you do not need to manually specify the boundary
conditions.

There are a few ways to walk such containers. One is via a constant iterator e.g.:

By now you will have seen the following range based for loop e.g.:
```C++
    for (auto const i : container) { }
```
Another, if you wish to modify the data as you go is:
```C++
    for (auto &i : container) { }
```
However it may be 'simpler' to always do the following and use a forward reference:
```C++
    for (auto &&i : container) { }
```
This handles corner cases for things like vectors of bitfields that you cannot have
a reference to.

Here is a full example:
```C++
#include <iostream>
#include <algorithm>
#include <vector>
#include "../common/common.h"

int main() {
    // Create a vector of strings:
    std::initializer_list< std::string > init1 = {"elem1", "elem1"};
    std::vector< std::string > vec1(init1);

    // Range based for loop iterator with a const:
    for (const auto i : vec1) {
        std::cout << "vec1: walk " << i << std::endl;
    }

    // Range based for loop iterator with a modifiable reference:
    for (auto &i : vec1) {
        i += "+ stuff";
        std::cout << "vec1: walk " << i << std::endl;
    }

    // Range based for loop iterator with forward reference:
    for (auto &&i : vec1) {
        i += "+ more stuff";
        std::cout << "vec1: walk " << i << std::endl;
    }
}
```
To build:
<pre>
cd range_based_for_loop
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# Create a vector of strings:

# Range based for loop iterator with a const:
vec1: walk elem1
vec1: walk elem1

# Range based for loop iterator with a modifiable reference:
vec1: walk elem1+ stuff
vec1: walk elem1+ stuff

# Range based for loop iterator with forward reference:
vec1: walk elem1+ stuff+ more stuff
vec1: walk elem1+ stuff+ more stuff
</pre>
