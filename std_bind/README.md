How to use std::bind, the basic stuff
=====================================

std::bind is a useful construct that allows you to associate some context
(i.e. a classes "this" pointer) with a function. It is primarily of use
with invoking callback functionaliy, but to get started let's focus on a
some simple cases:
```C++
    auto bind_fn = bind(bind_test, _1, _2, _3);
```
The funny syntax, _1 refers to an argument number is is fully called 
std::placeholders::_1 but for obvious reasons people prefer to use:
```C++
    using namespace std::placeholders; // for _1, _2, _3...
```
to make things readable. Once a function is bound, you can invoke it
like:
```C++
    bind_fn(1, 2, 3);
```
and bind_test with be called with (1, 2, 3). Nice.

It can also be used in std::for_each loops e.g.:
```C++
    static void do_stuff (const std::string& arg1) {
        std::cout << arg1 << std::endl;
    }

    std::list<std::string> l;
    auto bind_fn = bind(do_stuff, _1);
    std::for_each(l.begin(), l.end(), bind_fn);
```
And with std::transform (where you show intent to modify the list):
```C++
    static std::string do_stuff (std::string& arg1) {
        return arg1 + " is modified";
    }

    std::list<std::string> l;
    auto bind_fn = bind(do_stuff, _1);
    std::transform(l.begin(), l.end(), l.begin(), bind_fn);
```
Here is the full example:
```C++
#include <algorithm>
#include <functional> // for _1, _2
#include <iostream>
#include <list>
#include <string>
#include <utility>

using namespace std::placeholders; // for _1, _2, _3...

static void bind_fn1(const std::string &arg1, const std::string &arg2, const std::string &arg3)
{
  std::cout << "argument 1 is " << arg1 << std::endl;
  std::cout << "argument 2 is " << arg2 << std::endl;
  std::cout << "argument 3 is " << arg3 << std::endl;
}

//
// You could cheat here and use &arg1, but that makes a mockery of
// std::for_each
//
static void bind_try_to_modify_string(std::string arg1)
{
  arg1 = arg1 + " and is not modified and this will be ignored";
}

static std::string bind_really_modify_string(const std::string &arg1) { return arg1 + " and is modified"; }

int main(int, char **)
{
  const std::string a = "cat";
  const std::string b = "dog";
  const std::string c = "ork";

  auto fn1 = std::bind(bind_fn1, _1, _2, _3);
  auto fn2 = std::bind(bind_fn1, _2, _1, _3);
  auto fn3 = std::bind(bind_fn1, _3, _1, _2);

  // call bind_fn1(_1, _2, _3)
  fn1(a, b, c);

  // call bind_fn1(_2, _1, _3)
  fn2(a, b, c);

  // call bind_fn1(_3, _2, _1)
  fn3(a, b, c);

  //
  // Use std::bind with std::for_each
  //
  std::initializer_list< std::string > il = {a, b, c};
  std::list< std::string >             l1(il);

  // call our bind function via std::for_each to modify a list of strings
  auto fn4 = std::bind(bind_try_to_modify_string, _1);
  std::for_each(l1.begin(), l1.end(), fn4);
  for (auto e : l1) {
    std::cout << "l1 list-elem is now " << e << std::endl;
  }

  //
  // Use std::bind with std::transform
  //
  std::initializer_list< std::string > i2 = {a, b, c};
  std::list< std::string >             l2(i2);

  // call our bind function via std::transform to modify a list of strings
  auto fn5 = std::bind(bind_really_modify_string, _1);
  std::transform(l2.begin(), l2.end(), l2.begin(), fn5);
  for (auto e : l2) {
    std::cout << "l2 list-elem is now " << e << std::endl;
  }
}
```
To build:
<pre>
cd std_bind
rm -f *.o example
clang -std=c++2a -Werror -g -O3 -fstack-protector-all -ggdb3 -Wall -c -o main.o main.cpp
clang  main.o -lstdc++  -o example
./example
</pre>
Expected output:
<pre>

# call bind_fn1(_1, _2, _3)
argument 1 is cat
argument 2 is dog
argument 3 is ork

# call bind_fn1(_2, _1, _3)
argument 1 is dog
argument 2 is cat
argument 3 is ork

# call bind_fn1(_3, _2, _1)
argument 1 is ork
argument 2 is cat
argument 3 is dog

# call our bind function via std::for_each to modify a list of strings
l1 list-elem is now cat
l1 list-elem is now dog
l1 list-elem is now ork

# call our bind function via std::transform to modify a list of strings
l2 list-elem is now cat and is modified
l2 list-elem is now dog and is modified
l2 list-elem is now ork and is modified
</pre>
