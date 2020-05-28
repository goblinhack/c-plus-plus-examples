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
To reference "RED" you now need to specify which instance. So 
"ClassColors::RED", or for the global enum, "GlobalColors::RED" or "::RED"
or just plain old "RED".

Here is a full example:
```C++
#include <iostream>
#include <algorithm>
#include <vector>
#include <initializer_list>

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

int main() {
    // Create some enums with some uncertainty as to which value GREEN will have
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
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# Create some enums with some uncertainty as to which value GREEN will have
ClassColors::GREEN     = 5
GlobalColors::GREEN    = 2
GREEN                  = 2
::GREEN                = 2
</pre>
