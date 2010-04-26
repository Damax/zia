#include "ModLogger.hpp"
#include "ModLoggerHandler.hpp"
#include "IConfig.hpp"
#include "tools/ziafilesystem.hpp"

ModLogger::ModLogger()
  : _name("ModLogger"),
    _version("0.1"),
    _handlers()
{
  zia::api::handler::IHandler * loggerHandler = new ModLoggerHandler;

  this->_handlers.push_back(loggerHandler);
}

const std::string & ModLogger::getName(void) const
{
  return (this->_name);
}

const std::string & ModLogger::getVersion(void) const
{
  return (this->_version);
}

const std::vector<zia::api::handler::IHandler *> &  ModLogger::getHandlers(void) const
{
  return (this->_handlers);
}

bool     ModLogger::configure(zia::api::IConfig *conf)
{
  ZiaFileSystem	lib;
  zia::api::IConfig::ValueMap map = conf->getEntries();

  std::multimap<std::string, std::string> multi =map["conf"];
  std::multimap<std::string, std::string>::iterator it = multi.find("path");


  if (it == multi.end())
    {
      std::cerr << "ModLogger: Path not configured" << std::endl;
      return (false);
    }
  if (lib.create_dir(it->second.data()) == false)
    {
      std::cerr << "ModLogger: can not create directory [" << it->second.data() << "]" << std::endl;
      return (false);
    }
  this->_file.open((it->second+"/access.log").data(), std::ios::app);
  if (this->_file.is_open())
    {
      this->_logpath = it->second;
      ModLoggerHandler* hand = dynamic_cast<ModLoggerHandler *> (this->_handlers[0]);
      if (hand != NULL)
	hand->setFile(&(this->_file));
      return true;
    }
  std::cerr << "Cannot open logfile in directory " << it->second+"/access.log" << std::endl;;
  return false;
}

zia::api::IModule *     zia::api::ziaGetModuleInstance(void)
{
  return new ModLogger;
}
