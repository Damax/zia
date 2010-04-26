#ifndef _ZIA_OPENSSL_MODULE_HPP_
# define _ZIA_OPENSSL_MODULE_HPP_

# include <openssl/ssl.h>
# include <openssl/err.h>
# include <iostream>
# include <string>
# include "http/ITransaction.hpp"
# include "IConfig.hpp"
# include "handler/IHandler.hpp"
# include "IModule.hpp"


class ModOpenSSL : public zia::api::IModule
{
public:
  ModOpenSSL(void);
  virtual ~ModOpenSSL(void) { }
  const std::string &					getName(void) const;
  const std::string &					getVersion(void) const;
  const std::vector<zia::api::handler::IHandler *> &	getHandlers(void) const;
  bool							configure(zia::api::IConfig *);
  void							initSSL(void);

private:
  std::string	_name;
  std::string	_version;

  SSL*		_ssl;
  SSL_CTX*	_ctx;
  SSL_METHOD*	_method;
  std::vector<zia::api::handler::IHandler *>  _handlers;
};

#endif	/* ! _ZIA_OPENSSL_MODULE_HPP_ */
