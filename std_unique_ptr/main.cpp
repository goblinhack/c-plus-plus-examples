#include "../common/common.h"
#include <iostream>
#include <memory>
#include <sstream>

class Foo
{
private:
  std::string data; // local data for foo for debugging
public:
  Foo(std::string data) : data(data) { std::cout << "new " << to_string() << std::endl; }
  Foo(const Foo &other)
  {
    std::cout << "copy constructor " << to_string() << std::endl;
    data = other.data;
  }
  ~Foo()
  {
    // Note, other.reset will be called for us
    std::cout << "delete " << to_string() << std::endl;
  }
  std::string to_string(void)
  {
    auto              address = static_cast< const void              *>(this);
    std::stringstream ss;
    ss << address;
    return "Foo(" + ss.str() + ", data=" + data + ")";
  }
};

int main(void)
{
  DOC("NOTE: make_unique creates a new ptr and will invoke foo1's copy constructor:");
  auto uptr1 = std::make_unique< class Foo >(Foo("foo1"));

  DOC("NOTE: to avoid the copy, do this:");
  std::unique_ptr< class Foo > uptr2(new Foo("foo2"));

  DOC("As you cannot copy unique pointers, reassign it with move");
  std::unique_ptr< class Foo > uptr3;
  uptr3 = std::move(uptr2);

  DOC("Let's print all the unique ptrs now");
  std::cout << "uptr1 = " << (uptr1.get() ? uptr1->to_string() : "nullptr") << std::endl;
  std::cout << "uptr2 = " << (uptr2.get() ? uptr2->to_string() : "nullptr") << std::endl;
  std::cout << "uptr3 = " << (uptr3.get() ? uptr3->to_string() : "nullptr") << std::endl;

  DOC("Expect the unique ptr data to be destroyed now");
}
