How to use std::unique_ptr with a custom deleter
================================================

Often when dealing with legacy C code, memory is allocated for you which
you would like to be able to manage to avoid leaks. One way of doing this
is with a custom deleter.
```C++
    static void mydeleter (char* mem) noexcept {
        auto addr = static_cast<const void*>(mem);
        free((void*)mem);
    }
    std::unique_ptr<char, decltype(deleter) > retval(mem, deleter);
```
Or if you want, via a lambda:
```C++
    auto lambda_deleter = ( [](char *mem) {
        if (mem) {
            free((void*)mem);
        }
    } );

    std::unique_ptr<char, decltype(lambda_deleter) > retval(mem, lambda_deleter);
```
Note that make_unique cannot be used with this approach as the whole point
of make_unique is to automatically invoke a deleter for the managed object.
In our case, C is managing the object so we cannot use it and must provide
a deleter.
```C++
#include <memory>
#include <functional> // for _1, _2
#include <iostream>
#include <sstream>
#include <string.h>

using namespace std::placeholders; // for _1, _2, _3...

static auto cpp_strdup1 (const char *s)
{
    char *mem = strdup(s);
    auto addr = static_cast<const void*>(mem);
    std::cout << "copy " << mem << " addr " << addr << std::endl;

    auto lambda_deleter = ( [](char *mem) {
        if (mem) {
            auto addr = static_cast<const void*>(mem);
            std::cout << "free " << mem << " addr " << addr << std::endl;
            free((void*)mem);
        }
    } );

    std::unique_ptr<char, decltype(lambda_deleter) > retval(mem, lambda_deleter);
    return retval;
}

static void mydeleter (char* mem) noexcept {
    auto addr = static_cast<const void*>(mem);
    std::cout << "free " << mem << " addr " << addr << std::endl;
    free((void*)mem);
}

static auto cpp_strdup2 (const char *s)
{
    char *mem = strdup(s);
    auto addr = static_cast<const void*>(mem);
    std::cout << "copy " << mem << " addr " << addr << std::endl;
    std::unique_ptr<char, std::function<void(char*)> > retval(mem, mydeleter);
    return retval;
}

static auto cpp_strdup3 (const char *s)
{
    char *mem = strdup(s);
    auto addr = static_cast<const void*>(mem);
    std::cout << "copy " << mem << " addr " << addr << std::endl;
    std::unique_ptr<char, decltype(&mydeleter) > retval(mem, mydeleter);
    return retval;
}

static auto cpp_strdup4 (const char *s)
{
    char *mem = strdup(s);
    auto addr = static_cast<const void*>(mem);
    std::cout << "copy " << mem << " addr " << addr << std::endl;
    auto fn = std::bind(mydeleter, _1);
    std::unique_ptr<char, decltype(fn) > retval(mem, fn);
    return retval;
}

int main (void)
{
    // Cpp strdup wrapper with lambda deleter
    auto p1 = cpp_strdup1("hello");
    std::cout << p1.get() << std::endl;

    // Cpp strdup wrapper with std::function deleter
    auto p2 = cpp_strdup2("there");
    std::cout << p2.get() << std::endl;

    // Cpp strdup wrapper with decltype(&mydeleter) deleter
    auto p3 = cpp_strdup3("Zaphod");
    std::cout << p3.get() << std::endl;

    // Cpp strdup wrapper with std::bind deleter
    auto p4 = cpp_strdup4("Beeblebrox");
    std::cout << p4.get() << std::endl;

    // End, expect memory to be freed
}
```
To build:
<pre>
cd std_unique_ptr_with_custom_deallocator
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# Cpp strdup wrapper with lambda deleter
copy hello addr 0x7fdfc04029b0
hello

# Cpp strdup wrapper with std::function deleter
copy there addr 0x7fdfc04029c0
there

# Cpp strdup wrapper with decltype(&mydeleter) deleter
copy Zaphod addr 0x7fdfc04029d0
Zaphod

# Cpp strdup wrapper with std::bind deleter
copy Beeblebrox addr 0x7fdfc04029e0
Beeblebrox

# End, expect memory to be freed
free Beeblebrox addr 0x7fdfc04029e0
free Zaphod addr 0x7fdfc04029d0
free there addr 0x7fdfc04029c0
free hello addr 0x7fdfc04029b0
</pre>
