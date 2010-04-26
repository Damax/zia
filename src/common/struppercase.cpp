#include <string>
#include <iostream>
#include "utils.hpp"

const std::string	struppercase(const std::string& str)
{
  unsigned int	i;
  std::string newstr;

  for (i = 0; i < str.length(); i++)
    newstr += toupper(str[i]);
  return (newstr);
}
