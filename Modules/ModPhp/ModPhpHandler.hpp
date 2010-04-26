#ifndef __ZIA_MOD_LOGGER_HANDLER_HPP__
# define __ZIA_MOD_LOGGER_HANDLER_HPP__

# include <ostream>
# include <istream>
# include <fstream>

# include "handler/IBodyHandler.hpp"
# include "IConfig.hpp"

class   ModPhpHandler : public zia::api::handler::IBodyHandler
{
public:
  ModPhpHandler();
  virtual ~ModPhpHandler(void);
  virtual zia::api::handler::ECode    operator()(zia::api::http::ITransaction & transac, std::istream & is, std::ostream & os);
  virtual zia::api::handler::EHook   getHook(void) const;
  void	setFile(std::ofstream *);
  void	inParent(int *p, std::ostream& os, std::istream& is);
  void	inChild(int *p, const char *bin, const std::string& uri, zia::api::http::ITransaction& transac, std::istream& is);
  void	configure(const zia::api::IConfig::ValueMap& map);
private:
  const zia::api::IConfig::ValueMap*	_map;
  std::ofstream*		_file;
};

#endif	/* ! __ZIA_MOD_LOGGER_HANDLER_HPP__ */
