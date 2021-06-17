#include <cassert>
#include <fstream>
#include <ios>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>
#include "../common/common.h"

static const std::string filename = "tmp.txt";
static const std::string msg = "Time is an illusion.\n  Lunch time, doubly so.\n\n    Douglas Adams";

static void file_write (void) 
{
    std::ofstream outfile (filename, std::ios::out);
    if (outfile.is_open()) {
        outfile << msg;
        outfile.close();
    }
}

static void file_read_method1_string_at_a_time (void)
{
    std::ifstream infile (filename, std::ios::in);
    if (infile.is_open()) {
        //
        // good() is preferred over eof() as it combines all
        // of the following:
        //
        // goodbit: good()=1 eof()=0 fail()=0 bad()=0
        //  eofbit: good()=0 eof()=1 fail()=0 bad()=0
        // failbit: good()=0 eof()=0 fail()=1 bad()=0
        //  badbit: good()=0 eof()=0 fail()=1 bad()=1
        //
        while (infile.good()) {
            std::string s;
            infile >> s;
            std::cout << s << " ";
        }
        std::cout << std::endl;
    }
}

static void file_read_method1_string_at_a_time_noskipws (void)
{
    std::ifstream infile (filename, std::ios::in);

    if (infile.is_open()) {
        infile >> std::resetiosflags(std::ios_base::skipws);
        while (infile.good()) {
            std::string s;
            infile >> s;
            std::cout << s << " ";
        }
        std::cout << std::endl;
    }
}

static void file_read_method2_via_copy (void)
{
    std::ifstream infile (filename, std::ios::in);
    if (infile.is_open()) {
        std::vector<char> data;
        std::copy(std::istream_iterator<char>(infile),
                  std::istream_iterator<char>(),
                  std::back_inserter(data));
        for (auto c : data) {
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

static void file_read_method3_via_istreambuf (void)
{
    std::ifstream infile (filename, std::ios::in);
    if (infile.is_open()) {
        //
        // {} in the second argument for the data constructor the compiler
        // determines that the type of this argument should be
        // std::istreambuf_iterator<char> and therefore calls its default
        // constructor std::istreambuf_iterator<char>{}
        //
        // See https://stackoverflow.com/questions/4423361/constructing-a-vector-with-istream-iterators
        //
        std::vector<char> data1(std::istreambuf_iterator<char>(infile), {});
        //
        // Alternatively either of these work too:
        //
        std::vector<char> data2((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>() );
        std::vector<char> data3(std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>{} );
        for (auto c : data1) {
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

static void file_read_method4_getline (void)
{
    std::string line;
    std::ifstream infile (filename, std::ios::in);
    if (infile.is_open()) {
        while (infile.good() && std::getline(infile, line)) {
            std::cout << line << std::endl;
        }
    }
}

// See https://stackoverflow.com/questions/14092847/is-there-any-reason-why-the-stream-cout-has-the-flag-stdios-baseskipws-set
static void show_ios_base_flags (void)
{
    std::ios_base::fmtflags flags = std::cout.flags();
    std::string out;

    if (flags & std::ios_base::boolalpha)  out += ":boolalpha";
    if (flags & std::ios_base::dec)        out += ":dec";
    if (flags & std::ios_base::fixed)      out += ":fixed";
    if (flags & std::ios_base::hex)        out += ":hex";
    if (flags & std::ios_base::internal)   out += ":internal";
    if (flags & std::ios_base::left)       out += ":left";
    if (flags & std::ios_base::oct)        out += ":oct";
    if (flags & std::ios_base::right)      out += ":right";
    if (flags & std::ios_base::scientific) out += ":scientific";
    if (flags & std::ios_base::showbase)   out += ",showbase";
    if (flags & std::ios_base::showpoint)  out += ":showpoint";
    if (flags & std::ios_base::showpos)    out += ":showpos";
    if (flags & std::ios_base::skipws)     out += ":skipws";
    if (flags & std::ios_base::unitbuf)    out += ":unitbuf";
    if (flags & std::ios_base::uppercase)  out += ":uppercase";

    std::cout << "std::cout flags are: " << out << std::endl;
}

int main(void)
{
    DOC("Show default IO formatting flags:");
    show_ios_base_flags();

    DOC("Create a file");
    file_write();

    DOC("Method 1a (string at a time, newlines are lost, whitespace is skipped)");
    file_read_method1_string_at_a_time();

    DOC("Method 1b (string at a time, newlines are lost, no skip of whitespace)");
    file_read_method1_string_at_a_time_noskipws();

    DOC("Method 2 (via std::copy with back insert iterator, spaces are lost)");
    file_read_method2_via_copy();

    DOC("Method 3 (via istreambuf, char at a time, likely slower)");
    file_read_method3_via_istreambuf();

    DOC("Method 4 (via getline, line at a time, likely fast)");
    file_read_method4_getline();

    DOC("End");
}
