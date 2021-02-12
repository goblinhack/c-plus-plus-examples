How to use variadic templates
=============================

Do you want to have a template with a variable number of arguments?
And do you want to have different types for each? Are you insane? Of
course you want this!

The trick here is to think recursively when processing the arguments.
To do this you typically need a base worker template function and
then one at a higher level to do the recursion for you.

Here is a full example:
```C++
#include <iostream>
#include <sstream>

template <typename T>
void write_one(std::ostringstream & out, T t) {
    out << t << " ";
}

template <typename T, typename... Rest>
void write_one(std::ostringstream & out, T t, Rest... rest) {
    write_one(out, t);
    write_one(out, rest...);
}

template <typename T, typename ...Rest>
void write(T t, Rest... rest)
{
    std::ostringstream out;
    write_one(out, t, rest...);
    std::cout << out.str() << std::endl;
}

int main(void)
{
    // Combine various arguments of different types into a string
    write("The", "meaning", "of", "life", "is", 42.0, "or", 6, "times", 7);
    // End
    return 0;
}
```
To build:
<pre>
cd variadic_templates
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# Combine various arguments of different types into a string
The meaning of life is 42 or 6 times 7 

# End
</pre>
