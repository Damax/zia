#ifndef __UTILS_HPP__
# define __UTILS_HPP__

#include <string>
#include <vector>

const std::string			struppercase(const std::string&);
const std::string			strlowercase(const std::string&);
int					isdir (const char *);
std::vector<std::string>*		splitString(std::string&, const char*);
const std::string			inttostr(int i);
std::string				urlencode(const std::string &);
std::string				char2hex(char dec);
int				        myhtoi(const char *s);
std::string				urldecode(const std::string&);

// TODO: NOT USE, SHOULD BE REMOVE
// void					directoryList2(const std::string& directory_name, std::vector<std::string>& res);

#endif /* __UTILS_HPP__ */

