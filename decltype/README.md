How to use decltype
===================

"auto" can be used to derive the type of many things, but it has limitations.
For example, what is the type of this?:
```C++
    template <typename T, typename V>
    auto add_two_template (const T a, const V b) {
        return a + b;
    }
```
Is it T or V? It could be either, or some derived type. This is where decltype
comes to the rescue"
```C++
    template <typename T, typename V>
    auto add_two_template (const T a, const V b) -> decltype(a + b) {
        return a + b;
    }
```
You can apply this to lambdas too:
```C++
    auto lambda_add_two = ( [](const int a, const int b) { return a + b; } );
    std::cout << typeid(decltype(lambda_add_two)).name() << std::endl;
```
We are using typeid(...).name() here to print the information, however, this
is compiler specific and so the output will be mangled by the compiler.

In the following example we have a demangler (which you can ignore) but it
prints slightly more verbose output using g++ abi interface:
```C++
#include <cxxabi.h> // needed for abi::__cxa_demangle
#include <iostream>
#include <memory>

//
// See
// https://stackoverflow.com/questions/4939636/function-to-mangle-demangle-functions
//
auto cppDemangle(const char *abiName)
{
  //
  // This function allocates and returns storage in ret
  //
  int   status;
  char *ret = abi::__cxa_demangle(abiName, 0 /* output buffer */, 0 /* length */, &status);

  auto deallocator = ([](char *mem) {
    if (mem)
      free((void *) mem);
  });

  if (status) {
    // 0: The demangling operation succeeded.
    // -1: A memory allocation failure occurred.
    // -2: mangled_name is not a valid name under the C++ ABI mangling rules.
    // -3: One of the arguments is invalid.
    std::unique_ptr< char, decltype(deallocator) > retval(nullptr, deallocator);
  }

  //
  // Create a unique pointer to take ownership of the returned string so it
  // is freed when that pointers goes out of scope
  //
  std::unique_ptr< char, decltype(deallocator) > retval(ret, deallocator);
  return retval;
}

class TheUniverse
{
private:
  int size;

public:
  constexpr TheUniverse(int size) : size(size) {}
  constexpr int get_size() const { return (size); }
};

constexpr auto what_is_the(int meaning, int of)
{
  meaning++;
  return meaning * of;
}

// const works just as well here for all the below
constexpr auto meaning = 5;
constexpr auto of      = 7;
constexpr auto life    = what_is_the(meaning, of);
constexpr auto planets = 100000;
constexpr char the_earth[ life ] {};

//
// Note using constexpr will make this symbol visible in the linker
//
constexpr auto the_universe = TheUniverse(sizeof(the_earth) * planets);

static int add_two(const int a, const int b) { return a + b; }

template < typename T, typename V > auto add_two_template(const T a, const V b) -> decltype(a + b) { return a + b; }

int main(void)
{
  // What is the meaning...:
  std::cout << "the meaning is " << life << std::endl;

  // What type is life...:
  std::cout << typeid(decltype(life)).name() << std::endl;
  std::cout << cppDemangle(typeid(decltype(life)).name()) << std::endl;

  // How big is the earth...:
  std::cout << sizeof(the_earth) << std::endl;

  // What type is the earth...:
  std::cout << typeid(decltype(the_earth)).name() << std::endl;
  std::cout << cppDemangle(typeid(decltype(the_earth)).name()) << std::endl;

  // How big is the universe...:
  std::cout << the_universe.get_size() << std::endl;

  // What type is the universe...:
  std::cout << typeid(decltype(the_universe)).name() << std::endl;
  std::cout << cppDemangle(typeid(decltype(the_universe)).name()) << std::endl;

  auto lambda_add_two = ([](const int a, const int b) { return a + b; });
  // What type is a lambda_add_two...:
  std::cout << typeid(decltype(lambda_add_two)).name() << std::endl;
  std::cout << cppDemangle(typeid(decltype(lambda_add_two)).name()) << std::endl;

  // What type is a lambda_add_two(1, 2)...:
  std::cout << cppDemangle(typeid(decltype(lambda_add_two(1, 2))).name()) << std::endl;

  // What type is add_two()...:
  std::cout << typeid(decltype(add_two)).name() << std::endl;
  std::cout << cppDemangle(typeid(decltype(add_two)).name()) << std::endl;

  // What type is add_two(1, 2)...:
  std::cout << cppDemangle(typeid(decltype(add_two(1, 2))).name()) << std::endl;
  add_two(1, 2); // just to silence compiler warnings

  // What type is add_two_template<int, int>()...:
  std::cout << cppDemangle(typeid(decltype(add_two_template< int, int >)).name()) << std::endl;
  add_two(1, 2); // just to silence compiler warnings

  // What type is add_two_template<float, int>()...:
  std::cout << cppDemangle(typeid(decltype(add_two_template< float, int >)).name()) << std::endl;
  add_two(1, 2); // just to silence compiler warnings

  // What type is add_two_template<int, float>()...:
  std::cout << cppDemangle(typeid(decltype(add_two_template< int, float >)).name()) << std::endl;
  add_two(1, 2); // just to silence compiler warnings

  // What type is add_two_template<float, float>()...:
  std::cout << cppDemangle(typeid(decltype(add_two_template< float, float >)).name()) << std::endl;
  add_two(1, 2); // just to silence compiler warnings

  // What type is add_two_template((int)1, (int)2)...:
  std::cout << cppDemangle(typeid(decltype(add_two_template((int) 1, (int) 2))).name()) << std::endl;
  add_two(1, 2); // just to silence compiler warnings

  // What type is add_two_template((float)1.1, (int)2)...:
  std::cout << cppDemangle(typeid(decltype(add_two_template((float) 1.1, (int) 2))).name()) << std::endl;
  add_two(1, 2); // just to silence compiler warnings

  // What type is add_two_template((int)1, (float)2.2)...:
  std::cout << cppDemangle(typeid(decltype(add_two_template((int) 1, (float) 2.2))).name()) << std::endl;
  add_two(1, 2); // just to silence compiler warnings

  // What type is add_two_template((float)1.1, (float)2.2)...:
  std::cout << cppDemangle(typeid(decltype(add_two_template((int) 1.1, (float) 2.2))).name()) << std::endl;
  add_two(1, 2); // just to silence compiler warnings

  return 0;
}
```
To build:
<pre>
cd decltype
rm -f *.o example
g++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
g++ main.o  -o example
./example
</pre>
Expected output:
<pre>

