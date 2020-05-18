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
