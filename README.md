Modern C++ examples
===================

Are you looking for a collection of short snippets of modern C++ code
covering features in C++11 and beyond? Of course you are! As I'm always
learning C++ myself, I wanted to put out simple examples of each of the
main features of the language. You can build each example individually
or as a whole. Please let me know if this is useful to anyone or if
there are any areas you want covered.

work in progress
----------------
[shared pointer](shared_ptr/README.md)
[shared pointer wrapper](shared_ptr_wrapper/README.md)

Building all
============

Do

<pre>
    sh ./RUNME
</pre>
std::shared_ptr example
=======================

Here we create two instances of a std::shared_ptr and then make them point to
each other. This creates a deadlock which can only be resolved by calling 
reset() upon which the pointers can then be destructed.

```C++
#include <memory>
#include <iostream>
#include <sstream>

typedef std::shared_ptr< class Foo > Foop;

class Foo {
private:
    std::string data;
    Foop other;
public:
    Foo(std::string data) : data(data) {
        std::cout << "new " << to_string() << std::endl;
    }
    ~Foo() {
        std::cout << "delete " << to_string() << std::endl;
    }
    void addref (Foop other) {
        other = other;
    }
    void delref (void) {
        other.reset();
    }
    std::string to_string (void) {
        auto address = static_cast<const void*>(this);
        std::stringstream ss;
        ss << address;
        return "Foo(" + ss.str() + ", data=" + data + ")";
    }
};

int main (void)
{
    auto foo1 = std::make_shared< class Foo >(std::string("foo1-data"));
    auto foo2 = std::make_shared< class Foo >(std::string("foo2-data"));
    foo1->addref(foo2);
    foo2->addref(foo1);
    foo1->delref();
    foo2->delref();
}
```
To build:
<pre>
cd shared_ptr
rm -rf example .o/*.o
c++ -std=c++11 -std=c++2a -Werror -g -ggdb3 -O2 -Wall -c -o .o/main.o main.cpp
c++ .o/main.o  -o example
./example
</pre>

Expected output:
<pre>
new Foo(0x7f8b644029c8, data=foo1-data)
new Foo(0x7f8b64402a38, data=foo2-data)
delete Foo(0x7f8b64402a38, data=foo2-data)
delete Foo(0x7f8b644029c8, data=foo1-data)
</pre>
Simple C++ 11 wrapper for std::shared_ptr
=========================================

Do you want to wrap shared_ptr so you can add data to help with debugging? - 
of course you do! Example:

```C++
//
// Create two classes that refer to each other. Once the references are
// reset, the classes should be auto destructed.
//
#include "shared_ptr_wrapper.h"

typedef SmartPointerWrapper< class Bar > Barp;

class Bar {
public:
    Barp other;
    Bar(void) { }
    ~Bar() { }
};

int main (void)
{
    auto bar1 = SmartPointerWrapper< class Bar >(std::string("bar1"));
    auto bar2 = SmartPointerWrapper< class Bar >(std::string("bar2"));

    bar1->other = bar2;
    bar1->other.rename(std::string("ref to bar2"));

    bar2->other = bar1;
    bar2->other.rename(std::string("ref to bar1"));

    bar1->other.reset();
    bar2->other.reset();
}
```

output (with some extra debugs):

<pre>
    create classes
    ==============
    init SmartPointerWrapper(0x7f9a29c029f8, nullptr)
    new Bar(0x7f9a29c029f8)
    make_shared SmartPointerWrapper(0x7ffeea9f16b8, bar1)
    init SmartPointerWrapper(0x7f9a29c02a38, nullptr)
    new Bar(0x7f9a29c02a38)
    make_shared SmartPointerWrapper(0x7ffeea9f16e0, bar2)

    create reference loop
    =====================
    rename SmartPointerWrapper(0x7f9a29c029f8, ref to bar2)
    rename SmartPointerWrapper(0x7f9a29c02a38, ref to bar1)

    remove reference loop
    =====================
    reset SmartPointerWrapper(0x7f9a29c029f8, ref to bar2)
    reset SmartPointerWrapper(0x7f9a29c02a38, ref to bar1)

    end of main, expect auto destruct
    =================================
    delete SmartPointerWrapper(0x7ffeea9f16e0, bar2)
    delete Bar(0x7f9a29c02a38)
    delete SmartPointerWrapper(0x7f9a29c02a38, ref to bar1)
    delete SmartPointerWrapper(0x7ffeea9f16b8, bar1)
    delete Bar(0x7f9a29c029f8)
    delete SmartPointerWrapper(0x7f9a29c029f8, ref to bar2)
</pre>

Checking for leaks
==================

If you want to go a bit further there are a few more headers you can include:

```C++
    #define ENABLE_PTRCHECK
    #include "my_traceback.h"
    #include "my_ptrcheck.h"
    #include "shared_ptr_wrapper.h"
```

You will also need to include these in your build

<pre>
    ptrcheck.cpp
    sprintf.cpp
    traceback.cpp
</pre>

And then any pointer that you wish to track, use newptr on allocations and
oldptr on deallocations. You can then call ptrcheck_leak_print to see what
is currently still outstanding. Note I've only enabled this for single
threading; should anyone want multithread, ask me; it's not hard to add.

Example:

```C++
#define DEBUG
#define ENABLE_PTRCHECK
#include "my_traceback.h"
#include "my_ptrcheck.h"
#include "shared_ptr_wrapper.h"

typedef SmartPointerWrapper< class Foo > Foop;

class Foo {
private:
    std::string data;
    void debug (const std::string &what) {
#ifdef DEBUG
        std::cout << what << " " << to_string() << std::endl;
#endif
    }
public:
    Foop other;
    Foo(std::string data) : data(data) {
        debug("new");
        newptr(this, to_string());
    }
    ~Foo() {
        oldptr(this);
        debug("delete");
    }
    std::string to_string (void) {
        auto address = static_cast<const void*>(this);
        std::stringstream ss;
        ss << address;  
        return "Foo(" + ss.str() + ", data=" + data + ")";
    }
};

typedef SmartPointerWrapper< class Bar > Barp;

class Bar {
private:
    void debug (const std::string &what) {
#ifdef DEBUG
        std::cout << what << " " << to_string() << std::endl;
#endif
    }
public:
    Barp other;
    Bar(void) {
        debug("new");
        newptr(this, to_string());
    }
    ~Bar() {
        oldptr(this);
        debug("delete");
    }
    std::string to_string (void) {
        auto address = static_cast<const void*>(this);
        std::stringstream ss;
        ss << address;  
        return "Bar(" + ss.str() + ")";
    }
};

int main (void)
{
    auto foo1 = SmartPointerWrapper< class Foo >(std::string("ref to foo1"), 
                                                 std::string("foo1-data"));

    std::cout << "\ncreate classes" << std::endl;
    std::cout << "==============" << std::endl;
    auto bar1 = SmartPointerWrapper< class Bar >(std::string("bar1"));
    auto bar2 = SmartPointerWrapper< class Bar >(std::string("bar2"));

    std::cout << "\ncreate reference loop" << std::endl;
    std::cout << "=====================" << std::endl;
    bar1->other = bar2;
    bar1->other.rename(std::string("ref to bar2"));

    bar2->other = bar1;
    bar2->other.rename(std::string("ref to bar1"));

    std::cout << "\nremove reference loop" << std::endl;
    std::cout << "=====================" << std::endl;
    bar1->other.reset();
    bar2->other.reset();

    std::cout << "\ncheck for leaks" << std::endl;
    std::cout << "===============" << std::endl;

    ptrcheck_leak_print();

    std::cout << "\nend of main, expect auto destruct" << std::endl;
    std::cout << "=================================" << std::endl;
}
```

Output correctly catches that the objects are not yet released at the
time we call ptrcheck_leak_print():

<pre>
PTRCHECK: Leak 0x7f87d1d00018 "Foo(0x7f87d1d00018, data=foo1-data)" (64 bytes) at Foo::Foo(std::string):shared_ptr_wrapper.cpp() line 24 at Sun Nov 24 18:58:55 2019
stack trace:
> 1 ptrcheck_alloc(void const*, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, int, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, int)
> 2 Foo::Foo(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >)
> 3 std::__1::shared_ptr<Foo> std::__1::shared_ptr<Foo>::make_shared<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >&>(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >&&&)
> 4 SmartPointerWrapper<Foo>::SmartPointerWrapper<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > >(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >)
> 5   shared_ptr_wrapper                  0x000000010b99f1c2 main + 130
> 6   libdyld.dylib                       0x00007fff7b810015 start + 1

PTRCHECK: Leak 0x7f87d1e00498 "Bar(0x7f87d1e00498)" (40 bytes) at Bar::Bar():shared_ptr_wrapper.cpp() line 51 at Sun Nov 24 18:58:55 2019
stack trace:
> 1 ptrcheck_alloc(void const*, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, int, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, int)
> 2 Bar::Bar()
> 3 SmartPointerWrapper<Bar>::SmartPointerWrapper(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)
> 4   shared_ptr_wrapper                  0x000000010b99f2fb main + 443
> 5   libdyld.dylib                       0x00007fff7b810015 start + 1

PTRCHECK: Leak 0x7f87d1e00858 "Bar(0x7f87d1e00858)" (40 bytes) at Bar::Bar():shared_ptr_wrapper.cpp() line 51 at Sun Nov 24 18:58:55 2019
stack trace:
> 1 ptrcheck_alloc(void const*, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, int, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, int)
> 2 Bar::Bar()
> 3 SmartPointerWrapper<Bar>::SmartPointerWrapper(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)
> 4   shared_ptr_wrapper                  0x000000010b99f33e main + 510
> 5   libdyld.dylib                       0x00007fff7b810015 start + 1
</pre>

Building
========

Do

<pre>
    sh ./RUNME
</pre>

Or, if that fails, manual build:

<pre>
    c++ -std=c++11 -Werror -g -ggdb3 -O2 -Wall -Wall -c -o .o/shared_ptr_wrapper.o shared_ptr_wrapper.cpp
    c++ -std=c++11 -Werror -g -ggdb3 -O2 -Wall -Wall -c -o .o/traceback.o traceback.cpp
    c++ -std=c++11 -Werror -g -ggdb3 -O2 -Wall -Wall -c -o .o/ptrcheck.o ptrcheck.cpp
    c++ -std=c++11 -Werror -g -ggdb3 -O2 -Wall -Wall -c -o .o/sprintf.o sprintf.cpp
    c++ .o/shared_ptr_wrapper.o .o/traceback.o .o/ptrcheck.o .o/sprintf.o  -o shared_ptr_wrapper
</pre>

To test:

<pre>
    ./shared_ptr_wrapper
</pre>
