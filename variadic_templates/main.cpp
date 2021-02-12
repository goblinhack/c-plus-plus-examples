#include "../common/common.h"
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
    DOC("Combine various arguments of different types into a string");
    write("The", "meaning", "of", "life", "is", 42.0, "or", 6, "times", 7);
    DOC("End");
    return 0;
}
