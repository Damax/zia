#include <iostream>

#include "Conf.hpp"
#include "Zia.hpp"
#include "ModuleManager.hpp"
#include "AdminConsole.hpp"

void	dummy(int)
{
}

int main(void)
{
  Zia zia;

  zia.reloadConf();
  std::string path = Conf::Inst().get("modulepath", "Modules");
  ModuleManager::Inst().LoadAll(path);
  zia.updateServers();

  AdminConsole console(&zia);
  console.readLine();

}
