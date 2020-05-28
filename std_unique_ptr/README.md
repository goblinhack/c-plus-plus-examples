How to use std::unique_ptr
==========================

Unique pointers are just that. They cannot be shared. If you try to share
them you will find that an assign happens instead and the ownership of the
pointer moves, leaving the old unique pointer looking at nullptr.

To create a unique pointer you can either use the following which is a bit
long winded
```C++
    std::unique_ptr<class Foo> uptr(new Foo("foo2"));
```
Or, the following which hides the use of "new" but also invokes the copy
constructor. If you don't want that, use the above.
```C++
    auto uptr = std::make_unique< class Foo >(Foo("foo1"));
```
Note: To change the ownership of a unique pointer, use std::move

Example:
```C++
#include <memory>
#include <iostream>
#include <sstream>

class Foo {
private:
    std::string data; // local data for foo for debugging
public:
    Foo(std::string data) : data(data) {
        std::cout << "new " << to_string() << std::endl;
    }
    Foo(const Foo& other) {
        std::cout << "copy constructor " << to_string() << std::endl;
        data = other.data;
    }
    ~Foo() {
        // Note, other.reset will be called for us
        std::cout << "delete " << to_string() << std::endl;
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
    // NOTE: make_unique creates a new ptr and will invoke foo1's copy constructor:
    auto uptr1 = std::make_unique< class Foo >(Foo("foo1"));

    // NOTE: to avoid the copy, do this:
    std::unique_ptr<class Foo> uptr2(new Foo("foo2"));

    // As you cannot copy unique pointers, reassign it with move
    std::unique_ptr<class Foo> uptr3;
    uptr3 = std::move(uptr2);

    // Let's print all the unique ptrs now
    std::cout << "uptr1 = " << (uptr1.get() ? uptr1->to_string() : "nullptr") << std::endl;
    std::cout << "uptr2 = " << (uptr2.get() ? uptr2->to_string() : "nullptr") << std::endl;
    std::cout << "uptr3 = " << (uptr3.get() ? uptr3->to_string() : "nullptr") << std::endl;

    // Expect the unique ptr data to be destroyed now
}
```
To build:
<pre>
cd std_unique_ptr
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# NOTE: make_unique creates a new ptr and will invoke foo1's copy constructor:
new Foo(0x7ffee0251048, data=foo1)
copy constructor Foo(0x7fecac502880, data=)
delete Foo(0x7ffee0251048, data=foo1)

# NOTE: to avoid the copy, do this:
new Foo(0x7fecac5028a0, data=foo2)

# As you cannot copy unique pointers, reassign it with move

# Let's print all the unique ptrs now
uptr1 = Foo(0x7fecac502880, data=foo1)
uptr2 = nullptr
uptr3 = Foo(0x7fecac5028a0, data=foo2)

# Expect the unique ptr data to be destroyed now
delete Foo(0x7fecac5028a0, data=foo2)
delete Foo(0x7fecac502880, data=foo1)
</pre>
