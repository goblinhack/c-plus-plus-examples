#include "../common/common.h"
#include <functional> // for _1, _2
#include <iostream>
#include <memory>
#include <sstream>
#include <string.h>

using namespace std::placeholders; // for _1, _2, _3...

static auto cpp_strdup1(const char *s)
{
  char *mem  = strdup(s);
  auto  addr = static_cast< const void  *>(mem);
  std::cout << "copy " << mem << " addr " << addr << std::endl;

  auto lambda_deleter = ([](char *mem) {
    if (mem) {
      auto addr = static_cast< const void * >(mem);
      std::cout << "free " << mem << " addr " << addr << std::endl;
      free((void *) mem);
    }
  });

  std::unique_ptr< char, decltype(lambda_deleter) > retval(mem, lambda_deleter);
  return retval;
}

static void mydeleter(char *mem) noexcept
{
  auto addr = static_cast< const void * >(mem);
  std::cout << "free " << mem << " addr " << addr << std::endl;
  free((void *) mem);
}

static auto cpp_strdup2(const char *s)
{
  char *mem  = strdup(s);
  auto  addr = static_cast< const void  *>(mem);
  std::cout << "copy " << mem << " addr " << addr << std::endl;
  std::unique_ptr< char, std::function< void(char *) > > retval(mem, mydeleter);
  return retval;
}

static auto cpp_strdup3(const char *s)
{
  char *mem  = strdup(s);
  auto  addr = static_cast< const void  *>(mem);
  std::cout << "copy " << mem << " addr " << addr << std::endl;
  std::unique_ptr< char, decltype(&mydeleter) > retval(mem, mydeleter);
  return retval;
}

static auto cpp_strdup4(const char *s)
{
  char *mem  = strdup(s);
  auto  addr = static_cast< const void  *>(mem);
  std::cout << "copy " << mem << " addr " << addr << std::endl;
  auto                                  fn = std::bind(mydeleter, _1);
  std::unique_ptr< char, decltype(fn) > retval(mem, fn);
  return retval;
}

int main(void)
{
  DOC("Cpp strdup wrapper with lambda deleter");
  auto p1 = cpp_strdup1("hello");
  std::cout << p1.get() << std::endl;

  DOC("Cpp strdup wrapper with std::function deleter");
  auto p2 = cpp_strdup2("there");
  std::cout << p2.get() << std::endl;

  DOC("Cpp strdup wrapper with decltype(&mydeleter) deleter");
  auto p3 = cpp_strdup3("Zaphod");
  std::cout << p3.get() << std::endl;

  DOC("Cpp strdup wrapper with std::bind deleter");
  auto p4 = cpp_strdup4("Beeblebrox");
  std::cout << p4.get() << std::endl;

  DOC("End, expect memory to be freed");
}
