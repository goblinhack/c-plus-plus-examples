#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <utility>
#include "../common/common.h"

int main(int, char**)
{
    const std::string a = "Zaphod";
    const std::string b = "Marvin";
    const std::string c = "The Mice";
    std::initializer_list< std::string > i = { a, b, c };

    DOC("Use std::for_each to modify a list of strings");
    {
        std::list< std::string > l(i);
        std::for_each(l.begin(), l.end(),
                      [](std::string& s) {
                          s += " is modified via std::for_each";
                      });
        for (auto e : l) { std::cout << "list-elem is " << e << std::endl; }
    }

    DOC("Use std::transform to modify a list of strings");
    {
        std::list< std::string > l(i);
        std::transform(l.begin(), l.end(), l.begin(),
                       [](std::string& s) {
                           return s + " is modified via std::transform";
                       });
        for (auto e : l) { std::cout << "list-elem is " << e << std::endl; }
    }

    DOC("Use std::transform to write to a new list");
    {
        std::list< std::string > oldlist(i);
        std::list< std::string > newlist;
        std::transform(oldlist.begin(), oldlist.end(), std::back_inserter(newlist),
                       [](std::string& s) {
                           return s + " is modified via std::transform";
                       });
        for (auto e : oldlist) { std::cout << "oldlist-elem is " << e << std::endl; }
        for (auto e : newlist) { std::cout << "newlist-elem is " << e << std::endl; }
    }
}
