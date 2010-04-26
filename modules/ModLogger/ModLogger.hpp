#ifndef _ZIA_LOGGER_MODULE_HPP_
# define _ZIA_LOGGER_MODULE_HPP_

# include <fstream>
# include "http/ITransaction.hpp"
# include "IConfig.hpp"
# include "handler/IHandler.hpp"
# include "IModule.hpp"
# include "tools/ziafilesystem.hpp"

class ModLogger : public zia::api::IModule
{
public:
    ModLogger(void);
    virtual ~ModLogger(void) { }

  const std::string & getName(void) const;
  const std::string & getVersion(void) const;
  const std::vector<zia::api::handler::IHandler *> &   getHandlers(void) const;
  bool        configure(zia::api::IConfig *);

private:
  std::string	_logpath;
  std::string	_name;
  std::string	_version;
  std::ofstream _file;
  std::vector<zia::api::handler::IHandler *>  _handlers;
};

#endif	/* ! _ZIA_LOGGER_MODULE_HPP_ */
