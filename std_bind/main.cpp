#include <algorithm>
#include <functional> // for _1, _2
#include <iostream>
#include <list>
#include <string>
#include <utility>
#include "../common/common.h"

using namespace std::placeholders; // for _1, _2, _3...

static void bind_fn1 (const std::string& arg1,
                      const std::string& arg2,
                      const std::string& arg3)
{
    std::cout << "argument 1 is " << arg1 << std::endl;
    std::cout << "argument 2 is " << arg2 << std::endl;
    std::cout << "argument 3 is " << arg3 << std::endl;
}

//
// You could cheat here and use &arg1, but that makes a mockery of 
// std::for_each
//
static void bind_try_to_modify_string (std::string arg1)
{
    arg1 = arg1 + " and is not modified and this will be ignored";
}

static std::string bind_really_modify_string (const std::string& arg1)
{
    return arg1 + " and is modified";
}

int main(int, char**)
{
    const std::string a = "cat";
    const std::string b = "dog";
    const std::string c = "ork";

    auto fn1 = bind(bind_fn1, _1, _2, _3);
    auto fn2 = bind(bind_fn1, _2, _1, _3);
    auto fn3 = bind(bind_fn1, _3, _1, _2);

    DOC("call bind_fn1(_1, _2, _3)");
    fn1(a, b, c);

    DOC("call bind_fn1(_2, _1, _3)");
    fn2(a, b, c);

    DOC("call bind_fn1(_3, _2, _1)");
    fn3(a, b, c);

    //
    // Use std::bind with std::for_each
    //
    std::initializer_list< std::string > il = { a, b, c };
    std::list< std::string > l1(il);

    DOC("call our bind function via std::for_each to modify a list of strings");
    auto fn4 = bind(bind_try_to_modify_string, _1);
    std::for_each(l1.begin(), l1.end(), fn4);
    for (auto e : l1) { std::cout << "l1 list-elem is now " << e << std::endl; }

    //
    // Use std::bind with std::transform
    //
    std::initializer_list< std::string > i2 = { a, b, c };
    std::list< std::string > l2(i2);

    DOC("call our bind function via std::transform to modify a list of strings");
    auto fn5 = bind(bind_really_modify_string, _1);
    std::transform(l2.begin(), l2.end(), l2.begin(), fn5);
    for (auto e : l2) { std::cout << "l2 list-elem is now " << e << std::endl; }
}
