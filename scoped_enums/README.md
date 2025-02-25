How to use scoped enums
=======================

In days of old, C would allow this:
```C++
enum GlobalColors {
    RED = 1,
    GREEN = 2,
    BLU = 3,
};

enum BadGlobalColors {
    PURPLE = 1,
    GREEN = 66, // oops, redefinition of GREEN
    YELLOW = 3,
};
```
C++ introduces scoping of enums, so now we can avoid conflicts. For example:
```C++
enum GlobalColors {
    RED = 1,
    GREEN = 2,
    BLU = 3,
};

class ClassColors {
public:
    enum {
        RED = 4,
        GREEN = 5,
        BLUE = 6,
    };
};
```
To reference "GREEN" you now need to specify which instance. So
"ClassColors::GREEN", or for the global enum, "GlobalColors::GREEN" or "::GREEN"
or just plain old "GREEN".

Here is a full example:
```C++
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <vector>

enum GlobalColors {
  RED   = 1,
  GREEN = 2,
  BLU   = 3,
};

class ClassColors
{
public:
  enum {
    RED   = 4,
    GREEN = 5,
    BLUE  = 6,
  };
};

int main()
{
  // Various enum accessors:
  std::cout << "ClassColors::GREEN     = " << ClassColors::GREEN << std::endl;
  std::cout << "GlobalColors::GREEN    = " << GlobalColors::GREEN << std::endl;
  std::cout << "GREEN                  = " << GREEN << std::endl;
  std::cout << "::GREEN                = " << ::GREEN << std::endl;
}
```
To build:
<pre>
cd scoped_enums
rm -f *.o example
clang -std=c++2a -Werror -g -O3 -fstack-protector-all -ggdb3 -Wall -c -o main.o main.cpp
clang  main.o -lstdc++  -o example
./example
</pre>
Expected output:
<pre>

# Various enum accessors:
ClassColors::GREEN     = 5
GlobalColors::GREEN    = 2
GREEN                  = 2
::GREEN                = 2
</pre>
