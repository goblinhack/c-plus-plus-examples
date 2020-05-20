Modern C++ (C++11 ... C++20) examples
=====================================

Are you looking for a collection of short snippets of modern C++ code
covering core features in C++11 and beyond? Of course you are!

As I'm constantly learning C++ myself, I wanted to create some simple examples 
of each of the main features of the language. You can build each example
individually or as a whole.

Let me know if this is useful to anyone or if there are any areas you want
covered. This is (probably forever) a work in progress.

[How to print hello world in color in C++, because why not](hello_world_color/README.md)

[How to use range based for loops](range_based_for_loop/README.md)

[How to use range based for loops with your own iterator](range_based_for_loop_custom_begin_end/README.md)

[How to use std::bind with simple example](std_bind/README.md)

[How to use std::bind with methods or proxy functions](std_bind_with_a_method/README.md)

[How to use std::bind with callback inside a class](std_bind_with_a_class_callback/README.md)

[How to use std::move to avoid the cost of temporaries](std_move/README.md)

[How to use std::forward to invoke the exactly correct function through a template](std_forward/README.md)

[How to create and use std::unique_ptr](std_unique_ptr/README.md)

[How to create and use std::shared_ptr](std_shared_ptr/README.md)

[How to make a wrapper around std::shared_ptr](std_shared_ptr_wrapper/README.md)

To build all the examples, just do:
```C++
   make 
```
To build all the examples and the documentation and run every examle, do:
```C++
   sh ./RUNME
```
Hello world in color, because why not?
======================================

To get started let's do the usual "Hello World" kind of thing, but for C++.
I use a lot of colors in the output of these tools to highlight errors and
such, so it might be instructive to show how I do that before we move ahead.

ANSI colors are really nothing to do with C++, but they are fun. Such sequences
begin with the escape character (33) and act as instructions to your terminal to
turn on colors, underline, bold etc. It all depends on what your terminal can
do to get good results. For lots of detail on the types of ANSI codes you can use.
See [this link](https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences)
Many of these are used in the example below. Anyway, here we go:
```C++
    std::cout << "\033[31;1;4mHello world\033[0m" << std::endl;
```
This gives us red text (31), bold (1) and underlined (4). To reset back to
the terminal default, we send the reset sequence.

