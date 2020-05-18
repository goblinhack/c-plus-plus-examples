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
