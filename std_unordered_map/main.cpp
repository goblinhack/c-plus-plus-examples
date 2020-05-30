#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <map>
#include <unordered_map>
#include "../common/common.h"

static void std_unordered_map_sort (void) {
    DOC("std::unordered_map");
    std::unordered_map< std::string, int> m;

    m["zaphod"] = 1;
    m.insert(std::make_pair("universe", 2));
    m.emplace(std::make_pair("arthur", 3));
    m["marvin"] = 4;
    m["mice"] = 5;
    m["vogon"] = 6;
    m.erase("universe");

    for (auto i : m) {
        std::cout << i.first << " " << i.second << std::endl;
    }
}

static void std_map_sort (void) {
    DOC("std::map");
    std::map< std::string, int> m;

    m["zaphod"] = 1;
    m.insert(std::make_pair("universe", 2));
    m.emplace(std::make_pair("arthur", 3));
    m["marvin"] = 4;
    m["mice"] = 5;
    m["vogon"] = 6;
    m.erase("universe");

    for (auto i : m) {
        std::cout << i.first << " " << i.second << std::endl;
    }
}

int main(int, char**) {
    std_map_sort();
    std_unordered_map_sort();
}
