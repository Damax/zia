#ifndef __ZIA_CONF_PARSER__
#define __ZIA_CONF_PARSER__

#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <vector>

#include <QDomDocument>

#include "api/IConfig.hpp"

/***
    a class keeping the configuration for one module
 **/
class ModConf: public zia::api::IConfig
{
public:
  ModConf(const ModConf& ref)
  {
    this->map = ref.map;
  };

  ModConf&	operator=(const ModConf& ref)
  {
    this->map = ref.map;
    return *this;
  };
  ModConf(){};
  ~ModConf(){};
  virtual const zia::api::IConfig::ValueMap& getEntries(void) const
  { return this->map; }
  void addEntry(const std::string& scope, const std::string& key, const std::string& value)
  {
    std::multimap<std::string, std::string> multi = this->map[scope];
    multi.insert(std::pair<std::string, std::string>(key, value));
    this->map[scope] = multi;
  }

private:
  zia::api::IConfig::ValueMap map;
};

/***
 * Singleton class that parses the configuration
 *  file(s) and keep this configuration.
 *  VirtualHosts are kept in a vector.
 *  Modules are kept in a map, amap associated with their name
 *
 *  example:
 *  A Client receives a request for the host "one.localhost" on port 8000.
 *  The clients wants to know the DocumentRoot.
 *  Conf::get("documentroot", "/var/www", "one.localhost", "8000");
 *  To select the right Vhost, the config searches through all the vhosts with
 *  port=8000, with this priority:
 *  - An host with the correct port AND correct host
 *  - An host with the correct port and NO host
 *  - An host with the correct port and ANY host (the first found)
 *  - The value in the mainconfig (<zia/> node)
 *  - The default value specified
 *  This is implemented in the "get" method
 **/
class Conf
{
public:
  static Conf&		Inst();
  void			loadConfig(const std::string& filename="ziaconf.xml");
  const std::string	get(const std::string& option, const std::string& default_value);
  const std::string	get(const std::string& option, const std::string& default_value, const std::string& port, const std::string& host);
  ModConf*		getModConf(const std::string& name);
  const std::vector<std::map<std::string, std::string> > getVhosts()
  {
    return this->v_hosts;
  }
  void			clearConf(void);

private:
  static Conf* pInstance;
  Conf();
  Conf(const Conf&);
  ~Conf();
  Conf&  operator=(const Conf&);

  void	parseVirtualHost(QDomElement&);
  void	parseMainConf(QDomElement&);
  void	parseModule(QDomElement&);
  void	deleteOldConf(void);
  // Main configuration. (in the <zia> node)
  std::map<std::string, std::string>	default_conf;

  // Vhosts conf.
  std::vector<std::map<std::string, std::string> > v_hosts;

  // Modules conf. Associated with their name
  std::map<std::string, ModConf> mods;
};

#endif
