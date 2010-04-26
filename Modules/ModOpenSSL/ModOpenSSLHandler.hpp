#ifndef __ZIA_MOD_OPENSSL_HANDLER_HPP__
# define __ZIA_MOD_OPENSSL_HANDLER_HPP__

# include <ostream>
# include <istream>
# include <fstream>
# include <openssl/ssl.h>
# include <openssl/err.h>
# include <string>
# include <vector>

# include "handler/INetworkHandler.hpp"
# include "http/ITransaction.hpp"
# include "network/IEndPoint.hpp"
# include "ModSocketSSL.hpp"

class   ModOpenSSLHandler : public zia::api::handler::INetworkHandler
{
public:
  ModOpenSSLHandler();
  virtual ~ModOpenSSLHandler(void) { }
  virtual zia::api::handler::INetworkHandler::sPtrFunc	getNetworkHandlers(void);
  virtual zia::api::handler::EHook			getHook(void) const;
  virtual zia::api::handler::ECode			operator()(zia::api::http::ITransaction&);
  struct sPtrFunc s;
};


bool							initSSL(ModSocketSSL*);
void							loadCert(ModSocketSSL*, const std::string&, const std::string&);
zia::api::network::IEndPoint*	my_sock_accept(zia::api::network::IEndPoint&);
ssize_t				my_sock_read(zia::api::network::IEndPoint& client, void* buffer, size_t len);
// zia::api::network::IEndPoint*	my_sock_accept(zia::api::network::IEndPoint&);
// zia::api::network::IEndPoint*	my_sock_accept(zia::api::network::IEndPoint&);
// zia::api::network::IEndPoint*	my_sock_accept(zia::api::network::IEndPoint&);

#endif	/* ! __ZIA_MOD_OPENSSL_HANDLER_HPP__ */
