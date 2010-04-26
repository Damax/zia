#include <iostream>
#include <dlfcn.h>
#include "include/IModule.hpp"

int main(int ac, char **av)
{
  using std::cout;
  using std::cerr;

  void* mod = dlopen("./ModLogger.so", RTLD_LAZY);
  if (!mod)
    {
      cerr << "Cannot load library: " << dlerror() << '\n';
      return 1;
    }
  dlerror();

  typedef zia::api::IModule* (*instan)();
  instan create_mod = (instan) dlsym(mod, "ziaGetModuleInstance");
  const char* dlsym_error = dlerror();
  if (dlsym_error)
    {
      cerr << "Cannot load symbol create: " << dlsym_error << '\n';
      return 1;
    }

  zia::api::IModule* modlog = create_mod();
  //  std::cout << modlog->getName() << std::endl;
  const std::vector<zia::api::handler::IHandler *> &  handlers = modlog->getHandlers();
//   std::cout << handlers[0]->getHook() << std::endl;
  if (handlers[0]->getHook() == zia::api::handler::ResponseBodyModifier)
    {
      std::cout << " COOL BRO !" << std::endl;
    }
  return (0);
}


