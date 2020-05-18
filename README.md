Modern C++ (C++11 ... C++20) examples
=====================================

Are you looking for a collection of short snippets of modern C++ code
covering core features in C++11 and beyond? Of course you are!

As I'm constantly learning C++ myself, I wanted to create some simple examples 
of each of the main features of the language. You can build each example
individually or as a whole.

Let me know if this is useful to anyone or if there are any areas you want
covered. This is (probably forever) a work in progress.

[How to use std::unique_ptr](unique_ptr/README.md)

[How to use std::shared_ptr](shared_ptr/README.md)

[How to make your own wrapper around std::shared_ptr](shared_ptr_wrapper/README.md)

How to use std::shared_ptr
==========================

Here we create two instances of a std::shared_ptr and then make them point to
each other. This creates a deadlock which can only be resolved by calling 
reset() upon which the pointers can then be destructed.

To create a shared pointer you can either use the following which is a bit
long winded

- std::shared_ptr<class Foo> sptr(new Foo("foo2"));

Or, the following which hides the use of "new" but also invokes the copy
constructor. If you don't want that, use the above.

- auto sptr = std::make_shared< class Foo >(Foo("foo1"));

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
    Foo(const Foo& o) {
        std::cout << "copy constructor " << to_string() << std::endl;
        data = o.data;
        other = o.other;
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
    // Create a copy constructed class and share it between two pointers:
    auto sptr1 = std::make_shared< class Foo >(Foo("foo1"));
    std::cout << "sptr1 ref count now " << sptr1.use_count() << std::endl;
    auto sptr2 = sptr1;
    std::cout << "sptr2 ref count now " << sptr2.use_count() << std::endl;

    // Try to create a deadlock:
    sptr1->addref(sptr2);
    std::cout << "sptr1 ref count now " << sptr1.use_count() << std::endl;
    sptr2->addref(sptr1);
    std::cout << "sptr2 ref count now " << sptr2.use_count() << std::endl;

    // Undo the 'deadlock':
    sptr1->delref();
    std::cout << "sptr1 ref count now " << sptr1.use_count() << std::endl;
    sptr2->delref();
    std::cout << "sptr2 ref count now " << sptr2.use_count() << std::endl;

    // Release the shared sptrs, expect foo1 to be destroyed:
    sptr1.reset();
    std::cout << "sptr1 ref count now " << sptr1.use_count() << std::endl;
    sptr2.reset();
    std::cout << "sptr2 ref count now " << sptr2.use_count() << std::endl;

    // You can also create shared pointers WITHOUT copy constructor overhead
    std::shared_ptr<class Foo> sptr0(new Foo("foo0"));
    std::cout << "sptr0 = " << (sptr0.get() ? sptr0->to_string() : "nullptr") << std::endl;
}
```
To build:
<pre>
cd shared_ptr
rm -rf example .o/*.o
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o .o/main.o main.cpp
c++ .o/main.o  -o example
./example
</pre>
Expected output:
<pre>

# Create a copy constructed class and share it between two pointers:
new Foo(0x7ffee210e220, data=foo1)
copy constructor Foo(0x7fc8744029f8, data=)
delete Foo(0x7ffee210e220, data=foo1)
sptr1 ref count now 1
sptr2 ref count now 2

# Try to create a deadlock:
other use_count now 3
sptr1 ref count now 2
other use_count now 3
sptr2 ref count now 2

# Undo the 'deadlock':
other use_count now 0
sptr1 ref count now 2
other use_count now 0
sptr2 ref count now 2

# Release the shared sptrs, expect foo1 to be destroyed:
sptr1 ref count now 0
delete Foo(0x7fc8744029f8, data=foo1)
sptr2 ref count now 0

# You can also create shared pointers WITHOUT copy constructor overhead
new Foo(0x7fc8744029b0, data=foo0)
sptr0 = Foo(0x7fc8744029b0, data=foo0)
delete Foo(0x7fc8744029b0, data=foo0)
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
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o .o/main.o main.cpp
c++ .o/main.o  -o example
./example
</pre>
Expected output:
<pre>

# create a class and share it between two pointers:
new Foo(0x7ffee2f70658, data=foo1-data)
[foo1]: MySharedPtr::make_shared MySharedPtr(0x7ffee2f70688,Foo(0x7feff74029c8, data=foo1-data))
delete Foo(0x7ffee2f70658, data=foo1-data)
sptr1 ref count now 1
sptr2 ref count now 2

# release the shared sptrs, expect foo1 to be destroyed:
[foo1]: MySharedPtr::reset MySharedPtr(0x7ffee2f70688,Foo(0x7feff74029c8, data=foo1-data))
sptr1 ref count now 0
[foo1]: MySharedPtr::reset MySharedPtr(0x7ffee2f70608,Foo(0x7feff74029c8, data=foo1-data))
delete Foo(0x7feff74029c8, data=foo1-data)
sptr2 ref count now 0
[foo1]: MySharedPtr::delete MySharedPtr(0x7ffee2f70608)
[foo1]: MySharedPtr::delete MySharedPtr(0x7ffee2f70688)
</pre>
How to use std::unique_ptr
==========================

Unique pointers are just that. They cannot be shared. If you try to share
them you will find that an assign happens instead and the ownership of the
pointer moves, leaving the old unique pointer looking at nullptr.

To move unique pointers between owners, use std::move as below

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
c++ -std=c++2a -Werror -g -ggdb3 -O2 -Wall -c -o .o/main.o main.cpp
c++ .o/main.o  -o example
./example
</pre>
Expected output:
<pre>

# create a class and share it between two pointers:
new Foo(0x7ffeef188758, data=foo1)
delete Foo(0x7ffeef188758, data=foo1)
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
delete Foo(0x7fdd47d000c8, data=foo1)
sptr2 ref count now 0
</pre>
