#include "utils.hpp"

#include <string>
#include <sstream>

const std::string	inttostr(int i)
{
  std::stringstream out;
  out << i;
  return out.str();
}
