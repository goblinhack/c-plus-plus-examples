#include "../common/common.h"
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

  DOC("Create a file and write to it");
  std::ofstream(filename) << msg;

  DOC("Read from the file");
  auto cfp = std::fopen(filename.c_str(), "r");
  if (cfp) {
    std::cout << "open file " << cfp << std::endl;
    std::unique_ptr< std::FILE, decltype(&close_file) > filep(cfp, &close_file);
    if (filep) {
      char s[ msg.size() + 1 ];
      std::cout << std::fgets(s, sizeof(s), filep.get()) << '\n';
    }
  }

  DOC("End, expect file clost to automatically happen");
}
