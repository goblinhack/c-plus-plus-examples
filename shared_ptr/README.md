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

[31;1;4mcreate a class and share it between two pointers:[0m
new Foo(0x7ffee7731758, data=foo1)
delete Foo(0x7ffee7731758, data=foo1)
sptr1 ref count now 1
sptr2 ref count now 2

[31;1;4mtry to create a deadlock:[0m
other use_count now 3
sptr1 ref count now 2
other use_count now 3
sptr2 ref count now 2

[31;1;4mundo the deadlock:[0m
other use_count now 0
sptr1 ref count now 2
other use_count now 0
sptr2 ref count now 2

[31;1;4mrelease the shared sptrs, expect foo1 to be destroyed:[0m
sptr1 ref count now 0
delete Foo(0x7f92124029f8, data=foo1)
sptr2 ref count now 0
</pre>
