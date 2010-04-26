#include "ModPhp.hpp"
#include "ModPhpHandler.hpp"
#include "IConfig.hpp"

ModPhp::ModPhp()
  : _name("ModPhp"),
    _version("0.1"),
    _handlers()
{
  zia::api::handler::IHandler * phpHandler = new ModPhpHandler;

  this->_handlers.push_back(phpHandler);
  std::cout << "ModPhp::ModPhp()" << std::endl;
}

const std::string & ModPhp::getName(void) const
{
  return (this->_name);
}

const std::string & ModPhp::getVersion(void) const
{
  return (this->_version);
}

const std::vector<zia::api::handler::IHandler *> &  ModPhp::getHandlers(void) const
{
  return (this->_handlers);
}

bool     ModPhp::configure(zia::api::IConfig *conf)
{
  if (conf == NULL)
    return false;
  this->map = &(conf->getEntries());
  ModPhpHandler* tmp = dynamic_cast<ModPhpHandler *> (this->_handlers[0]);
  if (tmp == NULL)
    return false;
  tmp->configure(*(this->map));
  return true;
}

zia::api::IModule *     zia::api::ziaGetModuleInstance(void)
{
  return new ModPhp;
}
