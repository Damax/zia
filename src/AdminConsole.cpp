#include <iostream>
#include <vector>
#include "AdminConsole.hpp"
#include "common/utils.hpp"


AdminConsole::AdminConsole(Zia* zia)
{
  this->_zia = zia;
}

void	AdminConsole::readLine()
{
  std::vector<std::string> *res;

  std::string line;
  while (1)
    {
      std::cout << "> ";
      if ((std::getline(std::cin, line)).eof())
	{
	  std::cout << std::endl;
	  return ;
	}
      res = splitString(line, " ");
      int	i = 0;
      std::vector<std::string>::iterator it;
      std::string cmd;
      std::string arg = "";
      for (it = res->begin(); it != res->end(); ++it)
	{
	  if (i == 0)
	    cmd = *it;
	  else if (i == 1)
	    arg = *it;
	  else
	    break;
	  i++;
	}
      if (cmd == "exit")
	{
	  std::cout << std::endl;
	  return ;
	}
      this->execCommand(cmd, arg);
      delete res;
    }
}

typedef void (Zia::*ziaMethPtr)(const std::string&);

void	AdminConsole::execCommand(const std::string& cmd, const std::string& arg)
{
  if (cmd == "")
    return ;
  std::map<std::string, ziaMethPtr> methods;
  methods["reloadconf"] = &Zia::reloadConf;
  methods["unloadmodule"] = &Zia::unloadModule;
  methods["loadmodule"] = &Zia::loadModule;
  methods["list"] = &Zia::list;
  std::map<std::string, ziaMethPtr>::iterator it;
  if ((it = methods.find(cmd)) != methods.end())
    {
      (this->_zia->*it->second)(arg);
    }
  else
    std::cout << "Unknow command : " << cmd << std::endl;
}
