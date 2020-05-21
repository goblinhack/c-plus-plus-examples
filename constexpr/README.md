How to use constexpr
====================

Constants are one of the evolutions of "#define" from the C world but have
many more uses. Whereas a "#define" would not be visible in the debugger,
consts have the potential to be so. Consts (static ones) can also be optimized
by the linker to take up no store space. And of course they make your code more
readable and safer. However they have limits. You can do all the below with
just const:
```C++
    #include <iostream>

    auto what_is_the (int meaning, int of) {
        meaning++;
        return meaning * of;
    }

    const auto meaning = 5;
    const auto of = 7;
    const auto life = what_is_the(meaning, of);

    // int the_universe[life]; variable length array declaration not allowed

    int main(void) {
        std::cout << "the meaning is " << life << std::endl;
        return 0;
}
```
But you would not be allowed to do this:
```C++
    const int life = what_is_the(meaning, of);
```
However if you add "constexpr" then bingo:
```C++
    constexpr int what_is_the (int meaning, int of) {
        meaning++;
        return meaning * of;
    }
```
Now according to book of "The C++ Programming Language 4th Editon" by Bjarne Stroustrup
- const means ‘‘I promise not to change this value’’
- constexpr means ‘‘to be evaluated at compile time’’

One final note, if you wish a constexpr to be visible to other modules you must do
the following at the point of declaration of the constexpr:
```C++
    extern constexpr auto the_universe = TheUniverse(sizeof(the_earth) * planets);
```
If you then do the following you should see the symbol:
```
    c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
    c++ main.o  -o example
    0000000100001ea4 S _the_universe
```
Ok, here is a silly example:
```C++
#include <iostream>

class TheUniverse {
private:
    int size;
public:
    constexpr TheUniverse(int size) : size(size) { }
    constexpr int get_size() const { return (size); }
};

constexpr auto what_is_the (int meaning, int of) {
    meaning++;
    return meaning * of;
}

// const works just as well here for all the below
constexpr auto meaning = 5;
constexpr auto of = 7;
constexpr auto life = what_is_the(meaning, of);
constexpr auto planets = 100000;
constexpr char the_earth[life] {};

//
// Note using constexpr will make this symbol visible in the linker
//
constexpr auto the_universe = TheUniverse(sizeof(the_earth) * planets);

int main(void) {
    // What is the meaning...:
    std::cout << "the meaning is " << life << std::endl;

    // How big is the earth...:
    std::cout << sizeof(the_earth) << std::endl;

    // How big is the universe...:
    std::cout << the_universe.get_size() << std::endl;

    return 0;
}
```
To build:
<pre>
cd constexpr
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# What is the meaning...:
the meaning is 42

# How big is the earth...:
42

# How big is the universe...:
4200000
</pre>
