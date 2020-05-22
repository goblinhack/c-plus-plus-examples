#include <iostream>
#include <algorithm>
#include <vector>
#include <initializer_list>
#include "../common/common.h"

class MyString {
private:
    std::string s;
public:
    MyString() {
        auto address = static_cast<const void*>(this);
        std::cout << address << " MyString() " << std::endl;
    }
    ~MyString() {
        auto address = static_cast<const void*>(this);
        std::cout << address << " ~MyString() " << s << std::endl;
    }
    MyString(const std::string &s) : s(s) {
        auto address = static_cast<const void*>(this);
        std::cout << address << " MyString(std::string &) " << s << std::endl;
    }
    MyString(const std::string &&s) : s(s) {
        auto address = static_cast<const void*>(this);
        std::cout << address << " MyString(std::string &&) " << s << std::endl;
    }
    MyString(const MyString & o) : s(o.s) { // copy constructor
        auto address = static_cast<const void*>(this);
        std::cout << address << " MyString(const std::string &) " << s << std::endl;
    }
    MyString(const MyString && o) : s(o.s) { // move constructor
        auto address = static_cast<const void*>(this);
        std::cout << address << " MyString(const std::string &&) " << s << std::endl;
    }
    friend std::ostream& operator<<(std::ostream &os, const MyString& o) {
        return os << o.s;
    }
};

int main() {
    DOC("Create an std::initializer_list of MyString:");
    std::initializer_list< MyString > init1 = {
        MyString(std::string("elem1")), MyString(std::string("elem2"))
    };

    DOC("Assign this initializer_list to a vector:");
    std::vector< MyString > vec1(init1);

    DOC("Walk the vector with 'const auto i': (this will involve copies)");
    for (const auto i : vec1) {
        std::cout << i << std::endl;
    }

    DOC("Walk the vector with 'const auto &i': (should see no copies)");
    for (const auto &i : vec1) {
        std::cout << i << std::endl;
    }

    DOC("Walk the vector with forward reference 'auto &&i': (should see no copies)");
    for (auto &&i : vec1) {
        std::cout << i << std::endl;
    }

    DOC("Create another vector with an inline initializer list");
    // This will not work
    // std::vector< MyString > vec2 (
    //     MyString(std::string("elem3")), MyString(std::string("elem4"))
    // );
    std::vector< MyString > vec2 = {
        MyString(std::string("elem3")), MyString(std::string("elem4"))
    };

    DOC("End:");
}
