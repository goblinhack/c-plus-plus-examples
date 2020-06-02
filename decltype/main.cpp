#include <iostream>
#include <cxxabi.h>  // needed for abi::__cxa_demangle
#include "../common/common.h"

//
// See
// https://stackoverflow.com/questions/4939636/function-to-mangle-demangle-functions
//
auto cppDemangle (const char *abiName)
{
    //
    // This function allocates and returns storage in ret
    //
    int status;
    char *ret = abi::__cxa_demangle(abiName, 0 /* output buffer */, 0 /* length */, &status);

    auto deallocator = ( [](char *mem) { if (mem) free((void*)mem); } );

    if (status) {
        // 0: The demangling operation succeeded.
        // -1: A memory allocation failure occurred.
        // -2: mangled_name is not a valid name under the C++ ABI mangling rules.
        // -3: One of the arguments is invalid.
        std::unique_ptr<char, decltype(deallocator) > retval(nullptr, deallocator);
    }

    //
    // Create a unique pointer to take ownership of the returned string so it
    // is freed when that pointers goes out of scope
    //
    std::unique_ptr<char, decltype(deallocator) > retval(ret, deallocator);
    return retval;
}

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

static int add_two (const int a, const int b) {
    return a + b;
}

template <typename T, typename V>
auto add_two_template (const T a, const V b) -> decltype(a + b) {
    return a + b;
}

int main(void) {
    DOC("What is the meaning...:");
    std::cout << "the meaning is " << life << std::endl;

    DOC("What type is life...:");
    std::cout << typeid(decltype(life)).name() << std::endl;
    std::cout << cppDemangle(typeid(decltype(life)).name()) << std::endl;

    DOC("How big is the earth...:");
    std::cout << sizeof(the_earth) << std::endl;

    DOC("What type is the earth...:");
    std::cout << typeid(decltype(the_earth)).name() << std::endl;
    std::cout << cppDemangle(typeid(decltype(the_earth)).name()) << std::endl;

    DOC("How big is the universe...:");
    std::cout << the_universe.get_size() << std::endl;

    DOC("What type is the universe...:");
    std::cout << typeid(decltype(the_universe)).name() << std::endl;
    std::cout << cppDemangle(typeid(decltype(the_universe)).name()) << std::endl;

    auto lambda_add_two = ( [](const int a, const int b) { return a + b; } );
    DOC("What type is a lambda_add_two...:");
    std::cout << typeid(decltype(lambda_add_two)).name() << std::endl;
    std::cout << cppDemangle(typeid(decltype(lambda_add_two)).name()) << std::endl;

    DOC("What type is a lambda_add_two(1, 2)...:");
    std::cout << cppDemangle(typeid(decltype(lambda_add_two(1, 2))).name()) << std::endl;

    DOC("What type is add_two()...:");
    std::cout << typeid(decltype(add_two)).name() << std::endl;
    std::cout << cppDemangle(typeid(decltype(add_two)).name()) << std::endl;

    DOC("What type is add_two(1, 2)...:");
    std::cout << cppDemangle(typeid(decltype(add_two(1, 2))).name()) << std::endl;
    add_two(1, 2); // just to silence compiler warnings

    DOC("What type is add_two_template<int, int>()...:");
    std::cout << cppDemangle(typeid(decltype(add_two_template<int, int>)).name()) << std::endl;
    add_two(1, 2); // just to silence compiler warnings

    DOC("What type is add_two_template<float, int>()...:");
    std::cout << cppDemangle(typeid(decltype(add_two_template<float, int>)).name()) << std::endl;
    add_two(1, 2); // just to silence compiler warnings

    DOC("What type is add_two_template<int, float>()...:");
    std::cout << cppDemangle(typeid(decltype(add_two_template<int, float>)).name()) << std::endl;
    add_two(1, 2); // just to silence compiler warnings

    DOC("What type is add_two_template<float, float>()...:");
    std::cout << cppDemangle(typeid(decltype(add_two_template<float, float>)).name()) << std::endl;
    add_two(1, 2); // just to silence compiler warnings

    DOC("What type is add_two_template((int)1, (int)2)...:");
    std::cout << cppDemangle(typeid(decltype(add_two_template((int)1, (int)2))).name()) << std::endl;
    add_two(1, 2); // just to silence compiler warnings

    DOC("What type is add_two_template((float)1.1, (int)2)...:");
    std::cout << cppDemangle(typeid(decltype(add_two_template((float)1.1, (int)2))).name()) << std::endl;
    add_two(1, 2); // just to silence compiler warnings

    DOC("What type is add_two_template((int)1, (float)2.2)...:");
    std::cout << cppDemangle(typeid(decltype(add_two_template((int)1, (float)2.2))).name()) << std::endl;
    add_two(1, 2); // just to silence compiler warnings

    DOC("What type is add_two_template((float)1.1, (float)2.2)...:");
    std::cout << cppDemangle(typeid(decltype(add_two_template((int)1.1, (float)2.2))).name()) << std::endl;
    add_two(1, 2); // just to silence compiler warnings

    return 0;
}
