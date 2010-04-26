#ifndef __MODULEMANAGER__HPP
# define __MODULEMANAGER__HPP

#include <map>
#include <string>
#include <vector>
#include "api/handler/IHandler.hpp"
#include "api/IModule.hpp"

/***
    This class load/unload modules and keep them in a vector.
    Use 'getHandlersForHook' with the EHook you want to get
    a list of ALL the IHandlers of all loaded modules.
 **/
class ModuleManager
{
private:
  ModuleManager();
  ~ModuleManager();
  ModuleManager(const ModuleManager&);
  ModuleManager& operator=(const ModuleManager&);

public:
  void LoadAll(const std::string&);
  void LoadModule(const std::string&);
  void UnloadAll(void);
  bool isModuleLoaded(const std::string&);
  bool configureModule(const std::string&);
  void unloadModule(const std::string&);
  static ModuleManager& Inst();
  std::map<std::string, zia::api::IModule* >&	getModules(void)
  {
    return this->modules;
  }
private:
  std::map<std::string, zia::api::IModule* > modules;
  // std::map<zia::api::handler::EHook, std::vector<zia::api::handler::IHandler *> > listhandler;
  static ModuleManager*		pInstance;
};

#endif /* __MODULEMANAGER__HPP */
