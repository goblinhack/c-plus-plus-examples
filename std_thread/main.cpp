#include <iostream>
#include <sstream>
#include <thread>
#include <string>
#include "../common/common.h"

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
    DOC("Create two threads:");
    MyThread thread1("A", 1000);
    MyThread thread2("B", 1000);
    DOC("Wait for threads to finish");
}

int main()
{
    test();
    DOC("End");
    return 0;
}