[31;1;4mWhat is the meaning...:[0m
the meaning is 42

[31;1;4mWhat type is life...:[0m
i
int

[31;1;4mHow big is the earth...:[0m
42

[31;1;4mWhat type is the earth...:[0m
A42_c
char [42]

[31;1;4mHow big is the universe...:[0m
4200000

[31;1;4mWhat type is the universe...:[0m
11TheUniverse
TheUniverse

[31;1;4mWhat type is a lambda_add_two...:[0m
Z4mainEUliiE_
main::{lambda(int, int)#1}

[31;1;4mWhat type is a lambda_add_two(1, 2)...:[0m
int

[31;1;4mWhat type is add_two()...:[0m
FiiiE
int (int, int)

[31;1;4mWhat type is add_two(1, 2)...:[0m
int

[31;1;4mWhat type is add_two_template<int, int>()...:[0m
int (int, int)

[31;1;4mWhat type is add_two_template<float, int>()...:[0m
float (float, int)

[31;1;4mWhat type is add_two_template<int, float>()...:[0m
float (int, float)

[31;1;4mWhat type is add_two_template<float, float>()...:[0m
float (float, float)

[31;1;4mWhat type is add_two_template((int)1, (int)2)...:[0m
int

[31;1;4mWhat type is add_two_template((float)1.1, (int)2)...:[0m
float

[31;1;4mWhat type is add_two_template((int)1, (float)2.2)...:[0m
float

[31;1;4mWhat type is add_two_template((float)1.1, (float)2.2)...:[0m
float
</pre>
