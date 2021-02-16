How to use std::thread and std::mutex to wrap a blocking function
=================================================================

This example creates a variadic template that can be used to wrap a 
function with variable arguments and types. We can then call that
function like a callback to allow us to easily wrap any function.

Here is a full example:
```C++
#include <iostream>

int my_wrapped_function (int x, const std::string y)
{
    // Hello from my_wrapped_function(x=" << x << ", y=" << y << ")
    return 43;
}

void my_argument_modifier (int &x)
{
    DOC("Hello from my_argument_modifier(x=" << x << ") => " << x + 1);
    x++;
}


template<typename ret, typename T, typename... Rest>
using fn = std::function<ret(T, Rest...)>;

template<typename ret, typename T, typename... Rest>
ret wrapper(fn<ret, T, Rest...> f, T t, Rest... rest)
{
    return f(t, rest...);
}

template<typename ret, typename T, typename... Rest>
ret wrapper(fn<ret, T&, Rest&...> f, T& t, Rest&... rest)
{
    return f(t, rest...);
}

int main()
{
    // Wrap a function with variable arguments

    auto f1 = fn<int,int,const std::string>(my_wrapped_function);
    auto result = wrapper(f1, 42, std::string("hello"));
    DOC("Result is " << result);

    auto f2 = fn<void,int&>(my_argument_modifier);
    wrapper(f2, result);
    DOC("Result is " << result);

    // End

    return 0;
}

```
To build:
<pre>
cd std_function_with_variadic_template
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# Wrap a function with variable arguments

# Hello from my_wrapped_function(x=42, y=hello)

# Result is 43

# Hello from my_argument_modifier(x=43) => 44

# Result is 44

# End
</pre>
