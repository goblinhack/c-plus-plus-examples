#include <assert.h>
#include <iomanip>
#include <iostream>
#include <vector>

class Ansi
{
public:
  Ansi()
  {
    const std::initializer_list< std::string > code_strings = {
        /* reset                        */ "\033[0m",
        /* bold                         */ "\033[1m",
        /* faint                        */ "\033[2m",
        /* italic                       */ "\033[3m",
        /* underline                    */ "\033[4m",
        /* slow_blink                   */ "\033[5m",
        /* rapid_blink                  */ "\033[6m",
        /* reverse_video                */ "\033[7m",
        /* conceal                      */ "\033[8m",
        /* crossed_out                  */ "\033[9m",
        /* primary_font                 */ "\033[10m",
        /* alt_font1                    */ "\033[11m",
        /* alt_font2                    */ "\033[12m",
        /* alt_font3                    */ "\033[13m",
        /* alt_font4                    */ "\033[14m",
        /* alt_font5                    */ "\033[15m",
        /* alt_font6                    */ "\033[16m",
        /* alt_font7                    */ "\033[17m",
        /* alt_font8                    */ "\033[18m",
        /* alt_font9                    */ "\033[19m",
        /* fraktur                      */ "\033[20m",
        /* bold_off_or_double_underline */ "\033[21m",
        /* normal_color                 */ "\033[22m",
        /* not_italic                   */ "\033[23m",
        /* underline_off                */ "\033[24m",
        /* blink_off                    */ "\033[25m",
        /* unused                       */ "\033[26m",
        /* inverse_off                  */ "\033[27m",
        /* reveal                       */ "\033[28m",
        /* not_crossed_out              */ "\033[29m",
        /* foreground_black             */ "\033[30m",
        /* foreground_red               */ "\033[31m",
        /* foreground_green             */ "\033[32m",
        /* foreground_yellow            */ "\033[33m",
        /* foreground_blue              */ "\033[34m",
        /* foreground_magenta           */ "\033[35m",
        /* foreground_cyan              */ "\033[36m",
        /* foreground_white             */ "\033[37m",
        /* foreground_color2            */ "\033[38m",
        /* default_foreground_color     */ "\033[39m",
        /* background_black             */ "\033[40m",
        /* background_red               */ "\033[41m",
        /* background_green             */ "\033[42m",
        /* background_yellow            */ "\033[43m",
        /* background_blue              */ "\033[44m",
        /* background_magenta           */ "\033[45m",
        /* background_cyan              */ "\033[46m",
        /* background_white             */ "\033[47m",
        /* background_color2            */ "\033[48m",
        /* default_background_color     */ "\033[49m",
        /* unused2                      */ "\033[49m",
        /* framed                       */ "\033[51m",
        /* encircled                    */ "\033[52m",
        /* overlined                    */ "\033[53m",
        /* not_framed_or_encircled      */ "\033[54m",
        /* not_overlined                */ "\033[55m",
        /* unused3                      */ "\033[56m",
        /* unused4                      */ "\033[57m",
        /* unused5                      */ "\033[58m",
        /* unused6                      */ "\033[59m",
        /* underline2                   */ "\033[60m",
        /* double_underline             */ "\033[61m",
        /* overline                     */ "\033[62m",
        /* double_overline              */ "\033[63m",
        /* stress_marking               */ "\033[64m",
        /* attributes_off               */ "\033[65m",
    };
    codes = code_strings;
  };

  //
  // Get the ansii escape sequence for the given code
  //
  std::string get_code(unsigned char code)
  {
    assert(code < codes.size());
    return (codes[ code ]);
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
  std::vector< std::string > codes;
};

int main(int argc, char *argv[])
{
  Ansi ansii;

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
