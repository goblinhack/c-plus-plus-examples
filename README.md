Modern C++ (C++11 ... C++20) examples
=====================================

Are you looking for a collection of short snippets of modern C++ code
covering core features in C++11 and beyond? Of course you are!

As I'm constantly learning C++ myself, I wanted to create some simple examples 
of each of the main features of the language. You can build each example
individually or as a whole.

Let me know if this is useful to anyone or if there are any areas you want
covered. This is (probably forever) a work in progress.

[How to use std::shared_ptr](shared_ptr/README.md)

[How to make your own wrapper around std::shared_ptr](shared_ptr_wrapper/README.md)

How to use std::shared_ptr
==========================

Here we create two instances of a std::shared_ptr and then make them point to
each other. This creates a deadlock which can only be resolved by calling 
reset() upon which the pointers can then be destructed.

```C++
#include <memory>
#include <iostream>
#include <sstream>
#include "../common/common.h"

typedef std::shared_ptr< class Foo > Foop;

class Foo {
private:
    std::string data; // local data for foo for debugging
    Foop other;       // pointer to other Foo's so we can make a deadlock
public:
    Foo(std::string data) : data(data) {
        std::cout << "new " << to_string() << std::endl;
    }
    ~Foo() {
        // Note, other.reset will be called for us
        std::cout << "delete " << to_string() << std::endl;
    }
    void addref (Foop other) {
        other = other;
        std::cout << "other use_count now " << other.use_count() << std::endl;
    }
    void delref (void) {
        other.reset();
        std::cout << "other use_count now " << other.use_count() << std::endl;
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
    // create a class and share it between two pointers:
    auto sptr1 = std::make_shared< class Foo >(Foo("foo1"));
    std::cout << "sptr1 ref count now " << sptr1.use_count() << std::endl;
    auto sptr2 = sptr1;
    std::cout << "sptr2 ref count now " << sptr2.use_count() << std::endl;

    // try to create a deadlock:
    sptr1->addref(sptr2);
    std::cout << "sptr1 ref count now " << sptr1.use_count() << std::endl;
    sptr2->addref(sptr1);
    std::cout << "sptr2 ref count now " << sptr2.use_count() << std::endl;

    // undo the deadlock:
    sptr1->delref();
    std::cout << "sptr1 ref count now " << sptr1.use_count() << std::endl;
    sptr2->delref();
    std::cout << "sptr2 ref count now " << sptr2.use_count() << std::endl;

    // release the shared sptrs, expect foo1 to be destroyed:
    sptr1.reset();
    std::cout << "sptr1 ref count now " << sptr1.use_count() << std::endl;
    sptr2.reset();
    std::cout << "sptr2 ref count now " << sptr2.use_count() << std::endl;
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

# create a class and share it between two pointers:
new Foo(0x7ffee92d9758, data=foo1)
delete Foo(0x7ffee92d9758, data=foo1)
sptr1 ref count now 1
sptr2 ref count now 2

# try to create a deadlock:
other use_count now 3
sptr1 ref count now 2
other use_count now 3
sptr2 ref count now 2

# undo the deadlock:
other use_count now 0
sptr1 ref count now 2
other use_count now 0
sptr2 ref count now 2

# release the shared sptrs, expect foo1 to be destroyed:
sptr1 ref count now 0
delete Foo(0x7f92204029f8, data=foo1)
sptr2 ref count now 0
</pre>
How to make your own wrapper around std::shared_ptr
===================================================

Here we create a wrapper around std::shared_ptr. Why? Mostly educational,
but you could use this to keep track of memory allocation and frees as part
of a basic leak detector.

```C++
#include <memory>
#include <sstream>
#include <string>
#include <iostream>
#include "../common/common.h"

