How to use std::function and using
==================================

This example combines two things commonly used together, function
prototypes and the new "using" keyword. The reason for this is that
function declarations have always been notoriously long winded in C e.g.:
```C++
    int callback_wrapper(int (*callback)(const int a, const int b) cb, void *userdata);
```
Splitting it up into a typedef helps with sanity:
```C++
    typedef int (*add_two_numbers_callback)(const int a, const int b);
    int callback_wrapper(add_two_numbers_callback cb, void *userdata);
    callback_wrapper(cb, nullptr);
```
But it's still a bit unintuitive and different syntax from how you would
normally define a function.

C++ makes this simpler and more consistent:
```C++
    using add_two_numbers_callback = std::function< int(const int, const int) > sumFunc;
    int callback_wrapper(add_two_numbers_callback cb, void *userdata);
    callback_wrapper(cb, nullptr);
```
Notice that "using" has replaced the weird typedef syntax and looks much cleaner.

Lambdas can also supplied identically to such callbacks e.g.:
```C++
    auto add_two_numbers_callback = ([](int a, int b) { return a + b; });
    callback_wrapper(add_two_numbers_callback, 1, 2);
```
Or with inline syntax:
```C++
    callback_wrapper(([](int a, int b) { return a + b; }), 1, 2);
```
Or for clarity or if the return type is in doubt:
```C++
    callback_wrapper(([] (int a, int b) -> int { return a + b; }), 1, 2);
```
Here is the full example:
```C++
#include <algorithm>  // for std::move
#include <functional> // for _1, _2
#include <iostream>
#include <memory>
#include <sstream>    // for std::stringstream
#include <string>
#include <utility>

static int add_two_numbers_callback (const int a, const int b) {
    return a + b;
}

//
// Old C style
//
typedef int (*old_style_callback)(const int a, const int b);

static int old_function (old_style_callback cb, const int a, const int b) {
    auto result = cb(a, b);
    std::cout << "cb(" << a << ", " << b << ") = " << result << std::endl;
    return result;
}

//
// New C++ style
//
using new_style_callback = std::function< int(const int, const int) >;

std::function< int(const int, const int) > this_is_ok_too;

static int new_function (new_style_callback cb, const int a, const int b) {
    auto result = cb(a, b);
    std::cout << "cb(" << a << ", " << b << ") = " << result << std::endl;
    return result;
}

int main(int, char**)
{
    // invoke old C style typedef callback
    old_function(add_two_numbers_callback, 1, 2);

    // invoke new C++ style std::function callback
    new_function(add_two_numbers_callback, 1, 2);

    // invoke with a lambda (non inline syntax)
    auto lambda_add_two_numbers_callback = ([](int a, int b) { return a + b; });
    new_function(lambda_add_two_numbers_callback, 1, 2);

    // invoke with a lambda (inline syntax)
    new_function([] (int a, int b) { return a + b; }, 1, 2);

    // invoke with a lambda (inline longer syntax)
    new_function([] (int a, int b) -> int { return a + b; }, 1, 2);

    // end
}
```
To build:
<pre>
cd std_function_and_using
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# invoke old C style typedef callback
cb(1, 2) = 3

# invoke new C++ style std::function callback
cb(1, 2) = 3

# invoke with a lambda (non inline syntax)
cb(1, 2) = 3

# invoke with a lambda (inline syntax)
cb(1, 2) = 3

# invoke with a lambda (inline longer syntax)
cb(1, 2) = 3

# end
</pre>
