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
cd std_shared_ptr
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# Create a copy constructed class and share it between two pointers:
new Foo(0x7ffeed434720, data=foo1)
copy constructor Foo(0x7fd09bc02998, data=)
delete Foo(0x7ffeed434720, data=foo1)
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
delete Foo(0x7fd09bc02998, data=foo1)
sptr2 ref count now 0

# You can also create shared pointers WITHOUT copy constructor overhead
new Foo(0x7fd09bc02950, data=foo0)
sptr0 = Foo(0x7fd09bc02950, data=foo0)
delete Foo(0x7fd09bc02950, data=foo0)
</pre>
