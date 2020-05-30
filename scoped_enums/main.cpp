#include <iostream>
#include <algorithm>
#include <vector>
#include <initializer_list>
#include "../common/common.h"

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
    DOC("Various enum accessors:");
    std::cout << "ClassColors::GREEN     = " << ClassColors::GREEN << std::endl;
    std::cout << "GlobalColors::GREEN    = " << GlobalColors::GREEN << std::endl;
    std::cout << "GREEN                  = " << GREEN << std::endl;
    std::cout << "::GREEN                = " << ::GREEN << std::endl;
}
