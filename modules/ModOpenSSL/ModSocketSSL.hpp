#ifndef _MODSOCKETSSL_
# define _MODSOCKETSSL_

#include "network/IEndPoint.hpp"

class ModSocketSSL : public zia::api::network::IEndPoint
{
public:
  virtual			~ModSocketSSL(void);
  virtual Port		getPort(void) const {return this->_port;}
  virtual Socket		getSocket(void) const {return this->_socket;}
  virtual const Ip &		getIp(void) const {return this->_ip;}
  void				setSSL(SSL* ssl) {this->_ssl = ssl;}
  void				setCTX(SSL_CTX* ctx) {this->_ctx = ctx;}
  void				setMethod(SSL_METHOD* method) {this->_method = method;}
  SSL*				getSSL(void) {return this->_ssl;}
  SSL_CTX*		       	getCTX(void) {return this->_ctx;}
  SSL_METHOD*			getMethod(void) {return this->_method;}

private:
  Ip				_ip;
  Port				_port;
  Socket			_socket;
  SSL*				_ssl;
  SSL_CTX*			_ctx;
  SSL_METHOD*			_method;
};

#endif /* _MODSOCKETSSL_ */
