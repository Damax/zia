#include "utils.hpp"
#include <iostream>

std::vector<std::string>*	splitString(std::string& str, const char* sep)
{
  std::vector<std::string>* res =  new std::vector<std::string>;
  std::string tmp;

  size_t begin, end;
  while (str != "")
    {
      begin = str.find_first_not_of(sep);
      end = str.find_first_of(sep);
      if (begin == std::string::npos)
	break;
      if (end == std::string::npos)
	end = str.size();
      tmp = str.substr(begin, end);
      res->push_back(tmp);
      end = str.find_first_not_of(sep, end);
      if (end == std::string::npos)
	break;
      str = str.substr(end);
    }
  return res;
}
