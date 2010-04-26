#ifndef __ZIA_MOD_LOGGER_HANDLER_HPP__
# define __ZIA_MOD_LOGGER_HANDLER_HPP__

# include <ostream>
# include <istream>
# include <fstream>

# include "handler/IHeaderHandler.hpp"

class   ModLoggerHandler : public zia::api::handler::IHeaderHandler
{
public:
  ModLoggerHandler() { }
  virtual ~ModLoggerHandler(void) { }
  virtual zia::api::handler::ECode   operator()(zia::api::http::ITransaction &);
  virtual zia::api::handler::EHook   getHook(void) const;
  void	setFile(std::ofstream *);
private:
  std::ofstream* _file;
};

#endif	/* ! __ZIA_MOD_LOGGER_HANDLER_HPP__ */
