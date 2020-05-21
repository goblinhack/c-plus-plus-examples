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
    #include <iostream>
    use namespace std;
    std::cout << "\033[31;1;4mHello world\033[0m" << std::endl;
```
This gives us red text (31), bold (1) and underlined (4). To reset back to
the terminal default, we send the reset sequence.

We also introduce the "std" namespace here. Some people opt to do
```C++
    #include <iostream>
    use namespace std;
```
but it is considered bad practice to use this. See
[this link](https://stackoverflow.com/questions/1452721/why-is-using-namespace-std-considered-bad-practice).
Personally I agree and find code clearer when it uses "std:" explicitly in the code as
you are left in no doubt as to what namespace is being used.
```C++
    #include <iostream>
    use namespace std;
    std::cout << ...
```
sends output to the stdout for this process. For errors, you would want to use instead:
```C++
    #include <iostream>
    use namespace std;
    std::cerr << "aargh" << ...
```
For newlines, like in C where we had "\n", you have in C++:
```C++
    #include <iostream>
    use namespace std;
    ... << "aargh" << std::endl;
```
A couple of other things are introduced here, namely strings:
```C++
    #include <iostream>
    #include <string>
    use namespace std;
    const std::string what = "hello there"; // consts cannot be modified
```
And vectors e.g.:
```C++
    #include <iostream>
    #include <string>
    #include <vector>
    use namespace std;
    std::vector<std::string> = { "hello", "there" };
```
Ok, so enough background. The output should look like:

![Alt text](https://github.com/goblinhack/c-plus-plus-examples/blob/master/hello_world_color/screenshot.png?raw=true "hello colorful world")

And here is the full example.
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
