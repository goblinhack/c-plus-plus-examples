#include "../common/common.h"
#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
  DOC("Create a vector of strings:");
  std::initializer_list< std::string > init1 = {"elem1", "elem1"};
  std::vector< std::string >           vec1(init1);

  DOC("Range based for loop iterator with a const:");
  for (const auto &i : vec1) {
    std::cout << "vec1: walk " << i << std::endl;
  }

  DOC("Range based for loop iterator with a modifiable reference:");
  for (auto &i : vec1) {
    i += "+ stuff";
    std::cout << "vec1: walk " << i << std::endl;
  }

  DOC("Range based for loop iterator with forward reference:");
  for (auto &&i : vec1) {
    i += "+ more stuff";
    std::cout << "vec1: walk " << i << std::endl;
  }
}