We also introduce the "std" namespace here. Some people opt to do
```C++
    use namespace std;
```
but it is considered bad practice to use this. See
[this link](https://stackoverflow.com/questions/1452721/why-is-using-namespace-std-considered-bad-practice).
Personally I find code clearer when it uses "std:" as you are left in no
doubt as to what namespace is being used.
```C++
    use namespace std;
    std::cout << ...
```
sends output to the stdout for this process. For errors, you would want to use instead:
```C++
    use namespace std;
    std::cerr << "aargh" << ...
```
For newlines, like in C where we had "\n", you have in C++:
```C++
    use namespace std;
    ... << "aargh" << std::endl;
```
Ok, so enough background. Here is the full example. The output should look like:

![Alt text](https://github.com/goblinhack/c-plus-plus-examples/blob/master/hello_world_color/screenshot.png?raw=true "hello colorful world")

```C++
#include <iostream>
#include <iomanip>
#include <vector>

class Ansi {
public:
    Ansi() {
        //
        // See
        //
        // https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences#4842438
        //
        // For a very thorough explanation of these escape codes
        //
        const std::initializer_list<std::string> code_strings = {
            /* reset                        */ "[0m",
            /* bold                         */ "[1m",
            /* faint                        */ "[2m",
            /* italic                       */ "[3m",
            /* underline                    */ "[4m",
            /* slow_blink                   */ "[5m",
            /* rapid_blink                  */ "[6m",
            /* reverse_video                */ "[7m",
            /* conceal                      */ "[8m",
            /* crossed_out                  */ "[9m",
            /* primary_font                 */ "[10m",
            /* alt_font1                    */ "[11m",
            /* alt_font2                    */ "[12m",
            /* alt_font3                    */ "[13m",
            /* alt_font4                    */ "[14m",
            /* alt_font5                    */ "[15m",
            /* alt_font6                    */ "[16m",
            /* alt_font7                    */ "[17m",
            /* alt_font8                    */ "[18m",
            /* alt_font9                    */ "[19m",
            /* fraktur                      */ "[20m",
            /* bold_off_or_double_underline */ "[21m",
            /* normal_color                 */ "[22m",
            /* not_italic                   */ "[23m",
            /* underline_off                */ "[24m",
            /* blink_off                    */ "[25m",
            /* unused                       */ "[26m",
            /* inverse_off                  */ "[27m",
            /* reveal                       */ "[28m",
            /* not_crossed_out              */ "[29m",
            /* foreground_black             */ "[30m",
            /* foreground_red               */ "[31m",
            /* foreground_green             */ "[32m",
            /* foreground_yellow            */ "[33m",
            /* foreground_blue              */ "[34m",
            /* foreground_magenta           */ "[35m",
            /* foreground_cyan              */ "[36m",
            /* foreground_white             */ "[37m",
            /* foreground_color2            */ "[38m",
            /* default_foreground_color     */ "[39m",
            /* background_black             */ "[40m",
            /* background_red               */ "[41m",
            /* background_green             */ "[42m",
            /* background_yellow            */ "[43m",
            /* background_blue              */ "[44m",
            /* background_magenta           */ "[45m",
            /* background_cyan              */ "[46m",
            /* background_white             */ "[47m",
            /* background_color2            */ "[48m",
            /* default_background_color     */ "[49m",
            /* unused2                      */ "[49m",
            /* framed                       */ "[51m",
            /* encircled                    */ "[52m",
            /* overlined                    */ "[53m",
            /* not_framed_or_encircled      */ "[54m",
            /* not_overlined                */ "[55m",
            /* unused3                      */ "[56m",
            /* unused4                      */ "[57m",
            /* unused5                      */ "[58m",
            /* unused6                      */ "[59m",
            /* underline2                   */ "[60m",
            /* double_underline             */ "[61m",
            /* overline                     */ "[62m",
            /* double_overline              */ "[63m",
            /* stress_marking               */ "[64m",
            /* attributes_off               */ "[65m",
        };
        codes = code_strings;

        //
        // Mapping of code back to descriptive name
        //
        const std::initializer_list<std::string> code_name_strings = {
            "reset",
            "bold",
            "faint",
            "italic",
            "underline",
            "slow_blink",
            "rapid_blink",
            "reverse_video",
            "conceal",
            "crossed_out",
            "primary_font",
            "alt_font1",
            "alt_font2",
            "alt_font3",
            "alt_font4",
            "alt_font5",
            "alt_font6",
            "alt_font7",
            "alt_font8",
            "alt_font9",
            "fraktur",
            "bold_off_or_double_underline",
            "normal_color",
            "not_italic",
            "underline_off",
            "blink_off",
            "unused",
            "inverse_off",
            "reveal",
            "not_crossed_out",
            "foreground_black",
            "foreground_red",
            "foreground_green",
            "foreground_yellow",
            "foreground_blue",
            "foreground_magenta",
            "foreground_cyan",
            "foreground_white",
            "foreground_color2",
            "default_foreground_color",
            "background_black",
            "background_red",
            "background_green",
            "background_yellow",
            "background_blue",
            "background_magenta",
            "background_cyan",
            "background_white",
            "background_color2",
            "default_background_color",
            "unused2",
            "framed",
            "encircled",
            "overlined",
            "not_framed_or_encircled",
            "not_overlined",
            "unused3",
            "unused4",
            "unused5",
            "unused6",
            "underline2",
            "double_underline",
            "overline",
            "double_overline",
            "stress_marking",
            "attributes_off",
        };
        code_names = code_name_strings;

        //
        // Codes for printing background and foreground together, and without
        // needing to do any number to string conversions for fast lookups.
        //
        const std::initializer_list<std::string> bgfg_code_strings = {
            "[40;30m", "[40;31m", "[40;32m", "[40;33m",
            "[40;34m", "[40;35m", "[40;36m", "[40;37m",
            "[41;30m", "[41;31m", "[41;32m", "[41;33m",
            "[41;34m", "[41;35m", "[41;36m", "[41;37m",
            "[42;30m", "[42;31m", "[42;32m", "[42;33m",
            "[42;34m", "[42;35m", "[42;36m", "[42;37m",
            "[43;30m", "[43;31m", "[43;32m", "[43;33m",
            "[43;34m", "[43;35m", "[43;36m", "[43;37m",
            "[44;30m", "[44;31m", "[44;32m", "[44;33m",
            "[44;34m", "[44;35m", "[44;36m", "[44;37m",
            "[45;30m", "[45;31m", "[45;32m", "[45;33m",
            "[45;34m", "[45;35m", "[45;36m", "[45;37m",
            "[46;30m", "[46;31m", "[46;32m", "[46;33m",
            "[46;34m", "[46;35m", "[46;36m", "[46;37m",
            "[47;30m", "[47;31m", "[47;32m", "[47;33m",
            "[47;34m", "[47;35m", "[47;36m", "[47;37m",
        };

        bgfg_codes = bgfg_code_strings;
    };

    //
    // Get the ansii escape sequence for the given code
    //
    std::string get_code (unsigned char code)
    {
        assert(code < codes.size());
        return (codes[code]);
    }

    //
    // Get the human readable name for the ansii escape sequence
    //
    std::string get_code_name (unsigned char code)
    {
        assert(code < codes.size());
        return (code_names[code]);
    }

    //
    // For speed and to avoid converting strings, we have a special lookup
    // for background and foreground combinations.
    //
    std::string get_bgfg_code (unsigned char bg, unsigned char fg)
    {
        bg -= BACKGROUND_BLACK;
        fg -= FOREGROUND_BLACK;
        auto code = bg * 8 + fg;
        assert(code < codes.size());
        return (bgfg_codes[code]);
    }

    enum AnsiCode {
        RESET                        = 0,
        BOLD                         = 1,
        FAINT                        = 2,
        ITALIC                       = 3,
        UNDERLINE                    = 4,
        SLOW_BLINK                   = 5,
        RAPID_BLINK                  = 6,
        REVERSE_VIDEO                = 7,
        CONCEAL                      = 8,
        CROSSED_OUT                  = 9,
        PRIMARY_FONT                 = 10,
        ALT_FONT1                    = 11,
        ALT_FONT2                    = 12,
        ALT_FONT3                    = 13,
        ALT_FONT4                    = 14,
        ALT_FONT5                    = 15,
        ALT_FONT6                    = 16,
        ALT_FONT7                    = 17,
        ALT_FONT8                    = 18,
        ALT_FONT9                    = 19,
        FRAKTUR                      = 20,
        BOLD_OFF_OR_DOUBLE_UNDERLINE = 21,
        NORMAL_COLOR                 = 22,
        NOT_ITALIC                   = 23,
        UNDERLINE_OFF                = 24,
        BLINK_OFF                    = 25,
        UNUSED                       = 25,
        INVERSE_OFF                  = 27,
        REVEAL                       = 28,
        NOT_CROSSED_OUT              = 29,
        FOREGROUND_BLACK             = 30,
        FOREGROUND_RED               = 31,
        FOREGROUND_GREEN             = 32,
        FOREGROUND_YELLOW            = 33,
        FOREGROUND_BLUE              = 34,
        FOREGROUND_MAGENTA           = 35,
        FOREGROUND_CYAN              = 36,
        FOREGROUND_WHITE             = 37,
        FOREGROUND_COLOR2            = 38,
        DEFAULT_FOREGROUND_COLOR     = 39,
        BACKGROUND_BLACK             = 40,
        BACKGROUND_RED               = 41,
        BACKGROUND_GREEN             = 42,
        BACKGROUND_YELLOW            = 43,
        BACKGROUND_BLUE              = 44,
        BACKGROUND_MAGENTA           = 45,
        BACKGROUND_CYAN              = 46,
        BACKGROUND_WHITE             = 47,
        BACKGROUND_COLOR2            = 48,
        DEFAULT_BACKGROUND_COLOR     = 49,
        UNUSED2                      = 49,
        FRAMED                       = 51,
        ENCIRCLED                    = 52,
        OVERLINED                    = 53,
        NOT_FRAMED_OR_ENCIRCLED      = 54,
        NOT_OVERLINED                = 55,
        UNUSED3                      = 56,
        UNUSED4                      = 57,
        UNUSED5                      = 58,
        UNUSED6                      = 59,
        UNDERLINE2                   = 60,
        DOUBLE_UNDERLINE             = 61,
        OVERLINE                     = 62,
        DOUBLE_OVERLINE              = 63,
        STRESS_MARKING               = 64,
        ATTRIBUTES_OFF               = 65,
    };
private:
    std::vector<std::string> codes;
    std::vector<std::string> code_names;
    std::vector<std::string> bgfg_codes;
};

int main (int argc, char *argv[])
{
    Ansi ansii;

    //
    // Print a nice message
    //
    std::cout << std::endl;
    std::cout << ansii.get_code(ansii.FOREGROUND_RED);
    std::cout << "hello ";

    std::cout << ansii.get_code(ansii.FOREGROUND_GREEN);
    std::cout << "beautiful";
    std::cout << ansii.get_code(ansii.RESET);

    std::cout << ansii.get_code(ansii.FOREGROUND_CYAN);
    std::cout << " colorful";
    std::cout << ansii.get_code(ansii.RESET);

    std::cout << ansii.get_code(ansii.FOREGROUND_BLUE);
    std::cout << " world";
    std::cout << ansii.get_code(ansii.RESET);

    std::cout << " from C++";
    std::cout << std::endl;

    return (0);
}
```
To build:
<pre>
cd hello_world_color
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# hello # beautiful# #  colorful# #  world#  from C++
</pre>
How to use a range based for loop
=================================

A nice extension in C++ 11 is range based for loops. They are both
readable and safer as you do not need to manually specify the boundary
conditions.

There are a few ways to walk such containers. One is via a constant iterator e.g.:

By now you will have seen the following range based for loop e.g.:
```C++
    for (auto const i : container) { }
```
Another, if you wish to modify the data as you go is:
```C++
    for (auto &i : container) { }
```
However it may be 'simpler' to always do the following and use a forward reference:
```C++
    for (auto &&i : container) { }
```
This handles corner cases for things like vectors of bitfields that you cannot have
a reference to. Here is such a corner case. The iterator provided is actually a thing
called a proxy iterator, vector<bool>::reference. So, this will not compile:
```C++
    #include <vector>
    int main() {
        std::vector<bool> v(8);
        for (auto& e : v) { e = true; }
    }
```
But this will!
```C++
    #include <vector>
    int main() {
        std::vector<bool> v(8);
        for (auto&& e : v) { e = true; }
    }
```
Here is a full example:
```C++
#include <iostream>
#include <algorithm>
#include <vector>

int main() {
    // Create a vector of strings:
    std::initializer_list< std::string > init1 = {"elem1", "elem1"};
    std::vector< std::string > vec1(init1);

    // Range based for loop iterator with a const:
    for (const auto i : vec1) {
        std::cout << "vec1: walk " << i << std::endl;
    }

    // Range based for loop iterator with a modifiable reference:
    for (auto &i : vec1) {
        i += "+ stuff";
        std::cout << "vec1: walk " << i << std::endl;
    }

    // Range based for loop iterator with forward reference:
    for (auto &&i : vec1) {
        i += "+ more stuff";
        std::cout << "vec1: walk " << i << std::endl;
    }
}
```
To build:
<pre>
cd range_based_for_loop
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# Create a vector of strings:

# Range based for loop iterator with a const:
vec1: walk elem1
vec1: walk elem1

# Range based for loop iterator with a modifiable reference:
vec1: walk elem1+ stuff
vec1: walk elem1+ stuff

# Range based for loop iterator with forward reference:
vec1: walk elem1+ stuff+ more stuff
vec1: walk elem1+ stuff+ more stuff
</pre>
How to use a range based for loop with your own class iterator
==============================================================

By now you will have seen the following range based for loop e.g.:
```C++
    for (i : container) { }
```
Now it would be nice to apply that to our own classes too. To do this,
you need to define something that can hold where we are in the processing
loop, called an iterator e.g.:
```C++
    class Iterator {
    public:
        Iterator(T* ptr) : ptr(ptr) {}
        Iterator operator++() { ++ptr; return *this; }
        bool operator!= (const Iterator& o) const {
            return ptr != o.ptr;
        }
        const T& operator*() const { return *ptr; }
   private:
       T* ptr;
   };
public:
   Iterator begin() const { return Iterator(data); }
   Iterator end() const { return Iterator(data + len); }
```
Notice that the iterator has some private data that we increment as
we walk.

Here is a full example:
```C++
#include <iostream>
#include <sstream>
#include <algorithm>

template<class T> class MyVector {
private:
    T *data;
    size_t maxlen;
    size_t currlen;
public:
    MyVector<T> () : data (nullptr), maxlen(0), currlen(0) {
        std::cout << "default constructor " << to_string() << std::endl;
    }
    MyVector<T> (int maxlen) : data (new T [maxlen]),
                               maxlen(maxlen),
                               currlen(0) {
        std::cout << "new " << to_string() << std::endl;
    }
    MyVector<T> (const MyVector& o) {
        std::cout << "copy constructor called for " << o.to_string() << std::endl;
        data = new T [o.maxlen];
        maxlen = o.maxlen;
        currlen = o.currlen;
        std::copy(o.data, o.data + o.maxlen, data);
        std::cout << "copy constructor result is  " << to_string() << std::endl;
    }
    MyVector<T> (MyVector<T>&& o) {
        std::cout << "std::move called for " << o.to_string() << std::endl;
        data = o.data;
        maxlen = o.maxlen;
        currlen = o.currlen;
        o.data = nullptr;
        o.maxlen = 0;
        o.currlen = 0;
        std::cout << "std::move result is  " << to_string() << std::endl;
    }
    ~MyVector<T> () {
        std::cout << "delete " << to_string() << std::endl;
    }
    void push_back (const T& i) {
        if (currlen >= maxlen) {
            maxlen *= 2;
            auto newdata = new T [maxlen];
            std::copy(data, data + currlen, newdata);
            if (data) {
                delete[] data;
            }
            data = newdata;
        }
        data[currlen++] = i;
        std::cout << "push_back called " << to_string() << std::endl;
    }
    friend std::ostream& operator<<(std::ostream &os, const MyVector<T>& o) {
        auto s = o.data;
        auto e = o.data + o.currlen;;
        while (s < e) {
            os << "[" << *s << "]";
            s++;
        }
        return os;
    }
    std::string to_string (void) const {
        auto address = static_cast<const void*>(this);
        std::stringstream ss;
        ss << address;

        std::string elems;
        auto s = data;
        auto e = data + currlen;;
        while (s < e) {
            elems += std::to_string(*s);
            s++;
            if (s < e) {
                elems += ",";
            }
        }

        return "MyVector(" + ss.str() +
                         ", currlen=" + std::to_string(currlen) +
                         ", maxlen=" + std::to_string(maxlen) +
                         " elems=[" + elems + "])";
    }
    class Iterator {
    public:
        Iterator(T* ptr) : ptr(ptr) {}
        Iterator operator++() { ++ptr; return *this; }
        bool operator!= (const Iterator& o) const {
            return ptr != o.ptr;
        }
        const T& operator*() const { return *ptr; }
   private:
       T* ptr;
   };
public:
   Iterator begin() const {
       return Iterator(data);
   }
   Iterator end() const {
       return Iterator(data + currlen);
   }
};

int main() {
    // Create a custom vector class:
    auto vec1 = MyVector<int>(1);
    vec1.push_back(10);
    vec1.push_back(11);
    vec1.push_back(12);

    // Walk the custom vector with our iterator:
    for (auto i : vec1) {
        std::cout << "vec1: walk " << i << std::endl;
    }

    // End, expect vec1 destroy:
}
```
To build:
<pre>
cd range_based_for_loop_custom_begin_end
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# Create a custom vector class:
new MyVector(0x7ffeea8c1730, currlen=0, maxlen=1 elems=[])
push_back called MyVector(0x7ffeea8c1730, currlen=1, maxlen=1 elems=[10])
push_back called MyVector(0x7ffeea8c1730, currlen=2, maxlen=2 elems=[10,11])
push_back called MyVector(0x7ffeea8c1730, currlen=3, maxlen=4 elems=[10,11,12])

# Walk the custom vector with our iterator:
vec1: walk 10
vec1: walk 11
vec1: walk 12

# End, expect vec1 destroy:
delete MyVector(0x7ffeea8c1730, currlen=3, maxlen=4 elems=[10,11,12])
</pre>
How to use std::bind, the basic stuff
=====================================

std::bind is a useful construct that allows you to associate some context
(i.e. a classes "this" pointer) with a function. It is primarily of use
with invoking callback functionaliy, but to get started let's focus on a
some simple cases:
```C++
    auto bind_fn = bind(bind_test, _1, _2, _3);
```
The funny syntax, _1 refers to an argument number is is fully called 
std::placeholders::_1 but for obvious reasons people prefer to use:
```C++
    using namespace std::placeholders; // for _1, _2, _3...
```
to make things readable. Once a function is bound, you can invoke it
like:
```C++
    bind_fn(1, 2, 3);
```
and bind_test with be called with (1, 2, 3). Nice.

It can also be used in std::for_each loops e.g.:
```C++
    static void do_stuff (const std::string& arg1) {
        std::cout << arg1 << std::endl;
    }

    std::list<std::string> l;
    auto bind_fn = bind(do_stuff, _1);
    std::for_each(l.begin(), l.end(), bind_fn);
```
And with std::transform (where you show intent to modify the list):
```C++
    static std::string do_stuff (std::string& arg1) {
        return arg1 + " is modified";
    }

    std::list<std::string> l;
    auto bind_fn = bind(do_stuff, _1);
    std::transform(l.begin(), l.end(), l.begin(), bind_fn);
```
Here is the full example:
```C++
#include <algorithm>
#include <functional> // for _1, _2
#include <iostream>
#include <list>
#include <string>
#include <utility>

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

    // call bind_fn1(_1, _2, _3)
    fn1(a, b, c);

    // call bind_fn1(_2, _1, _3)
    fn2(a, b, c);

    // call bind_fn1(_3, _2, _1)
    fn3(a, b, c);

    //
    // Use std::bind with std::for_each
    //
    std::initializer_list< std::string > il = { a, b, c };
    std::list< std::string > l1(il);

    // call our bind function via std::for_each to modify a list of strings
    auto fn4 = bind(bind_try_to_modify_string, _1);
    std::for_each(l1.begin(), l1.end(), fn4);
    for (auto e : l1) { std::cout << "l1 list-elem is now " << e << std::endl; }

    //
    // Use std::bind with std::transform
    //
    std::initializer_list< std::string > i2 = { a, b, c };
    std::list< std::string > l2(i2);

    // call our bind function via std::transform to modify a list of strings
    auto fn5 = bind(bind_really_modify_string, _1);
    std::transform(l2.begin(), l2.end(), l2.begin(), fn5);
    for (auto e : l2) { std::cout << "l2 list-elem is now " << e << std::endl; }
}
```
To build:
<pre>
cd std_bind
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# call bind_fn1(_1, _2, _3)
argument 1 is cat
argument 2 is dog
argument 3 is ork

# call bind_fn1(_2, _1, _3)
argument 1 is dog
argument 2 is cat
argument 3 is ork

# call bind_fn1(_3, _2, _1)
argument 1 is ork
argument 2 is cat
argument 3 is dog

# call our bind function via std::for_each to modify a list of strings
l1 list-elem is now cat
l1 list-elem is now dog
l1 list-elem is now ork

# call our bind function via std::transform to modify a list of strings
l2 list-elem is now cat and is modified
l2 list-elem is now dog and is modified
l2 list-elem is now ork and is modified
</pre>
How to use std::bind with methods or proxy functions
====================================================

Sometimes we want to be able to bind to a method so we can call it directly,
or more likely pass that function around. There are a few ways to do this.
One is via a proxy function like:
```C++
    void proxy_function (int cash, BankAccount<int> &b)
    {
        b.deposit(cash);
    }

    auto f1 = std::bind(proxy_function, _1, account1);
    f1(100);
```
Or without the proxy:
```C++
    auto f2 = std::bind(&BankAccount<int>::deposit, &account1, _1);
    f2(100);
```
Here is the full example:
```C++
#include <algorithm>  // for std::move
#include <functional> // for _1, _2
#include <iostream>
#include <memory>
#include <sstream>    // for std::stringstream
#include <string>
#include <utility>

using namespace std::placeholders;  // for _1, _2, _3...

template<class T> class BankAccount;

template<class T> class BankAccount {
private:
    const T no_cash {};
    T cash {};
public:
    BankAccount<T> () {
        std::cout << "default constructor " << to_string() << std::endl;
    }
    BankAccount<T> (T cash) : cash (cash) {
        std::cout << "new cash " << to_string() << std::endl;
    }
    BankAccount<T> (const BankAccount& o) {
        std::cout << "copy cash constructor called for " << o.to_string() << std::endl;
        cash = o.cash;
        std::cout << "copy cash constructor result is  " << to_string() << std::endl;
    }
    // Transfer of funds?
    BankAccount<T> (BankAccount<T>&& o) {
        std::cout << "move cash called for " << o.to_string() << std::endl;
        cash = o.cash;
        o.cash = no_cash;
        std::cout << "move cash result is  " << to_string() << std::endl;
    }
    ~BankAccount<T> () {
        std::cout << "delete account " << to_string() << std::endl;
    }
    void deposit (const T& deposit) {
        cash += deposit;
        std::cout << "deposit cash called " << to_string() << std::endl;
    }
    using CheckTransactionCallback = std::function<void(T)>;
    int check_transaction (int cash, CheckTransactionCallback fn) {
        if (cash < 100) {
            throw std::string("transaction is too small for Mr Money Bags");
        } else {
            fn(cash);
        }
        return cash;
    }
    T balance (void) const {
        return cash;
    }
    bool check_balance (T expected) const {
        if (cash == expected) {
            return true;
        } else {
            throw std::string("account has different funds " +
                              to_string() + " than expected " +
                              std::to_string(expected));
        }
    }
    friend std::ostream& operator<<(std::ostream &os, const BankAccount<T>& o) {
        os << "$" << std::to_string(o.cash);
        return os;
    }
    std::string to_string (void) const {
        auto address = static_cast<const void*>(this);
        std::stringstream ss;
        ss << address;
        return "BankAccount(" + ss.str() + ", cash $" + std::to_string(cash) + ")";
    }
};

void proxy_function (int cash, BankAccount<int> &b)
{
    b.deposit(cash);
}

int main(int, char**)
{
    try {
        // create account1 and try to deposit into it
        auto account1 = BankAccount<int>(0);

        // invoke a bind to a method via proxy
        auto f1 = std::bind(proxy_function, _1, account1);
        f1(100);

        //
        // std::ref is not normally needed unless it is something, like
        // say a bitfield that cannot have a direct reference and needs
        // to be wrapped by std::reference
        //
        // invoke a bind to a method via proxy with std::ref
        auto f2 = std::bind(proxy_function, _1, std::ref(account1));
        f2(100);

        // invoke a bind to a method directly
        auto f3 = std::bind(&BankAccount<int>::deposit, &account1, _1);
        f3(100);

        // check the deposits succeeded
        account1.check_balance(300);

        // end
    } catch (const std::string &e) {
        std::cerr << "FAILED: account1 deposit failed!: " << e << std::endl;
    }
}
```
To build:
<pre>
cd std_bind_with_a_method
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# create account1 and try to deposit into it
new cash BankAccount(0x7ffee25cdf68, cash $0)

# invoke a bind to a method via proxy
copy cash constructor called for BankAccount(0x7ffee25cdf68, cash $0)
copy cash constructor result is  BankAccount(0x7ffee25cdf60, cash $0)
deposit cash called BankAccount(0x7ffee25cdf60, cash $100)

# invoke a bind to a method via proxy with std::ref
deposit cash called BankAccount(0x7ffee25cdf68, cash $100)

# invoke a bind to a method directly
deposit cash called BankAccount(0x7ffee25cdf68, cash $200)

# check the deposits succeeded
delete account BankAccount(0x7ffee25cdf60, cash $100)
delete account BankAccount(0x7ffee25cdf68, cash $200)
# FAILED: account1 deposit failed!: account has different funds BankAccount(0x7ffee25cdf68, cash $200) than expected 300
</pre>
How to use std::bind with a class callback
==========================================

Sometimes we want to be able to invoke callbacks from our class. This can be
difficult in C++ as we need to have the context (i.e. the class object) somehow
associated with the function.

Thankfully, std::bind, can do this for us. Here we bind "account1" to one of
its methods.
```C++
    auto method = std::bind(&BankAccount<int>::deposit, &account1, _1);
    method(100);
```
This is identical to:
```C++
    account1.deposit(100);
```
So not too earth shattering. However, suppose we want to call a method in
our class and have it call another method within the same class (or potentially
another class) i.e. a callback.

Here we define the ability for "check_transaction" to accept a callack
function:
```C++
    using CheckTransactionCallback = std::function<void(T)>;
    int check_transaction (int cash, CheckTransactionCallback fn) {
        if (cash < 100) {
            throw std::string("transaction is too small for Mr Money Bags");
        } else {
            fn(cash);
        }
        return cash;
    }
```
And to invoke it:
```C++
    auto callback = std::bind(&BankAccount<int>::deposit, &account1, _1);
    account1.check_transaction(99, callback);
```
Here is the full example:
```C++
#include <algorithm>  // for std::move
#include <functional> // for _1, _2
#include <iostream>
#include <memory>
#include <sstream>    // for std::stringstream
#include <string>
#include <utility>

using namespace std::placeholders;  // for _1, _2, _3...

template<class T> class BankAccount;

template<class T> class BankAccount {
private:
    const T no_cash {};
    T cash {};
public:
    BankAccount<T> () {
        std::cout << "default constructor " << to_string() << std::endl;
    }
    BankAccount<T> (T cash) : cash (cash) {
        std::cout << "new cash " << to_string() << std::endl;
    }
    BankAccount<T> (const BankAccount& o) {
        std::cout << "copy cash constructor called for " << o.to_string() << std::endl;
        cash = o.cash;
        std::cout << "copy cash constructor result is  " << to_string() << std::endl;
    }
    // Transfer of funds?
    BankAccount<T> (BankAccount<T>&& o) {
        std::cout << "move cash called for " << o.to_string() << std::endl;
        cash = o.cash;
        o.cash = no_cash;
        std::cout << "move cash result is  " << to_string() << std::endl;
    }
    ~BankAccount<T> () {
        std::cout << "delete account " << to_string() << std::endl;
    }
    void deposit (const T& deposit) {
        cash += deposit;
        std::cout << "deposit cash called " << to_string() << std::endl;
    }
    using CheckTransactionCallback = std::function<void(T)>;
    int check_transaction (int cash, CheckTransactionCallback fn) {
        if (cash < 100) {
            throw std::string("transaction is too small for Mr Money Bags");
        } else {
            fn(cash);
        }
        return cash;
    }
    T balance (void) const {
        return cash;
    }
    bool check_balance (T expected) const {
        if (cash == expected) {
            return true;
        } else {
            throw std::string("account has different funds " +
                              to_string() + " than expected " +
                              std::to_string(expected));
        }
    }
    friend std::ostream& operator<<(std::ostream &os, const BankAccount<T>& o) {
        os << "$" << std::to_string(o.cash);
        return os;
    }
    std::string to_string (void) const {
        auto address = static_cast<const void*>(this);
        std::stringstream ss;
        ss << address;
        return "BankAccount(" + ss.str() + ", cash $" + std::to_string(cash) + ")";
    }
};

int main(int, char**)
{
    try {
        // create account1 and try to deposit into it
        auto account1 = BankAccount<int>(0);

        auto deposit_method = std::bind(&BankAccount<int>::deposit, &account1, _1);
        account1.check_transaction(100, deposit_method);
        account1.check_transaction(100, deposit_method);
        account1.check_balance(200);
        std::cout << "SUCCESS: account1 1st deposit succeeded!" << std::endl;

        //
        // This will fail as we catch the 'small' transaction
        //
        account1.check_transaction(99, deposit_method);
        std::cout << "SUCCESS: account1 2nd deposit succeeded!" << std::endl;

        // end
    } catch (const std::string &e) {
        std::cerr << "FAILED: account1 deposit failed!: " << e << std::endl;
    }
}
```
To build:
<pre>
cd std_bind_with_a_class_callback
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# create account1 and try to deposit into it
new cash BankAccount(0x7ffee4b9e540, cash $0)
deposit cash called BankAccount(0x7ffee4b9e540, cash $100)
deposit cash called BankAccount(0x7ffee4b9e540, cash $200)
# SUCCESS: account1 1st deposit succeeded!
delete account BankAccount(0x7ffee4b9e540, cash $200)
# FAILED: account1 deposit failed!: transaction is too small for Mr Money Bags
</pre>
How to use std::move to avoid the cost of temporaries
=====================================================

std::move is a nifty way to avoid the use of temporaries when doing copies
of data. For example if you do:

```C++
template<class T> void simpleswap(T& a, T& b) {
    T tmp = b;
    b = a;
    a = b;
}
```

You will see the copy constructor invoked. Not a huge deal for small classes,
but if you are dealing with a vector, that could be very expensive.

A better approach is to use std::move e.g.

```C++
template<class T> void simpleswap(T& a, T& b) {
    T tmp { std::move(a) };
    a = std::move(b);
    b = std::move(tmp);
}
```

Now the question is, how do we implement std::move on our own classes.
To use this you will see a new form of constructor with the "&&" syntax.
The example below will create our own vector class and then implement
the move constructor to transfer elements across. The old vector will
be left empty.

Note that std::vector already does this approach; we're just applying
the same ideas to a custom class.
```C++
#include <iostream>
#include <string>
#include <sstream> // std::stringstream
#include <algorithm> // std::move

template<class T> class MyVector {
private:
    T *data;
    size_t maxlen;
    size_t currlen;
public:
    MyVector<T> () : data (nullptr), maxlen(0), currlen(0) {
        std::cout << "default constructor " << to_string() << std::endl;
    }
    MyVector<T> (int maxlen) : data (new T [maxlen]),
                               maxlen(maxlen),
                               currlen(0) {
        std::cout << "new " << to_string() << std::endl;
    }
    MyVector<T> (const MyVector& o) {
        std::cout << "copy constructor called for " << o.to_string() << std::endl;
        data = new T [o.maxlen];
        maxlen = o.maxlen;
        currlen = o.currlen;
        std::copy(o.data, o.data + o.maxlen, data);
        std::cout << "copy constructor result is  " << to_string() << std::endl;
    }
    MyVector<T> (MyVector<T>&& o) {
        std::cout << "std::move called for " << o.to_string() << std::endl;
        data = o.data;
        maxlen = o.maxlen;
        currlen = o.currlen;
        o.data = nullptr;
        o.maxlen = 0;
        o.currlen = 0;
        std::cout << "std::move result is  " << to_string() << std::endl;
    }
    ~MyVector<T> () {
        std::cout << "delete " << to_string() << std::endl;
    }
    void push_back (const T& i) {
        if (currlen >= maxlen) {
            maxlen *= 2;
            auto newdata = new T [maxlen];
            std::copy(data, data + currlen, newdata);
            if (data) {
                delete[] data;
            }
            data = newdata;
        }
        data[currlen++] = i;
        std::cout << "push_back called " << to_string() << std::endl;
    }
    friend std::ostream& operator<<(std::ostream &os, const MyVector<T>& o) {
        auto s = o.data;
        auto e = o.data + o.currlen;;
        while (s < e) {
            os << "[" << *s << "]";
            s++;
        }
        return os;
    }
    std::string to_string (void) const {
        auto address = static_cast<const void*>(this);
        std::stringstream ss;
        ss << address;

        std::string elems;
        auto s = data;
        auto e = data + currlen;;
        while (s < e) {
            elems += std::to_string(*s);
            s++;
            if (s < e) {
                elems += ",";
            }
        }

        return "MyVector(" + ss.str() +
                         ", currlen=" + std::to_string(currlen) +
                         ", maxlen=" + std::to_string(maxlen) +
                         " elems=[" + elems + "])";
    }
};

int main() {
    // Create a custom vector class:
    auto vec1 = new MyVector<int>(1);
    vec1->push_back(10);
    vec1->push_back(11);
    std::cout << "vec1: " << *vec1 << std::endl;

    // Create a new copy of vec1, vec2 via copy constructor (&):
    auto vec2 = *vec1;
    std::cout << "vec2: " << vec2 << std::endl;

    // Check we can append onto the copied vector:
    vec2.push_back(12);
    vec2.push_back(13);
    std::cout << "vec2: " << vec2 << std::endl;

    // Create a new vector from vec1, vec3 via the move constructor (&&):
    auto vec3 = std::move(*vec1);
    std::cout << "vec3: " << vec3 << std::endl;

    // Check we can append onto the std:move'd vector:
    vec3.push_back(14);
    vec3.push_back(15);
    std::cout << "vec3: " << vec3 << std::endl;

    // Destroy the old vector, vec1. It has no invalid elems:
    delete vec1;

    // End, expect vec2 and vec3 destroy:
}
```
To build:
<pre>
cd std_move
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# Create a custom vector class:
new MyVector(0x7f9470c029b0, currlen=0, maxlen=1 elems=[])
push_back called MyVector(0x7f9470c029b0, currlen=1, maxlen=1 elems=[10])
push_back called MyVector(0x7f9470c029b0, currlen=2, maxlen=2 elems=[10,11])
vec1: [10][11]

# Create a new copy of vec1, vec2 via copy constructor (&):
copy constructor called for MyVector(0x7f9470c029b0, currlen=2, maxlen=2 elems=[10,11])
copy constructor result is  MyVector(0x7ffee0fda698, currlen=2, maxlen=2 elems=[10,11])
vec2: [10][11]

# Check we can append onto the copied vector:
push_back called MyVector(0x7ffee0fda698, currlen=3, maxlen=4 elems=[10,11,12])
push_back called MyVector(0x7ffee0fda698, currlen=4, maxlen=4 elems=[10,11,12,13])
vec2: [10][11][12][13]

# Create a new vector from vec1, vec3 via the move constructor (&&):
std::move called for MyVector(0x7f9470c029b0, currlen=2, maxlen=2 elems=[10,11])
std::move result is  MyVector(0x7ffee0fda678, currlen=2, maxlen=2 elems=[10,11])
vec3: [10][11]

# Check we can append onto the std:move'd vector:
push_back called MyVector(0x7ffee0fda678, currlen=3, maxlen=4 elems=[10,11,14])
push_back called MyVector(0x7ffee0fda678, currlen=4, maxlen=4 elems=[10,11,14,15])
vec3: [10][11][14][15]

# Destroy the old vector, vec1. It has no invalid elems:
delete MyVector(0x7f9470c029b0, currlen=0, maxlen=0 elems=[])

# End, expect vec2 and vec3 destroy:
delete MyVector(0x7ffee0fda678, currlen=4, maxlen=4 elems=[10,11,14,15])
delete MyVector(0x7ffee0fda698, currlen=4, maxlen=4 elems=[10,11,12,13])
</pre>
How to use std::forward
=======================

std::forward's aim is to make it easier to pass along the exact type of function
prototype that is given. For example consider before std::forward you would have
to define all combinations of a template e.g.:
```C++
template <typename A, typename B< void f(A& a, B& b) { }
template <typename A, typename B< void f(const A& a, B& b) { }
template <typename A, typename B< void f(A& a, const B& b) { }
template <typename A, typename B< void f(A& a, B& b) { }
template <typename A, typename B< void f(const const A& a, const B& b) { }
```
This is tedious and does not scale. Now with std::forward we need to define only one template:
```C++
template<typename A, typename Account> void f(A&& a, B&& b) {
    f(std::forward<Account>(a), std::forward<Account>(b));
}
```
I struggled to find a useful non generic example of std::forward, but hit upon an
example of a bank account that we pass along the cash to be deposited as an argument.

So if we have a const version of an account we should expect when we pass it to our
deposit template<> that the const function is called; and this then throws an exception 
(the idea being this was a locked account!)

If we have a non const account then we should be able to modify the account.

As a final aside, std::forward is more or less eqivalent to static_cast<U&&>
althought there are apparently some corner cases where this is not true; but
I'm not aware of them, so please let me know :)

Here is the full example:
```C++
#include <iostream>
#include <string>
#include <sstream> // std::stringstream
#include <algorithm> // std::move
#include <utility>
#include <iostream>
#include <functional>

template<class T> class BankAccount {
private:
    const T no_cash {};
    T cash {};
public:
    BankAccount<T> () {
        std::cout << "default constructor " << to_string() << std::endl;
    }
    BankAccount<T> (T cash) : cash (cash) {
        std::cout << "new cash " << to_string() << std::endl;
    }
    BankAccount<T> (const BankAccount& o) {
        std::cout << "copy cash constructor called for " << o.to_string() << std::endl;
        cash = o.cash;
        std::cout << "copy cash constructor result is  " << to_string() << std::endl;
    }
    // Transfer of funds?
    BankAccount<T> (BankAccount<T>&& o) {
        std::cout << "move cash called for " << o.to_string() << std::endl;
        cash = o.cash;
        o.cash = no_cash;
        std::cout << "move cash result is  " << to_string() << std::endl;
    }
    ~BankAccount<T> () {
        std::cout << "delete account " << to_string() << std::endl;
    }
    void deposit (const T& deposit) {
        cash += deposit;
        std::cout << "deposit cash called " << to_string() << std::endl;
    }
    friend int deposit (int cash, const BankAccount<int> &&account) {
        throw std::string("tried to write to a locked (const) account");
    }
    friend int deposit (int cash, const BankAccount<int> &account) {
        throw std::string("tried to write to a locked (const) account");
    }
    friend int deposit (int cash, BankAccount<int> &account) {
        account.deposit(cash);
        return account.cash;
    }
    friend std::ostream& operator<<(std::ostream &os, const BankAccount<T>& o) {
        os << "$" << std::to_string(o.cash);
        return os;
    }
    std::string to_string (void) const {
        auto address = static_cast<const void*>(this);
        std::stringstream ss;
        ss << address;
        return "BankAccount(" + ss.str() + ", cash $" + std::to_string(cash) + ")";
    }
};

template<typename T, typename Account>
int process_deposit(T cash, Account&& b) {
    return deposit(cash, std::forward<Account>(b));
}

int main(int, char**)
{
    try {
        // create account1 and try to deposit into it
        auto account1 = BankAccount<int>(0);
        process_deposit<int>(100, account1);
        std::cout << account1.to_string() << std::endl;
        std::cout << "SUCCESS: account1 deposit succeeded!" << std::endl;
    } catch (const std::string &e) {
        std::cerr << "FAILED: account1 deposit failed!: " << e << std::endl;
    }

    try {
        // create locked account2 and try to deposit into it; this should fail
        const auto account2 = BankAccount<int>(0);
        process_deposit<int>(100, account2);
        std::cout << account2.to_string() << std::endl;
        std::cout << "SUCCESS: account2 deposit succeeded!" << std::endl;
    } catch (const std::string &e) {
        std::cerr << "FAILED: account2 deposit failed!: " << e << std::endl;
    }

    try {
        // create locked account3 and try to deposit into it; this should fail
        auto account3 = BankAccount<int>(0);
        process_deposit<int>(100, std::move(account3));
        std::cout << account3.to_string() << std::endl;
        std::cout << "SUCCESS: account3 deposit succeeded!" << std::endl;
    } catch (const std::string &e) {
        std::cerr << "FAILED: account3 deposit failed!: " << e << std::endl;
    }
}
```
To build:
<pre>
cd std_forward
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# create account1 and try to deposit into it
new cash BankAccount(0x7ffee5d7b6b0, cash $0)
deposit cash called BankAccount(0x7ffee5d7b6b0, cash $100)
BankAccount(0x7ffee5d7b6b0, cash $100)
# SUCCESS: account1 deposit succeeded!
delete account BankAccount(0x7ffee5d7b6b0, cash $100)

# create locked account2 and try to deposit into it; this should fail
new cash BankAccount(0x7ffee5d7b670, cash $0)
delete account BankAccount(0x7ffee5d7b670, cash $0)
# FAILED: account2 deposit failed!: tried to write to a locked (const) account

# create locked account3 and try to deposit into it; this should fail
new cash BankAccount(0x7ffee5d7b630, cash $0)
delete account BankAccount(0x7ffee5d7b630, cash $0)
# FAILED: account3 deposit failed!: tried to write to a locked (const) account
</pre>
How to use std::unique_ptr
==========================

Unique pointers are just that. They cannot be shared. If you try to share
them you will find that an assign happens instead and the ownership of the
pointer moves, leaving the old unique pointer looking at nullptr.

To create a unique pointer you can either use the following which is a bit
long winded
```C++
    std::unique_ptr<class Foo> uptr(new Foo("foo2"));
```
Or, the following which hides the use of "new" but also invokes the copy
constructor. If you don't want that, use the above.
```C++
    auto uptr = std::make_unique< class Foo >(Foo("foo1"));
```
Note: To change the ownership of a unique pointer, use std::move

Example:
```C++
#include <memory>
#include <iostream>
#include <sstream>

class Foo {
private:
    std::string data; // local data for foo for debugging
public:
    Foo(std::string data) : data(data) {
        std::cout << "new " << to_string() << std::endl;
    }
    Foo(const Foo& other) {
        std::cout << "copy constructor " << to_string() << std::endl;
        data = other.data;
    }
    ~Foo() {
        // Note, other.reset will be called for us
        std::cout << "delete " << to_string() << std::endl;
    }
    std::string to_string (void) {
        auto address = static_cast<const void*>(this);
        std::stringstream ss;
        ss << address;
        return "Foo(" + ss.str() + ", data=" + data + ")";
    }
};

int main (void)
{
    // NOTE: make_unique creates a new ptr and will invoke foo1's copy constructor:
    auto uptr1 = std::make_unique< class Foo >(Foo("foo1"));

    // NOTE: to avoid the copy, do this:
    std::unique_ptr<class Foo> uptr2(new Foo("foo2"));

    // As you cannot copy unique pointers, reassign it with move
    std::unique_ptr<class Foo> uptr3;
    uptr3 = std::move(uptr2);

    // Let's print all the unique ptrs now
    std::cout << "uptr1 = " << (uptr1.get() ? uptr1->to_string() : "nullptr") << std::endl;
    std::cout << "uptr2 = " << (uptr2.get() ? uptr2->to_string() : "nullptr") << std::endl;
    std::cout << "uptr3 = " << (uptr3.get() ? uptr3->to_string() : "nullptr") << std::endl;

    // Expect the unique ptr data to be destroyed now
}
```
To build:
<pre>
cd std_unique_ptr
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# NOTE: make_unique creates a new ptr and will invoke foo1's copy constructor:
new Foo(0x7ffee366e088, data=foo1)
copy constructor Foo(0x7fb25f4029e0, data=)
delete Foo(0x7ffee366e088, data=foo1)

# NOTE: to avoid the copy, do this:
new Foo(0x7fb25f402a00, data=foo2)

# As you cannot copy unique pointers, reassign it with move

# Let's print all the unique ptrs now
uptr1 = Foo(0x7fb25f4029e0, data=foo1)
uptr2 = nullptr
uptr3 = Foo(0x7fb25f402a00, data=foo2)

# Expect the unique ptr data to be destroyed now
delete Foo(0x7fb25f402a00, data=foo2)
delete Foo(0x7fb25f4029e0, data=foo1)
</pre>
How to use std::shared_ptr
==========================

Here we create two instances of a std::shared_ptr and then make them point to
each other. This creates a deadlock which can only be resolved by calling
reset() upon which the pointers can then be destructed.

To create a shared pointer you can either use the following which is a bit
long winded
```C++
    std::shared_ptr<class Foo> sptr(new Foo("foo2"));
```
Or, the following which hides the use of "new" but also invokes the copy
constructor. If you don't want that, use the above.
```C++
    auto sptr = std::make_shared< class Foo >(Foo("foo1"));
```
Example:
```C++
#include <memory>
#include <iostream>
#include <sstream>

typedef std::shared_ptr< class Foo > Foop;

class Foo {
private:
    std::string data; // local data for foo for debugging
    Foop other;       // pointer to other Foo's so we can make a deadlock
public:
    Foo(std::string data) : data(data) {
        std::cout << "new " << to_string() << std::endl;
    }
    Foo(const Foo& o) {
        std::cout << "copy constructor " << to_string() << std::endl;
        data = o.data;
        other = o.other;
    }
    ~Foo() {
        // Note, other.reset will be called for us
        std::cout << "delete " << to_string() << std::endl;
    }
    void addref (Foop other) {
        other = other;
        std::cout << "other use_count now " << other.use_count() << std::endl;
    }
    void delref (void) {
        other.reset();
        std::cout << "other use_count now " << other.use_count() << std::endl;
    }
    std::string to_string (void) {
        auto address = static_cast<const void*>(this);
        std::stringstream ss;
        ss << address;
        return "Foo(" + ss.str() + ", data=" + data + ")";
    }
};

int main (void)
{
    // Create a copy constructed class and share it between two pointers:
    auto sptr1 = std::make_shared< class Foo >(Foo("foo1"));
    std::cout << "sptr1 ref count now " << sptr1.use_count() << std::endl;
    auto sptr2 = sptr1;
    std::cout << "sptr2 ref count now " << sptr2.use_count() << std::endl;

    // Try to create a deadlock:
    sptr1->addref(sptr2);
    std::cout << "sptr1 ref count now " << sptr1.use_count() << std::endl;
    sptr2->addref(sptr1);
    std::cout << "sptr2 ref count now " << sptr2.use_count() << std::endl;

    // Undo the 'deadlock':
    sptr1->delref();
    std::cout << "sptr1 ref count now " << sptr1.use_count() << std::endl;
    sptr2->delref();
    std::cout << "sptr2 ref count now " << sptr2.use_count() << std::endl;

    // Release the shared sptrs, expect foo1 to be destroyed:
    sptr1.reset();
    std::cout << "sptr1 ref count now " << sptr1.use_count() << std::endl;
    sptr2.reset();
    std::cout << "sptr2 ref count now " << sptr2.use_count() << std::endl;

    // You can also create shared pointers WITHOUT copy constructor overhead
    std::shared_ptr<class Foo> sptr0(new Foo("foo0"));
    std::cout << "sptr0 = " << (sptr0.get() ? sptr0->to_string() : "nullptr") << std::endl;
}
```
To build:
<pre>
cd std_shared_ptr
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# Create a copy constructed class and share it between two pointers:
new Foo(0x7ffee2e2a220, data=foo1)
copy constructor Foo(0x7fb0b05028d8, data=)
delete Foo(0x7ffee2e2a220, data=foo1)
sptr1 ref count now 1
sptr2 ref count now 2

# Try to create a deadlock:
other use_count now 3
sptr1 ref count now 2
other use_count now 3
sptr2 ref count now 2

# Undo the 'deadlock':
other use_count now 0
sptr1 ref count now 2
other use_count now 0
sptr2 ref count now 2

# Release the shared sptrs, expect foo1 to be destroyed:
sptr1 ref count now 0
delete Foo(0x7fb0b05028d8, data=foo1)
sptr2 ref count now 0

# You can also create shared pointers WITHOUT copy constructor overhead
new Foo(0x7fb0b0502890, data=foo0)
sptr0 = Foo(0x7fb0b0502890, data=foo0)
delete Foo(0x7fb0b0502890, data=foo0)
</pre>
How to make your own wrapper around std::shared_ptr
===================================================

Here we create a wrapper around std::shared_ptr. Why? Mostly educational,
but you could use this to keep track of memory allocation and frees as part
of a basic leak detector.

```C++
#include <memory>
#include <sstream>
#include <string>
#include <iostream>

template <typename T> class MySharedPtr {
private:
    std::shared_ptr<T> sptr;
    std::string name {"nullptr"};

    void debug (const std::string &what) {
        std::cout << name << ": " << what << " " << to_string() << std::endl;
    }

    std::string to_string (void) {
        auto address = static_cast<const void*>(this);
        std::stringstream ss;
        ss << address;
        if (sptr) {
            return "MySharedPtr(" + ss.str() + "," + sptr->to_string() + ")";
        } else {
            return "MySharedPtr(" + ss.str() + ")";
        }
    }
public:
    // explicit means constructor must match exactly
    template <typename ...ARGS> explicit
      MySharedPtr(const std::string &name, ARGS... a) : name(name) {
        sptr = std::make_shared<T>(a...);
        debug("MySharedPtr::make_shared");
    }

    explicit MySharedPtr(const std::string &name) : name(name) {
        sptr = std::make_shared<T>();
        debug("MySharedPtr::make_shared");
    }

    explicit MySharedPtr(void) {
        debug("MySharedPtr::default constructor");
    }

    ~MySharedPtr() {
        debug("MySharedPtr::delete");
    }

    T* const operator->() {
        debug("MySharedPtr::-> dereference");
        return sptr.operator->();
    }

    T* get() const {
        debug("MySharedPtr::get ptr");
        return sptr.get();
    }

    T& operator*() {
        debug("MySharedPtr::* ptr");
        return *sptr;
    }

    const T& operator*() const {
        debug("MySharedPtr::const * ptr");
        return *sptr;
    }

    operator bool() const {
        debug("MySharedPtr::bool");
        return (bool)sptr;
    }

    size_t use_count(void) const {
        return sptr.use_count();
    }

    void reset() {
        debug("MySharedPtr::reset");
        sptr.reset();
    }
};

typedef MySharedPtr< class Foo > Foop;

class Foo {
private:
    std::string data;
    void debug (const std::string &what) {
        std::cout << what << " " << to_string() << std::endl;
    }
public:
    Foo(std::string data) : data(data) {
        debug("new");
    }
    ~Foo() {
        debug("delete");
    }
    std::string to_string (void) {
        auto address = static_cast<const void*>(this);
        std::stringstream ss;
        ss << address;
        return "Foo(" + ss.str() + ", data=" + data + ")";
    }
};

int main (void)
{
    // create a class and share it between two pointers:
    auto sptr1 = MySharedPtr< class Foo >("[foo1]", Foo("foo1-data"));
    std::cout << "sptr1 ref count now " << sptr1.use_count() << std::endl;
    auto sptr2 = sptr1;
    std::cout << "sptr2 ref count now " << sptr2.use_count() << std::endl;

    // release the shared sptrs, expect foo1 to be destroyed:
    sptr1.reset();
    std::cout << "sptr1 ref count now " << sptr1.use_count() << std::endl;
    sptr2.reset();
    std::cout << "sptr2 ref count now " << sptr2.use_count() << std::endl;
}
```
To build:
<pre>
cd std_shared_ptr_wrapper
rm -f *.o example
c++ -std=c++2a -Werror -g -ggdb3 -Wall -c -o main.o main.cpp
c++ main.o  -o example
./example
</pre>
Expected output:
<pre>

# create a class and share it between two pointers:
new Foo(0x7ffee8f84658, data=foo1-data)
[foo1]: MySharedPtr::make_shared MySharedPtr(0x7ffee8f84688,Foo(0x7fa36e4029c8, data=foo1-data))
delete Foo(0x7ffee8f84658, data=foo1-data)
sptr1 ref count now 1
sptr2 ref count now 2

# release the shared sptrs, expect foo1 to be destroyed:
[foo1]: MySharedPtr::reset MySharedPtr(0x7ffee8f84688,Foo(0x7fa36e4029c8, data=foo1-data))
sptr1 ref count now 0
[foo1]: MySharedPtr::reset MySharedPtr(0x7ffee8f84608,Foo(0x7fa36e4029c8, data=foo1-data))
delete Foo(0x7fa36e4029c8, data=foo1-data)
sptr2 ref count now 0
[foo1]: MySharedPtr::delete MySharedPtr(0x7ffee8f84608)
[foo1]: MySharedPtr::delete MySharedPtr(0x7ffee8f84688)
</pre>
