How to use lambdas
==================

Lambdas are like inline functions and very useful in a number of places where
you need a one off function. For example:
```C++
    std::initializer_list< int > i = {42, 6, 7, 1, 4, 2, 3};
    std::vector< int > v(i);
    std::sort(v.begin(), v.end(), [](const int &a, const int &b) { return a < b; });
```
This invokes a lambda at each point in the sort algorighm and is identical to:
```C++
    static int my_sorter (const int &a, const int &b) { return a < b; };
    std::initializer_list< int > i = {42, 6, 7, 1, 4, 2, 3};
    std::vector< int > v(i);
    std::sort(v.begin(), v.end(), my_sorter);
}
```
Now the syntax is a bit obscure. "[]" is defined as the capture set and indicates
what kind of things are accessible within the lambda:
- [] means capture everything
- [&zaphod] means capture the lvalue zaphod by reference so it can be modified
- [=zaphod] capture by value
- "()" are the arguements to the lambda
- "->" is used to specify the return type if that cannot be derived or is not 
clear to the reader.

Lambas can also be generic. This is very similar to templating e.g.:
```C++
    auto lambda = [](auto x, auto y) {return x + y;};
```
You can also do odd things like the following. "life" here is created on the
fly as a local variable and can be incremented each time "deep_thought()" is
invoked.
```C++
    auto the_meaning = 42;
    auto deep_thought = [&life = the_meaning, the_meaning = the_meaning + 1] () {
                life += 2;
                return the_meaning + 2;
            };
    auto the_answer = deep_thought();
    deep_thought();
```
Here is a full example:
```C++
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>

int main() {
    // Create a vector of unsorted ints:
    std::initializer_list< int > init1 = {42, 6, 7, 1, 4, 2, 3};
    std::vector< int > vec1(init1);

    // Sort via lamda sorter:
    std::sort(vec1.begin(), vec1.end(), [](const int &a, const int &b) { return a < b; });
    for (auto &i : vec1) {
        std::cout << "vec1: walk " << i << std::endl;
    }

    // Modify vector via lamda modifier:
    std::for_each(vec1.begin(), vec1.end(), [](int &a) { a++; });
    for (auto &i : vec1) {
        std::cout << "vec1: walk " << i << std::endl;
    }

    // Modify vector via lamda modifier that specifies return type:
    std::for_each(vec1.begin(), vec1.end(), [](int &a) -> void { a++; });
    for (auto &i : vec1) {
        std::cout << "vec1: walk " << i << std::endl;
    }

    // Modify vector via lamda modifier that specifies return type:
    std::transform(vec1.begin(), vec1.end(), vec1.begin(), [](int &a) -> int { return a + 1; });
    for (auto &i : vec1) {
        std::cout << "vec1: walk " << i << std::endl;
    }

    // Modify vector via lamda modifier with [=] capture:
    auto delta = 1;
    std::for_each(vec1.begin(), vec1.end(), [=](int &a) { a+=delta; });
    for (auto &i : vec1) {
        std::cout << "vec1: walk " << i << std::endl;
    }

    // Modify vector via lamda modifier with [&] capture:
    std::for_each(vec1.begin(), vec1.end(), [&](int &a) { a+=delta; delta++; });
    for (auto &i : vec1) {
        std::cout << "vec1: walk " << i << std::endl;
    }

    // Auto lambda:
    auto lambda = [](auto x, auto y) {return x + y;};
    std::initializer_list< float > init2 = {42.1, 6.2, 7.3, 1.0, 4.4, 2.5, 3.6};
    std::vector< float > vec2(init2);

    std::sort(vec2.begin(), vec2.end(), lambda);
    for (auto &i : vec2) {
        std::cout << "vec2: walk " << std::setprecision(4) << i << std::endl;
    }

    // Lambda with modification in the capture:
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
```
To build:
<pre>
cd lambda
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# Create a vector of unsorted ints:

# Sort via lamda sorter:
vec1: walk 1
vec1: walk 2
vec1: walk 3
vec1: walk 4
vec1: walk 6
vec1: walk 7
vec1: walk 42

# Modify vector via lamda modifier:
vec1: walk 2
vec1: walk 3
vec1: walk 4
vec1: walk 5
vec1: walk 7
vec1: walk 8
vec1: walk 43

# Modify vector via lamda modifier that specifies return type:
vec1: walk 3
vec1: walk 4
vec1: walk 5
vec1: walk 6
vec1: walk 8
vec1: walk 9
vec1: walk 44

# Modify vector via lamda modifier that specifies return type:
vec1: walk 4
vec1: walk 5
vec1: walk 6
vec1: walk 7
vec1: walk 9
vec1: walk 10
vec1: walk 45

# Modify vector via lamda modifier with [=] capture:
vec1: walk 5
vec1: walk 6
vec1: walk 7
vec1: walk 8
vec1: walk 10
vec1: walk 11
vec1: walk 46

# Modify vector via lamda modifier with [&] capture:
vec1: walk 6
vec1: walk 8
vec1: walk 10
vec1: walk 12
vec1: walk 15
vec1: walk 17
vec1: walk 53

# Auto lambda:
vec2: walk 3.6
vec2: walk 2.5
vec2: walk 4.4
vec2: walk 1
vec2: walk 7.3
vec2: walk 6.2
vec2: walk 42.1

# Lambda with modification in the capture:
life        = 38
life        = 40
life        = 42
the_meaning = 42
the_answer  = 6
</pre>
