#include <iostream>
#include <dlfcn.h>
#include <map>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include "include/IModule.hpp"

extern char **environ;

int main(int ac, char **av)
{

    std::map<std::string, std::multimap<std::string, std::string> > map;

    std::multimap<std::string, std::string>& toto = map["conf"];
    std::multimap<std::string, std::string>::iterator it = toto.find(".php");
    
    if (!execve("/usr/bin/speedy", av, environ))
      std::cout << "execve failed" << std::endl;
    
    return 0;
}
