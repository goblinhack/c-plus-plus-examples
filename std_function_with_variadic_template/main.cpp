#include <iostream>
#include "../common/common.h"

int my_wrapped_function (int x, const std::string y)
{
    DOC("Hello from my_wrapped_function(x=" << x << ", y=" << y << ")");
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
    DOC("Wrap a function with variable arguments");
    auto f1 = fn<int,int,const std::string>(my_wrapped_function);
    auto result = wrapper(f1, 42, std::string("hello"));
    DOC("Result should be 43: " << result);

    DOC("Wrap a function that modifies its arguments");
    auto f2 = fn<void,int&>(my_argument_modifier);
    wrapper(f2, result);
    DOC("Result should be 44: " << result);
    DOC("End");

    return 0;
}

