#ifndef __ZIA__
# define __ZIA__

#include "Server.hpp"

#include <vector>

/***
    This class is the main class, it is instantiated
    at startup, reads the configuration (with ZiaConfParser)
    and creates the Servers specified in the configuration,
    keeping them in a vector.
    It catches signals (SIGHUP for config reload, SIGKILL for
    zia shutdown, etc), reloads the configuration and closes/open
    Servers from this new configuration.
 **/
class Zia
{
  Zia(const Zia&){};
  Zia&	operator=(const Zia&){return *this;};
public:
  Zia();
  ~Zia();
  void	reloadConf(const std::string& str = "");
  void	updateServers(void);
  void	exit(void);
  void	unloadModule(const std::string&);
  void	loadModule(const std::string&);
  void	list(const std::string&);
private:
  std::vector<Server *> servers;
  void	startUnstartedServs(void);
  void	stopUnconfigedServs(void);
  bool	isServerStarted(std::map<std::string, std::string>);
  bool	isServerConfiged(Server*);
};

#endif
