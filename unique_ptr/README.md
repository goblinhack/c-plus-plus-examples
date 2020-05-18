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
