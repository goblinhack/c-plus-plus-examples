#include <iostream>
#include "../common/common.h"

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
    DOC("What is the meaning...:");
    std::cout << "the meaning is " << life << std::endl;

    DOC("How big is the earth...:");
    std::cout << sizeof(the_earth) << std::endl;

    DOC("How big is the universe...:");
    std::cout << the_universe.get_size() << std::endl;

    return 0;
}
