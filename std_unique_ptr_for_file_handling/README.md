How to use std::unique_ptr for C API file handling
==================================================

This is a bit of an anti-example, as the best way to handle files in C++
is to use std::ofstream and std::ifstream. However, supposing you do not
want to do that and want to manage files yourself. For this we can use a
unique_ptr to wrap the file handle. First, open the file via the C API:
```C++
    auto cfp = std::fopen(filename.c_str(), "r");
```
Now create a pointer to wrap it:
```C++
    std::unique_ptr<std::FILE, decltype(&close_file)> filep(cfp, &close_file);
```
And provide a deleter:
```C++
    static void close_file (std::FILE* fp) {
        if (fp) {
            std::fclose(fp);
        }
    }
```
Now magically, when "filep" goes out of scope the deleter is called and the
file is closed!

Here is the full example:
```C++
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string.h>

static void close_file(std::FILE *fp)
{
  if (fp) {
    std::cout << "close file " << fp << std::endl;
    std::fclose(fp);
  }
}

int main(void)
{
  const std::string filename = "tmp.txt";
  const std::string msg      = "Time is an illusion. Lunch time, doubly so";

  // Create a file and write to it
  std::ofstream(filename) << msg;

  // Read from the file
  auto cfp = std::fopen(filename.c_str(), "r");
  if (cfp) {
    std::cout << "open file " << cfp << std::endl;
    std::unique_ptr< std::FILE, decltype(&close_file) > filep(cfp, &close_file);
    if (filep) {
      char s[ msg.size() + 1 ];
      std::cout << std::fgets(s, sizeof(s), filep.get()) << '\n';
    }
  }

  // End, expect file clost to automatically happen
}
```
To build:
<pre>
cd std_unique_ptr_for_file_handling
rm -f *.o example
clang -std=c++2a -Werror -g -O3 -fstack-protector-all -ggdb3 -Wall -c -o main.o main.cpp
clang  main.o -lstdc++  -o example
./example
</pre>
Expected output:
<pre>

[31;1;4mCreate a file and write to it[0m

[31;1;4mRead from the file[0m
open file 0x561f0b5cff00
Time is an illusion. Lunch time, doubly so
close file 0x561f0b5cff00

# End, expect file clost to automatically happen
</pre>
