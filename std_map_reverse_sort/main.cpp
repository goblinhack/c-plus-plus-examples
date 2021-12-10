#include "../common/common.h"
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>

static void backward_sort(void)
{
  DOC("backward sorted map");
  std::map< std::string, int, std::greater< std::string > > m;

  m[ "zaphod" ]   = 1;
  m[ "universe" ] = 2;
  m[ "arthur" ]   = 3;
  m[ "marvin" ]   = 4;
  m[ "mice" ]     = 5;
  m[ "vogon" ]    = 6;

  for (auto i : m) {
    std::cout << i.first << " " << i.second << std::endl;
  }
}

static void forward_sort(void)
{
  DOC("forward sorted map");
  std::map< std::string, int, std::less< std::string > > m;

  m[ "zaphod" ]   = 1;
  m[ "universe" ] = 2;
  m[ "arthur" ]   = 3;
  m[ "marvin" ]   = 4;
  m[ "mice" ]     = 5;
  m[ "vogon" ]    = 6;

  for (auto i : m) {
    std::cout << i.first << " " << i.second << std::endl;
  }
}

static void default_sort(void)
{
  DOC("default sorted map");
  std::map< std::string, int > m;

  m[ "zaphod" ]   = 1;
  m[ "universe" ] = 2;
  m[ "arthur" ]   = 3;
  m[ "marvin" ]   = 4;
  m[ "mice" ]     = 5;
  m[ "vogon" ]    = 6;

  for (auto i : m) {
    std::cout << i.first << " " << i.second << std::endl;
  }
}

int main(int, char **)
{
  backward_sort();
  forward_sort();
  default_sort();
}