template <typename T> class MySharedPtr {
private:
    std::shared_ptr<T> sptr;
    std::string name {"nullptr"};

    void debug (const std::string &what) {
        std::cout << name << ": " << what << " " << to_string() << std::endl;
    }

    std::string to_string (void) {
        auto address = static_cast<const void*>(this);
        std::stringstream ss;
        ss << address;
        if (sptr) {
            return "MySharedPtr(" + ss.str() + "," + sptr->to_string() + ")";
        } else {
            return "MySharedPtr(" + ss.str() + ")";
        }
    }
public:
    // explicit means constructor must match exactly
    template <typename ...ARGS> explicit
      MySharedPtr(const std::string &name, ARGS... a) : name(name) {
        sptr = std::make_shared<T>(a...);
        debug("MySharedPtr::make_shared");
    }

    explicit MySharedPtr(const std::string &name) : name(name) {
        sptr = std::make_shared<T>();
        debug("MySharedPtr::make_shared");
    }

    explicit MySharedPtr(void) {
        debug("MySharedPtr::default constructor");
    }

    ~MySharedPtr() {
        debug("MySharedPtr::delete");
    }

    T* const operator->() {
        debug("MySharedPtr::-> dereference");
        return sptr.operator->();
    }

    T* get() const {
        debug("MySharedPtr::get ptr");
        return sptr.get();
    }

    T& operator*() {
        debug("MySharedPtr::* ptr");
        return *sptr;
    }

    const T& operator*() const {
        debug("MySharedPtr::const * ptr");
        return *sptr;
    }

    operator bool() const {
        debug("MySharedPtr::bool");
        return (bool)sptr;
    }

    size_t use_count(void) const {
        return sptr.use_count();
    }

    void reset() {
        debug("MySharedPtr::reset");
        sptr.reset();
    }
};

typedef MySharedPtr< class Foo > Foop;

class Foo {
private:
    std::string data;
    void debug (const std::string &what) {
        std::cout << what << " " << to_string() << std::endl;
    }
public:
    Foo(std::string data) : data(data) {
        debug("new");
    }
    ~Foo() {
        debug("delete");
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
    // create a class and share it between two pointers:
    auto sptr1 = MySharedPtr< class Foo >("[foo1]", Foo("foo1-data"));
    std::cout << "sptr1 ref count now " << sptr1.use_count() << std::endl;
    auto sptr2 = sptr1;
    std::cout << "sptr2 ref count now " << sptr2.use_count() << std::endl;

    // release the shared sptrs, expect foo1 to be destroyed:
    sptr1.reset();
    std::cout << "sptr1 ref count now " << sptr1.use_count() << std::endl;
    sptr2.reset();
    std::cout << "sptr2 ref count now " << sptr2.use_count() << std::endl;
}
```
To build:
<pre>
cd shared_ptr_wrapper
rm -rf example .o/*.o
c++ -std=c++11 -std=c++2a -Werror -g -ggdb3 -O2 -Wall -c -o .o/main.o main.cpp
c++ .o/main.o  -o example
./example
</pre>
Expected output:
<pre>

# create a class and share it between two pointers:
new Foo(0x7ffee74d7700, data=foo1-data)
[foo1]: MySharedPtr::make_shared MySharedPtr(0x7ffee74d7718,Foo(0x7fedf24029c8, data=foo1-data))
delete Foo(0x7ffee74d7700, data=foo1-data)
sptr1 ref count now 1
sptr2 ref count now 2

# release the shared sptrs, expect foo1 to be destroyed:
[foo1]: MySharedPtr::reset MySharedPtr(0x7ffee74d7718,Foo(0x7fedf24029c8, data=foo1-data))
sptr1 ref count now 0
[foo1]: MySharedPtr::reset MySharedPtr(0x7ffee74d7760,Foo(0x7fedf24029c8, data=foo1-data))
delete Foo(0x7fedf24029c8, data=foo1-data)
sptr2 ref count now 0
[foo1]: MySharedPtr::delete MySharedPtr(0x7ffee74d7760)
[foo1]: MySharedPtr::delete MySharedPtr(0x7ffee74d7718)
</pre>
