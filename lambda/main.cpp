#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include "../common/common.h"

int main() {
    DOC("Create a vector of unsorted ints:");
    std::initializer_list< int > init1 = {42, 6, 7, 1, 4, 2, 3};
    std::vector< int > vec1(init1);

    DOC("Sort via lamda sorter:");
    std::sort(vec1.begin(), vec1.end(), [](const int &a, const int &b) { return a < b; });
    for (auto &i : vec1) {
        std::cout << "vec1: walk " << i << std::endl;
    }

    DOC("Modify vector via lamda modifier:");
    std::for_each(vec1.begin(), vec1.end(), [](int &a) { a++; });
    for (auto &i : vec1) {
        std::cout << "vec1: walk " << i << std::endl;
    }

    DOC("Modify vector via lamda modifier that specifies return type:");
    std::for_each(vec1.begin(), vec1.end(), [](int &a) -> void { a++; });
    for (auto &i : vec1) {
        std::cout << "vec1: walk " << i << std::endl;
    }

    DOC("Modify vector via lamda modifier that specifies return type:");
    std::transform(vec1.begin(), vec1.end(), vec1.begin(), [](int &a) -> int { return a + 1; });
    for (auto &i : vec1) {
        std::cout << "vec1: walk " << i << std::endl;
    }

    DOC("Modify vector via lamda modifier with [=] capture:");
    auto delta = 1;
    std::for_each(vec1.begin(), vec1.end(), [=](int &a) { a+=delta; });
    for (auto &i : vec1) {
        std::cout << "vec1: walk " << i << std::endl;
    }

    DOC("Modify vector via lamda modifier with [&] capture:");
    std::for_each(vec1.begin(), vec1.end(), [&](int &a) { a+=delta; delta++; });
    for (auto &i : vec1) {
        std::cout << "vec1: walk " << i << std::endl;
    }

    DOC("Auto lambda:");
    auto lambda = [](auto x, auto y) {return x + y;};
    std::initializer_list< float > init2 = {42.1, 6.2, 7.3, 1.0, 4.4, 2.5, 3.6};
    std::vector< float > vec2(init2);

    std::sort(vec2.begin(), vec2.end(), lambda);
    for (auto &i : vec2) {
        std::cout << "vec2: walk " << std::setprecision(4) << i << std::endl;
    }

    DOC("Lambda with modification:");
    {
        auto zaphod_head = 1;
        auto new_head = [&zaphod_head](){zaphod_head++;};
        new_head();
        std::cout << "Zaphod has " << zaphod_head << " heads" << std::endl;
    }

    DOC("Lambda with modification:");
    {
        auto marvin_head = 1;
        auto new_head = [=]() mutable {marvin_head++;};
        new_head();
        std::cout << "Marvin has " << marvin_head << " heads" << std::endl;
    }

    DOC("Lambda with modification in the capture:");
    auto the_meaning = 36;
    auto deep_thought = [&life = the_meaning, the_meaning = the_meaning + 1] () {
                life += 2;
                std::cout << "life        = " << life << std::endl;
                return the_meaning / 6;
            };

    deep_thought();
    deep_thought();
    auto the_answer = deep_thought();

    std::cout << "the_meaning = " << the_meaning << std::endl;
    std::cout << "the_answer  = " << the_answer << std::endl;
}
