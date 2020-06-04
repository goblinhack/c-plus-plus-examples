How to use std::thread
======================

Do you want a portable standard way to start and stop threads? Of course
you do! Enter std::thread
TODO
```C++
```
Here is a full example:
```C++
#include <iostream>
#include <sstream>
#include <thread>
#include <string>

class MyThread {
public:
    ~MyThread () {
        std::cout << to_string() << " destructor" << std::endl;
        if (my_thread.joinable()) {
            my_thread.join();
        }
    }
    MyThread (const std::string& data, const int count) : data(data), count(count) {
        my_thread = std::thread(&MyThread::run, this);
        std::cout << to_string() << " constructor" << std::endl;
    }
    std::string to_string (void) const {
        auto address = static_cast<const void*>(this);
        std::stringstream ss;
        ss << address;
        return "MyThread(" + ss.str() + ")";
    }
    void run() {
        while (count--) {
            std::cout << data;
        }
    }
private:
    std::thread my_thread;
    std::string data;
    int count;
};

static void test()
{
    // Create two threads:
    MyThread thread1("A", 100);
    MyThread thread2("B", 100);
    // Wait for threads to finish
}

int main()
{
    test();
    // End
    return 0;
}
```
To build:
<pre>
cd std_thread
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# Create two threads:
AAAAAAAAAAAAAAAAMyThread(0x7ffeeb9d1668)A constructorA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAMyThread(0x7ffeeb9d1618) constructor

# Wait for threads to finish
MyThread(0x7ffeeb9d1618) destructor
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBMyThread(0x7ffeeb9d1668) destructor

# End
</pre>
