#include "../common/common.h"
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
  DOC("Various enum accessors:");
  std::cout << "ClassColors::GREEN     = " << ClassColors::GREEN << std::endl;
  std::cout << "GlobalColors::GREEN    = " << GlobalColors::GREEN << std::endl;
  std::cout << "GREEN                  = " << GREEN << std::endl;
  std::cout << "::GREEN                = " << ::GREEN << std::endl;
}
