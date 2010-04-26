#include <string>
#include <iostream>
#include "utils.hpp"

int myhtoi(const char *s)
{
  int value;
  int c;

  c = ((unsigned char *)s)[0];
  if (isupper(c))
    c = tolower(c);
  value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

  c = ((unsigned char *)s)[1];
  if (isupper(c))
    c = tolower(c);
  value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

  return value;
}

std::string urldecode(const std::string& str)
{
  std::string dest;
  int	len = str.size();
  const char *data = str.data();

  while (len--)
    {
      if (*data == '+')
	{
	  dest += ' ';
	}
      else if (*data == '%' && len >= 2 && isxdigit((int) *(data + 1))
	       && isxdigit((int) *(data + 2)))
	{
	  dest += (char) myhtoi(data + 1);
	  data += 2;
	  len -= 2;
	}
      else
	{
	  dest += *data;
	}
      data++;
    }
  return dest;
}

// int	main(int ac, char **av)
// {
//   std::string ret = urlencode("http://c'est un test… ÇÀ™ÀÇ™");
//   std::string ter = url_decode(ret);
//   std::cout << ter << std::endl;
//   return 0;
// }
