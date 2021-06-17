How to use std::map for reverse sorting
=======================================

std::map provides a key to value mapping, where the key is sorted.
Behind the scenes std::map could be implemented as a tree or hashtable,
but the point is you don't need to worry about that. All you need to
do is provide a means to sort the keys via the "<" operator.

However, for standard provided classes you don't even need to do that e.g.:
```C++
    std::map< std::string, int> m;
```
But what is the sort order? Well it is alphabetically increasing, but you
can be specific and say:
```C++
    std::map< std::string, int, std::less<std::string> > m;
```
Or alternatively for reverse sorting:
```C++
    std::map< std::string, int, std::greater<std::string> > m;

    m["zaphod"] = 1;
    m["universe"] = 2;
    m["arthur"] = 3;
    m["marvin"] = 4;
    m["mice"] = 5;
    m["vogon"] = 6;
```
yields
```C++
    arthur 3
    marvin 4
    mice 5
    universe 2
    vogon 6
    zaphod 1
```
Here is the full example:
```C++
#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <map>

static void backward_sort (void) {
    // backward sorted map
    std::map< std::string, int, std::greater<std::string> > m;

    m["zaphod"] = 1;
    m["universe"] = 2;
    m["arthur"] = 3;
    m["marvin"] = 4;
    m["mice"] = 5;
    m["vogon"] = 6;

    for (auto i : m) {
        std::cout << i.first << " " << i.second << std::endl;
    }
}

static void forward_sort (void) {
    // forward sorted map
    std::map< std::string, int, std::less<std::string> > m;

    m["zaphod"] = 1;
    m["universe"] = 2;
    m["arthur"] = 3;
    m["marvin"] = 4;
    m["mice"] = 5;
    m["vogon"] = 6;

    for (auto i : m) {
        std::cout << i.first << " " << i.second << std::endl;
    }
}

static void default_sort (void) {
    // default sorted map
    std::map< std::string, int> m;

    m["zaphod"] = 1;
    m["universe"] = 2;
    m["arthur"] = 3;
    m["marvin"] = 4;
    m["mice"] = 5;
    m["vogon"] = 6;

    for (auto i : m) {
        std::cout << i.first << " " << i.second << std::endl;
    }
}

int main(int, char**) {
    backward_sort();
    forward_sort();
    default_sort();
}
```
To build:
<pre>
cd std_map_reverse_sort
rm -f *.o example
g++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
g++ main.o  -o example
./example
</pre>
Expected output:
<pre>

[31;1;4mbackward sorted map[0m
zaphod 1
vogon 6
universe 2
mice 5
marvin 4
arthur 3

[31;1;4mforward sorted map[0m
arthur 3
marvin 4
mice 5
universe 2
vogon 6
zaphod 1

[31;1;4mdefault sorted map[0m
arthur 3
marvin 4
mice 5
universe 2
vogon 6
zaphod 1
</pre>
