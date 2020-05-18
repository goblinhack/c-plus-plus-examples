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
new Foo(0x7fb9b3c029c8, data=foo1-data)
new Foo(0x7fb9b3c02a38, data=foo2-data)
delete Foo(0x7fb9b3c02a38, data=foo2-data)
delete Foo(0x7fb9b3c029c8, data=foo1-data)
</pre>
