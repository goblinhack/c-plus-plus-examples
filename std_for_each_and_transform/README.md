How to use std::for_each and std::transform
===========================================

std::for_each is used for iterating over containers. You *can* modify
that list as you walk, but if you do, you should perhaps be using
std::transform as that is clearer to the reader what you are doing.

Here is a simple example that std::for_each is well suited for in that
it does not create and side effects while doing the walk.
```C++
        std::list< std::string > l;
        std::for_each(l.begin(), l.end(), [](std::string& s) { std::cout << s << std::endl; });
    }
```
Now you *could* do:
```C++
        std::list< std::string > l;
        std::for_each(l.begin(), l.end(), [](std::string& s) { s += "stuff"; });
    }
```
And all the elements in the list will be modified but it the implication of
for_each is that you would not be doing any modifications, In such a case,
transform is clearer e.g.:
```C++
        std::list< std::string > l;
        std::transform(l.begin(), l.end(), l.begin(), [](std::string& s) { return s + "stuff"; });
    }
```
Note the use of return there. If you want to avoid modifying the original list then you can do:
```C++
        std::list< std::string > o;
        std::list< std::string > n;
        std::transform(o.begin(), o.end(), std::back_inserter(n), [](std::string& s) { return s + "stuff"; });
    }
```
Here is a full example:
```C++
#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <utility>

int main(int, char **)
{
  const std::string                    a = "Zaphod";
  const std::string                    b = "Marvin";
  const std::string                    c = "The Mice";
  std::initializer_list< std::string > i = {a, b, c};

  // Use std::for_each to modify a list of strings
  {
    std::list< std::string > l(i);
    std::for_each(l.begin(), l.end(), [](std::string &s) { s += " is modified via std::for_each"; });
    for (auto e : l) {
      std::cout << "list-elem is " << e << std::endl;
    }
  }

  // Use std::transform to modify a list of strings
  {
    std::list< std::string > l(i);
    std::transform(l.begin(), l.end(), l.begin(),
                   [](std::string &s) { return s + " is modified via std::transform"; });
    for (auto e : l) {
      std::cout << "list-elem is " << e << std::endl;
    }
  }

  // Use std::transform to write to a new list
  {
    std::list< std::string > oldlist(i);
    std::list< std::string > newlist;
    std::transform(oldlist.begin(), oldlist.end(), std::back_inserter(newlist),
                   [](std::string &s) { return s + " is modified via std::transform"; });
    for (auto e : oldlist) {
      std::cout << "oldlist-elem is " << e << std::endl;
    }
    for (auto e : newlist) {
      std::cout << "newlist-elem is " << e << std::endl;
    }
  }
}
```
To build:
<pre>
cd std_for_each_and_transform
rm -f *.o example
clang -std=c++2a -Werror -g -O3 -fstack-protector-all -ggdb3 -Wall -c -o main.o main.cpp
clang  main.o -lstdc++  -o example
./example
</pre>
Expected output:
<pre>

[31;1;4mUse std::for_each to modify a list of strings[0m
list-elem is Zaphod is modified via std::for_each
list-elem is Marvin is modified via std::for_each
list-elem is The Mice is modified via std::for_each

[31;1;4mUse std::transform to modify a list of strings[0m
list-elem is Zaphod is modified via std::transform
list-elem is Marvin is modified via std::transform
list-elem is The Mice is modified via std::transform

[31;1;4mUse std::transform to write to a new list[0m
oldlist-elem is Zaphod
oldlist-elem is Marvin
oldlist-elem is The Mice
newlist-elem is Zaphod is modified via std::transform
newlist-elem is Marvin is modified via std::transform
newlist-elem is The Mice is modified via std::transform
</pre>
