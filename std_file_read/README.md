How to read a file in C++ in 967 different ways
===============================================

Ok, a bit of an exaggeration but there are a number of different ways to
read the data from files, depending on perforance needs and the form of
the data (binary, text etc...)

Opening a file is easy enough, but you should also consider if this is
a binary file (ios::std::binary):
```C++
    std::ifstream infile (filename, std::ios::in);
    std::ifstream infile (filename, std::ios::in | std::ios::binary);
```
To check the open succeeded:
```C++
    if (outfile.is_open()) {
```
And to read from the file, it is preferred to check with the good()
method as this encompasses eof and read/write errors. So do not do
this:
```C++
    while (!outfile.eof()) {
        // read data
    }
```
But instead do:
```C++
    while (outfile.good()) {
        // read data
    }
```
Now as for the actual data, you can choose to read by chars, strings or lines. 
By line will likely be faster.

Here is arguably the simplest way to read. Note that operator<< for std::ifstream
eats all delimiting spaces.
```C++
    std::ifstream infile (filename, std::ios::in);
    if (infile.is_open()) {
        while (infile.good()) {
            infile >> s;
        }
    }
```
An alternative approach is to populate a vector by providing an iterator.
However this still eats spaces.
```C++
    std::ifstream infile (filename, std::ios::in);
    if (infile.is_open()) {
        std::vector<char> data;
        std::copy(std::istream_iterator<char>(infile),
                  std::istream_iterator<char>(),
                  std::back_inserter(data));
    }
```
Here is a similar but better approach using an istreambuf_iterator instead
of the istream_iterator above. Here spaces are not lost:
```C++
    std::ifstream infile (filename, std::ios::in);
    if (infile.is_open()) {
        std::vector<char> data1(std::istreambuf_iterator<char>(infile), {});
    }
```
And finally, if you want to read line at a time:
```C++
    std::string line;
    std::ifstream infile (filename, std::ios::in);
    if (infile.is_open()) {
        while (infile.good() && std::getline(infile, line)) {
            std::cout << line << std::endl;
        }
    }
```
Here is a full example:
```C++
#include <cassert>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

static const std::string filename = "tmp.txt";
static const std::string msg      = "Time is an illusion.\n  Lunch time, doubly so.\n\n    Douglas Adams";

static void file_write(void)
{
  std::ofstream outfile(filename, std::ios::out);
  if (outfile.is_open()) {
    outfile << msg;
    outfile.close();
  }
}

static void file_read_method1_string_at_a_time(void)
{
  std::ifstream infile(filename, std::ios::in);
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

static void file_read_method1_string_at_a_time_noskipws(void)
{
  std::ifstream infile(filename, std::ios::in);

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

static void file_read_method2_via_copy(void)
{
  std::ifstream infile(filename, std::ios::in);
  if (infile.is_open()) {
    std::vector< char > data;
    std::copy(std::istream_iterator< char >(infile), std::istream_iterator< char >(), std::back_inserter(data));
    for (auto c : data) {
      std::cout << c;
    }
    std::cout << std::endl;
  }
}

static void file_read_method3_via_istreambuf(void)
{
  std::ifstream infile(filename, std::ios::in);
  if (infile.is_open()) {
    //
    // {} in the second argument for the data constructor the compiler
    // determines that the type of this argument should be
    // std::istreambuf_iterator<char> and therefore calls its default
    // constructor std::istreambuf_iterator<char>{}
    //
    // See https://stackoverflow.com/questions/4423361/constructing-a-vector-with-istream-iterators
    //
    std::vector< char > data1(std::istreambuf_iterator< char >(infile), {});
    //
    // Alternatively either of these work too:
    //
    std::vector< char > data2((std::istreambuf_iterator< char >(infile)), std::istreambuf_iterator< char >());
    std::vector< char > data3(std::istreambuf_iterator< char >(infile), std::istreambuf_iterator< char > {});
    for (auto c : data1) {
      std::cout << c;
    }
    std::cout << std::endl;
  }
}

static void file_read_method4_getline(void)
{
  std::string   line;
  std::ifstream infile(filename, std::ios::in);
  if (infile.is_open()) {
    while (infile.good() && std::getline(infile, line)) {
      std::cout << line << std::endl;
    }
  }
}

// See
// https://stackoverflow.com/questions/14092847/is-there-any-reason-why-the-stream-cout-has-the-flag-stdios-baseskipws-set
static void show_ios_base_flags(void)
{
  std::ios_base::fmtflags flags = std::cout.flags();
  std::string             out;

  if (flags & std::ios_base::boolalpha)
    out += ":boolalpha";
  if (flags & std::ios_base::dec)
    out += ":dec";
  if (flags & std::ios_base::fixed)
    out += ":fixed";
  if (flags & std::ios_base::hex)
    out += ":hex";
  if (flags & std::ios_base::internal)
    out += ":internal";
  if (flags & std::ios_base::left)
    out += ":left";
  if (flags & std::ios_base::oct)
    out += ":oct";
  if (flags & std::ios_base::right)
    out += ":right";
  if (flags & std::ios_base::scientific)
    out += ":scientific";
  if (flags & std::ios_base::showbase)
    out += ",showbase";
  if (flags & std::ios_base::showpoint)
    out += ":showpoint";
  if (flags & std::ios_base::showpos)
    out += ":showpos";
  if (flags & std::ios_base::skipws)
    out += ":skipws";
  if (flags & std::ios_base::unitbuf)
    out += ":unitbuf";
  if (flags & std::ios_base::uppercase)
    out += ":uppercase";

  std::cout << "std::cout flags are: " << out << std::endl;
}

int main(void)
{
  // Show default IO formatting flags:
  show_ios_base_flags();

  // Create a file
  file_write();

  // Method 1a (string at a time, newlines are lost, whitespace is skipped)
  file_read_method1_string_at_a_time();

  // Method 1b (string at a time, newlines are lost, no skip of whitespace)
  file_read_method1_string_at_a_time_noskipws();

  // Method 2 (via std::copy with back insert iterator, spaces are lost)
  file_read_method2_via_copy();

  // Method 3 (via istreambuf, char at a time, likely slower)
  file_read_method3_via_istreambuf();

  // Method 4 (via getline, line at a time, likely fast)
  file_read_method4_getline();

  // End
}
```
To build:
<pre>
cd std_file_read
rm -f *.o example
clang -std=c++2a -Werror -g -O3 -fstack-protector-all -ggdb3 -Wall -c -o main.o main.cpp
clang  main.o -lstdc++  -o example
./example
</pre>
Expected output:
<pre>

[31;1;4mShow default IO formatting flags:[0m
std::cout flags are: :dec:skipws

[31;1;4mCreate a file[0m

[31;1;4mMethod 1a (string at a time, newlines are lost, whitespace is skipped)[0m
Time is an illusion. Lunch time, doubly so. Douglas Adams 

[31;1;4mMethod 1b (string at a time, newlines are lost, no skip of whitespace)[0m
Time  

[31;1;4mMethod 2 (via std::copy with back insert iterator, spaces are lost)[0m
Timeisanillusion.Lunchtime,doublyso.DouglasAdams

[31;1;4mMethod 3 (via istreambuf, char at a time, likely slower)[0m
Time is an illusion.
  Lunch time, doubly so.

    Douglas Adams

[31;1;4mMethod 4 (via getline, line at a time, likely fast)[0m
Time is an illusion.
  Lunch time, doubly so.

    Douglas Adams

# End
</pre>
