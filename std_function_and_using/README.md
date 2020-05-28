How to use std::function and "using"
====================================

This example combines two things commonly seen together, std::function and
the new C++11 "using" keyword. The reason they are often seen together is
that creating function types for callbacks has always been notoriously long
winded. std::function and "using" just make things simpler. For example in
the days of C yore we used to have:
```C++
    int callback_wrapper(int (*add_two_numbers_callback)(const int a, const int b) cb, void *userdata);
    callback_wrapper(cb, 1, 2);
```
Not pretty. Splitting it up into a typedef helps a bit:
```C++
    typedef int (*add_two_numbers_callback)(const int a, const int b);
    int callback_wrapper(add_two_numbers_callback cb, void *userdata);
    callback_wrapper(cb, 1, 2);
```
But it's still a bit unintuitive and different syntax from how you would
normally define a function. C++11 makes this simpler and more consistent.
Notice that the "using" keyword has replaced the weird typedef syntax and
looks much cleaner:
```C++
    using add_two_numbers_callback = std::function< int(const int, const int) > sumFunc;
    int callback_wrapper(add_two_numbers_callback cb, void *userdata);
    callback_wrapper(cb, 1, 2);
```
Lambdas can also be supplied identically to such callbacks e.g.:
```C++
    auto add_two_numbers_callback = ([](int a, int b) { return a + b; });
    callback_wrapper(add_two_numbers_callback, 1, 2);
```
Or with inline lambda syntax:
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

    // invoke the old typedef handler with a lambda, this should be equivalent
    old_function([] (int a, int b) -> int { return a + b; }, 1, 2);

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

# invoke the old typedef handler with a lambda, this should be equivalent
cb(1, 2) = 3

# end
</pre>
