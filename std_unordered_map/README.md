How to use std::unordered_map
=============================

Perhaps the question is more why use it? If you do not care about sort
order i.e. performance is more important, then std::unordered_map is
for you.

To insert into an unordered map, you can do any of:
```C++
    std::unordered_map< std::string, int> m;
    m["zaphod"] = 1;
    m.insert(std::make_pair("universe", 2));
    m.emplace(std::make_pair("mice", 3));
```
To remove
```C++
    std::unordered_map< std::string, int> m;
    m.erase("universe")
```
To find
```C++
    std::unordered_map< std::string, int> m;
    if (m.find("universe") == m.end()) {
        // not found!
    }
```
To destroy everything:
```C++
    std::unordered_map< std::string, int> m;
    m.clear();
```
To demonstrate the difference, we can compare std::map with
std::unordered_map.
```C++
    std::unordered_map< std::string, int> m;

    m["zaphod"] = 1;
    m["universe"] = 2;
    m["arthur"] = 3;
    m["marvin"] = 4;
    m["mice"] = 5;
    m["vogon"] = 6;

    for (auto i : m) {
        std::cout << i.first << " " << i.second << std::endl;
    }
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
Whereas:
```C++
    std::unordered_map< std::string, int> m;

    m["zaphod"] = 1;
    m["universe"] = 2;
    m["arthur"] = 3;
    m["marvin"] = 4;
    m["mice"] = 5;
    m["vogon"] = 6;
```
yields
```C++
    zaphod 1
    marvin 4
    arthur 3
    universe 2
    vogon 6
    mice 5
```
Here is the full example:
```C++
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>

static void std_unordered_map_sort(void)
{
  // std::unordered_map
  std::unordered_map< std::string, int > m;

  m[ "zaphod" ] = 1;
  m.insert(std::make_pair("universe", 2));
  m.emplace(std::make_pair("arthur", 3));
  m[ "marvin" ] = 4;
  m[ "mice" ]   = 5;
  m[ "vogon" ]  = 6;
  m.erase("universe");

  for (auto i : m) {
    std::cout << i.first << " " << i.second << std::endl;
  }
}

static void std_map_sort(void)
{
  // std::map
  std::map< std::string, int > m;

  m[ "zaphod" ] = 1;
  m.insert(std::make_pair("universe", 2));
  m.emplace(std::make_pair("arthur", 3));
  m[ "marvin" ] = 4;
  m[ "mice" ]   = 5;
  m[ "vogon" ]  = 6;
  m.erase("universe");

  for (auto i : m) {
    std::cout << i.first << " " << i.second << std::endl;
  }
}

int main(int, char **)
{
  std_map_sort();
  std_unordered_map_sort();
}
```
To build:
<pre>
cd std_unordered_map
rm -f *.o example
g++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
g++ main.o  -o example
./example
</pre>
Expected output:
<pre>

[31;1;4mstd::map[0m
arthur 3
marvin 4
mice 5
vogon 6
zaphod 1

[31;1;4mstd::unordered_map[0m
vogon 6
mice 5
marvin 4
arthur 3
zaphod 1
</pre>
