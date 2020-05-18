How to make your own wrapper around std::shared_ptr
===================================================

Here we create a wrapper around std::shared_ptr. Why? Mostly educational,
but you could use this to keep track of memory allcoation and frees as part
of a basic leak detector.

We will create two instances of our wrapper around std::shared_ptr and then
make them point to each other. This creates a deadlock which can only be
resolved by calling reset() upon which the pointers can then be destructed.

```C++
#include <memory>
#include <sstream>
#include <string>
#include <iostream>

template <typename T> class SmartPointerWrapper {
private:
    std::shared_ptr<T> sptr;
    std::string name {"nullptr"};

    void debug (const std::string &what) {
        std::cout << what << " " << to_string() << std::endl;
    }

    std::string to_string (void) {
        auto address = static_cast<const void*>(this);
        std::stringstream ss;
        ss << address;
        if (sptr) {
            return "SmartPointerWrapper(" + ss.str() + "," + sptr->to_string() + ")";
        } else {
            return "SmartPointerWrapper(" + ss.str() + ")";
        }
    }
public:
    // explicit means constructor must match exactly
    template <typename ...ARGS> explicit 
      SmartPointerWrapper(const std::string &name, ARGS... a) : name(name) { 
        sptr = std::make_shared<T>(a...);
        debug("make_shared");
    }

    explicit SmartPointerWrapper(const std::string &name) : name(name) { 
        sptr = std::make_shared<T>();
        debug("make_shared");
    }

    explicit SmartPointerWrapper(void) {
        debug("init");
    }

    ~SmartPointerWrapper() {
        debug("delete");
    }

    void rename(const std::string &name) { 
        this->name = name;
        debug("rename");
    }

    T* const operator->() { return sptr.operator->(); }
    T* get() const { return sptr.get(); }
    T& operator*() { return *sptr; }
    const T& operator*() const { return *sptr; }
    operator bool() const { return (bool)sptr; }

    void reset() { 
        debug("reset");
        sptr.reset(); 
    }
};

typedef SmartPointerWrapper< class Foo > Foop;

class Foo {
private:
    std::string data;
    Foop other {};
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
    auto foo1 = SmartPointerWrapper< class Foo >(std::string("foo1"),
                                                 std::string("foo1-data"));
    auto foo2 = SmartPointerWrapper< class Foo >(std::string("foo2"),
                                                 std::string("foo2-data"));
    foo1->addref(foo2);
    foo2->addref(foo1);
    foo1->delref();
    foo2->delref();
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
init SmartPointerWrapper(0x7f94984029e0)
new Foo(0x7f94984029c8, data=foo1-data)
make_shared SmartPointerWrapper(0x7ffee4ea9688,Foo(0x7f94984029c8, data=foo1-data))
init SmartPointerWrapper(0x7f9498402ae0)
new Foo(0x7f9498402ac8, data=foo2-data)
make_shared SmartPointerWrapper(0x7ffee4ea9760,Foo(0x7f9498402ac8, data=foo2-data))
delete SmartPointerWrapper(0x7ffee4ea96b0,Foo(0x7f9498402ac8, data=foo2-data))
delete SmartPointerWrapper(0x7ffee4ea96d8,Foo(0x7f94984029c8, data=foo1-data))
reset SmartPointerWrapper(0x7f94984029e0)
reset SmartPointerWrapper(0x7f9498402ae0)
delete SmartPointerWrapper(0x7ffee4ea9760,Foo(0x7f9498402ac8, data=foo2-data))
delete Foo(0x7f9498402ac8, data=foo2-data)
delete SmartPointerWrapper(0x7f9498402ae0)
delete SmartPointerWrapper(0x7ffee4ea9688,Foo(0x7f94984029c8, data=foo1-data))
delete Foo(0x7f94984029c8, data=foo1-data)
delete SmartPointerWrapper(0x7f94984029e0)
</pre>
