#include "../common/common.h"
#include <functional>
#include <iostream>

static int add_two_numbers_callback(const int a, const int b) { return a + b; }

//
// Old C style
//
typedef int (*old_style_callback)(const int a, const int b);

static int old_function(old_style_callback cb, const int a, const int b)
{
  auto result = cb(a, b);
  std::cout << "cb(" << a << ", " << b << ") = " << result << std::endl;
  return result;
}

//
// New C++ style
//
using new_style_callback = std::function< int(const int, const int) >;

std::function< int(const int, const int) > this_is_ok_too;

static int new_function(new_style_callback cb, const int a, const int b)
{
  auto result = cb(a, b);
  std::cout << "cb(" << a << ", " << b << ") = " << result << std::endl;
  return result;
}

int main(int, char **)
{
  DOC("invoke old C style typedef callback");
  old_function(add_two_numbers_callback, 1, 2);

  DOC("invoke new C++ style std::function callback");
  new_function(add_two_numbers_callback, 1, 2);

  DOC("invoke with a lambda (non inline syntax)");
  auto lambda_add_two_numbers_callback = ([](const int a, const int b) { return a + b; });
  new_function(lambda_add_two_numbers_callback, 1, 2);

  DOC("invoke with a lambda (inline syntax)");
  new_function([](const int a, const int b) { return a + b; }, 1, 2);

  DOC("invoke with a lambda (inline longer syntax)");
  new_function([](const int a, const int b) -> int { return a + b; }, 1, 2);

  DOC("invoke the old typedef handler with a lambda, this should be equivalent");
  old_function([](const int a, const int b) -> int { return a + b; }, 1, 2);

  DOC("end");
}
