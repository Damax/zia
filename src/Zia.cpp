#include "Zia.hpp"
#include "Server.hpp"
#include "Conf.hpp"
#include "ModuleManager.hpp"

#include <iostream>
#include <stdlib.h>

Zia::Zia():
  servers()
{
}

Zia::~Zia()
{
}

/**
   Reload the conf from the file(s) and
   shutdown and/or start the Servers according
   to this new conf
 **/
void	Zia::reloadConf(const std::string&)
{
  Conf::Inst().loadConfig();
}

void	Zia::updateServers(void)
{
  this->startUnstartedServs();
  this->stopUnconfigedServs();
}

void	Zia::startUnstartedServs(void)
{
  // search for unstarted Servers that are in the conf
  std::vector<std::map<std::string, std::string> > v_hosts = Conf::Inst().getVhosts();

  std::vector<std::map<std::string, std::string> >::iterator confit;

  for (confit = v_hosts.begin(); confit != v_hosts.end(); ++confit)
    {
      if (this->isServerStarted(*confit) == false)
	{
	  Server* new_serv = new Server(atoi((*confit)["port"].data()));
	  this->servers.push_back(new_serv);
	}
    }
}

bool	Zia::isServerStarted(std::map<std::string, std::string> confit)
{
  std::vector<Server *>::iterator servit;
  for (servit = this->servers.begin(); servit != this->servers.end(); ++servit)
    if (confit["port"] == (*servit)->getPortStr()) // server already started
      return true;
  return false;
}

void	Zia::stopUnconfigedServs(void)
{
  std::vector<Server *>::iterator servit;
  for (servit = this->servers.begin(); servit != this->servers.end(); ++servit)
    {
      if (this->isServerConfiged(*servit) == false)
	{
	  // stop the server;
	  ;
	}
    }
}

bool	Zia::isServerConfiged(Server* serv)
{
  std::vector<std::map<std::string, std::string> > v_hosts = Conf::Inst().getVhosts();
  std::vector<std::map<std::string, std::string> >::iterator confit;

  for (confit = v_hosts.begin(); confit != v_hosts.end(); ++confit)
    {
      if (serv->getPortStr() == (*confit)["port"])
	return true;
    }
  return false;
}

void	Zia::exit(void)
{
  std::vector<Server *>::iterator it;
  for (it = this->servers.begin(); it != this->servers.end(); ++it)
    delete (*it);
}

void	Zia::unloadModule(const std::string& modulename)
{
  ModuleManager::Inst().unloadModule(modulename);
}

void	Zia::loadModule(const std::string& modulename)
{
  std::string path = Conf::Inst().get("modulepath", "");
  path = path+"/";
  std::string modpath = path+modulename;
  modpath = modpath+".so";
  ModuleManager::Inst().LoadModule(modpath);
}

void	Zia::list(const std::string& arg)
{
  if (arg == "module")
    {
      std::map<std::string, zia::api::IModule* >& loadedmodules = ModuleManager::Inst().getModules();
      std::map<std::string, zia::api::IModule* >::iterator it = loadedmodules.begin();
      if (it == loadedmodules.end())
	std::cout << "No module loaded" << std::endl;
      else
	{
	  std::cout << "Modules loaded:" << std::endl;
	  for (;it != loadedmodules.end(); ++it)
	    std::cout << it->first << std::endl;
	}
    }
  else if (arg == "client")
    {
      // TODO afficher tous les clients connectes avec ip+... etc
    }
  else
    std::cout << "Unknow command list: " << arg << std::endl;
}
