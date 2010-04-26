#include "ModOpenSSL.hpp"
#include "ModOpenSSLHandler.hpp"
#include "IConfig.hpp"

ModOpenSSL::ModOpenSSL()
  : _name("ModOpenSSL"),
    _version("0.1"),
    _handlers()
{
  zia::api::handler::IHandler * openSSLHandler = new ModOpenSSLHandler;

  this->_handlers.push_back(openSSLHandler);
}

const std::string & ModOpenSSL::getName(void) const
{
  return (this->_name);
}

const std::string & ModOpenSSL::getVersion(void) const
{
  return (this->_version);
}

const std::vector<zia::api::handler::IHandler *> &  ModOpenSSL::getHandlers(void) const
{
  return (this->_handlers);
}

bool     ModOpenSSL::configure(zia::api::IConfig *conf)
{
  zia::api::IConfig::ValueMap map = conf->getEntries();

  std::multimap<std::string, std::string> multi = map["conf"];
  std::multimap<std::string, std::string>::iterator it = multi.find("hosts");

  if (it == multi.end())
    {
      std::cerr << "ModOpenSSL: No hosts configured" << std::endl;
      return false;
    }

  //  ModOpenSSLHandler* handler = static_cast<ModOpenSSLHandler*>(this->_handlers[0]);
//   if (handler->initSSL(multi) == false)
//     return false;
//   handler->LoadCert("");
  return true;
}

zia::api::IModule *     zia::api::ziaGetModuleInstance(void)
{
  return new ModOpenSSL;
}
