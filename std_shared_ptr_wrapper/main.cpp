#include "../common/common.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

template < typename T > class MySharedPtr
{
private:
  std::shared_ptr< T > sptr;
  std::string          name {"nullptr"};

  void debug(const std::string &what) { std::cout << name << ": " << what << " " << to_string() << std::endl; }

  std::string to_string(void)
  {
    auto              address = static_cast< const void              *>(this);
    std::stringstream ss;
    ss << address;
    if (sptr) {
      return "MySharedPtr(" + ss.str() + "," + sptr->to_string() + ")";
    } else {
      return "MySharedPtr(" + ss.str() + ")";
    }
  }

public:
  // explicit means constructor must match exactly
  template < typename... ARGS > explicit MySharedPtr(const std::string &name, ARGS... a) : name(name)
  {
    sptr = std::make_shared< T >(a...);
    debug("MySharedPtr::make_shared");
  }

  explicit MySharedPtr(const std::string &name) : name(name)
  {
    sptr = std::make_shared< T > ();
    debug("MySharedPtr::make_shared");
  }

  explicit MySharedPtr(void) { debug("MySharedPtr::default constructor"); }

  ~MySharedPtr() { debug("MySharedPtr::delete"); }

  T *const operator->()
  {
    debug("MySharedPtr::-> dereference");
    return sptr.operator->();
  }

  T *get() const
  {
    debug("MySharedPtr::get ptr");
    return sptr.get();
  }

  T &operator*()
  {
    debug("MySharedPtr::* ptr");
    return *sptr;
  }

  const T &operator*() const
  {
    debug("MySharedPtr::const * ptr");
    return *sptr;
  }

  operator bool() const
  {
    debug("MySharedPtr::bool");
    return (bool) sptr;
  }

  size_t use_count(void) const { return sptr.use_count(); }

  void reset()
  {
    debug("MySharedPtr::reset");
    sptr.reset();
  }
};

typedef MySharedPtr< class Foo > Foop;

class Foo
{
private:
  std::string data;
  void        debug(const std::string &what) { std::cout << what << " " << to_string() << std::endl; }

public:
  Foo(std::string data) : data(data) { debug("new"); }
  ~Foo() { debug("delete"); }
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
  DOC("create a class and share it between two pointers:");
  auto sptr1 = MySharedPtr< class Foo >("[foo1]", Foo("foo1-data"));
  std::cout << "sptr1 ref count now " << sptr1.use_count() << std::endl;
  auto sptr2 = sptr1;
  std::cout << "sptr2 ref count now " << sptr2.use_count() << std::endl;

  DOC("release the shared sptrs, expect foo1 to be destroyed:");
  sptr1.reset();
  std::cout << "sptr1 ref count now " << sptr1.use_count() << std::endl;
  sptr2.reset();
  std::cout << "sptr2 ref count now " << sptr2.use_count() << std::endl;
}
