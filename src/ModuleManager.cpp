
#include <map>
#include <iostream>
#include <sys/types.h>
#include <vector>

#include "ModuleManager.hpp"
#include "tools/zialibrary.hpp"
#include "common/utils.hpp"
#include "Conf.hpp"
#include "tools/ziafilesystem.hpp"

ModuleManager::ModuleManager()
{
	std::cout << "ModuleManager 1" << std::endl << std::flush;
}

ModuleManager::~ModuleManager()
{
}

ModuleManager& ModuleManager::operator=(const ModuleManager&)
{
  return *this;
}

/***
    Load all modules located in a modulepath.
    conf is :   <zia><Modules/></zia>.
    If the module is already loaded and is in the conf, just re-configure it.
    If the module is already loaded but NOT in the conf, unload it.
    If the module is not loaded and NOT in the conf, do nothing.
    If the module is not loaded and IS in the conf, load and configure it.
 **/
void	ModuleManager::LoadAll(const std::string& path)
{
  std::string slash;
  if (path.length() == 0)
	return ;
  slash = (path[path.length() - 1] == '/') ? "" : "/";
  std::vector<std::string>	list;
  ZiaFileSystem io;
  io.directoryList(path, list);
  std::vector<std::string>::iterator	it = list.begin();
  for (; it != list.end(); ++it)
	  this->LoadModule(path + slash + (*it));
}

/***
    Load one single module and add it to the modules vector
 **/
void	ModuleManager::LoadModule(const std::string& modpath)
{
  // be carefull, do NOT do lib.close if the module is loaded. Doing so
  // would make the module a NULL pointer and segfault when trying to use it
  ZiaLibrary	lib;

  void* handle = lib.open(modpath.data());
  if (handle == NULL)
    return ;
  typedef zia::api::IModule* (*instan)();
  instan create_mod = (instan) lib.load(handle, "ziaGetModuleInstance");
  if (create_mod == NULL)
    {
      lib.close(handle);
      return ;
    }
  zia::api::IModule* module = create_mod();
  if (module == NULL)
    {
      std::cerr << "Module " << modpath << " is not valid." << std::endl;
      lib.close(handle);
      return ;
    }
  if (Conf::Inst().getModConf(module->getName()) == NULL)
    {
      std::cout << "Module is not configured: " << module->getName() << ", not loading it." << std::endl;
      if (isModuleLoaded(module->getName()) == true)
	{
	  this->unloadModule(module->getName());
	}
      lib.close(handle);
      return ;
    }
  if (this->isModuleLoaded(module->getName()) == true)
    {
      std::cout << "Module already loaded, (re)configuring it: " << module->getName() << std::endl;
      this->configureModule(module->getName());
    }
  else
    {
      std::cout << "Loading module: " << module->getName() << std::endl;
      this->modules[module->getName()] = module;
      if (this->configureModule(module->getName()) == false)
	{
	  std::cout << "Bad Conf for module " << module->getName() << ", unloading." << std::endl;
	  this->unloadModule(module->getName());
	  lib.close(handle);
	  return ;
	}
    }
}

void ModuleManager::unloadModule(const std::string& modulename)
{
  std::cout << "unloading module : " << modulename << std::endl;
  std::map<std::string, zia::api::IModule* >::iterator it = this->modules.find(modulename);
  if (it == this->modules.end())
    {
      std::cerr << "Can't unload module: " << modulename << std::endl;
      return ;
    }
  this->modules.erase(it);
  delete (*it).second;
  return ;
}

bool	ModuleManager::isModuleLoaded(const std::string& name)
{
  std::map<std::string, zia::api::IModule* >::iterator it = this->modules.find(name);
  if (it == this->modules.end())
    return false;
  return true;
}

bool	ModuleManager::configureModule(const std::string& name)
{
  if (this->isModuleLoaded(name) == true)
      return this->modules[name]->configure(Conf::Inst().getModConf(name));
  std::cerr << "Cannot configure unloaded module: " << name << std::endl;
  return false;
}

ModuleManager&	ModuleManager::Inst()
{
  if (pInstance == 0)
    pInstance = new ModuleManager();
  return *(pInstance);
}

ModuleManager* ModuleManager::pInstance = 0;
