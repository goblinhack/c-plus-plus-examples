How to use a range-based for loop
=================================

A nice extension in C++ 11 is range-based for loops. They are both
readable and safer as you do not need to manually specify the boundary
conditions.

There are a few ways to walk such containers. One is via a constant iterator e.g.:

By now you will have seen the following range-based for loop e.g.:
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
a reference to. Here is such a corner case. The iterator provided is actually a thing
called a proxy iterator, vector<bool>::reference. So, this will not compile:
```C++
    #include <vector>
    int main() {
        std::vector<bool> v(8);
        for (auto& e : v) { e = true; }
    }
```
But this will!
```C++
    #include <vector>
    int main() {
        std::vector<bool> v(8);
        for (auto&& e : v) { e = true; }
    }
```
Here is a full example:
```C++
#include <iostream>
#include <algorithm>
#include <vector>

int main() {
    // Create a vector of strings:
    std::initializer_list< std::string > init1 = {"elem1", "elem1"};
    std::vector< std::string > vec1(init1);

    // Range based for loop iterator with a const:
    for (const auto &i : vec1) {
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
g++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
g++ main.o  -o example
./example
</pre>
Expected output:
<pre>

[31;1;4mCreate a vector of strings:[0m

[31;1;4mRange based for loop iterator with a const:[0m
vec1: walk elem1
vec1: walk elem1

[31;1;4mRange based for loop iterator with a modifiable reference:[0m
vec1: walk elem1+ stuff
vec1: walk elem1+ stuff

[31;1;4mRange based for loop iterator with forward reference:[0m
vec1: walk elem1+ stuff+ more stuff
vec1: walk elem1+ stuff+ more stuff
</pre>
