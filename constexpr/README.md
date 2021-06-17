How to use constexpr, but probably not for fame and profit
==========================================================

Constants are one of the evolutions from "#define" of the C world but have many more uses.

- Whereas a "#define" would not be visible in the debugger, consts have the potential to be so.
- Consts (static ones) can also be optimized by the linker to take up no store space.
- And of course they make your code more readable and safer.

However they have limits. You can do all the below with *just* const:
```C++
    #include <iostream>

    auto what_is_the (int meaning, int of) {
        meaning++;
        return meaning * of;
    }

    const auto meaning = 5;
    const auto of = 7;
    const auto life = what_is_the(meaning, of);

    int main(void) {
        std::cout << "the meaning is " << life << std::endl;
        return 0;
    }
```
But you would *not* be allowed to do this:
```C++
    int what_is_the (int meaning, int of) {
        meaning++;
        return meaning * of;
    }
    const int life = what_is_the(meaning, of);
```
However if you add "constexpr" then bingo:
```C++
    constexpr int what_is_the (int meaning, int of) {
        meaning++;
        return meaning * of;
    }
    const int life = what_is_the(meaning, of);
```
If you wish a constexpr to be visible to other modules at link time then you 
must do the following at the point of declaration of the constexpr:
```C++
    extern constexpr auto the_universe = TheUniverse(sizeof(the_earth) * planets);
```
If you try the below you should see the symbol:
```bash
    c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
    c++ main.o  -o example
    nm -gU example | grep _the_universe
    0000000100001ea4 S _the_universe
```
If you want to have a constexpr in a header file made visible to other
files you are compiling you can use the concept of "C++17 inline variables":
```C++
    inline constexpr int the_meaning_of = 42;
```
See [this link](https://stackoverflow.com/questions/30208685/how-to-declare-constexpr-extern)
for more information.

So in summary:
- const means ‘‘I promise not to change this value’’
- constexpr means ‘‘to be evaluated at compile time’’

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
g++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
g++ main.o  -o example
./example
</pre>
Expected output:
<pre>

[31;1;4mWhat is the meaning...:[0m
the meaning is 42

[31;1;4mHow big is the earth...:[0m
42

[31;1;4mHow big is the universe...:[0m
4200000
</pre>
