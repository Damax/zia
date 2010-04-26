#ifndef __TRANSACTION__
#define __TRANSACTION__

#include <iostream>

#include "api/http/ITransaction.hpp"
#include "api/http/IHeader.hpp"
#include "Request.hpp"
#include "Response.hpp"

#include "tools/ziasocket.hpp"

class Transaction: public zia::api::http::ITransaction
{
  Transaction(const Transaction&){};
  Transaction&				operator=(const Transaction&){return *this;};
public:
  Transaction(): client_endpoint(NULL), server_endpoint(NULL)
  {};
  ~Transaction(){};
  const zia::api::http::IRequest&	getRequest(void) const
  {
    return this->request;
  }
  const zia::api::http::IResponse&	getResponse(void) const
  {
    return this->response;
  }
  const zia::api::network::IEndPoint&	getClientEndPoint(void) const
  {
    return *(this->client_endpoint);
  }
  const zia::api::network::IEndPoint&	getServerEndPoint(void) const
  {
    return *(this->server_endpoint);
  }
  zia::api::http::IRequest&		getRequest(void)
  {
    return this->request;
  }
  zia::api::http::IResponse&		getResponse(void)
  {
    return this->response;
  }
  zia::api::network::IEndPoint&		getClientEndPoint(void)
  {
    return *(this->client_endpoint);
  }
  zia::api::network::IEndPoint&		getServerEndPoint(void)
  {
    return *(this->server_endpoint);
  }
  void					clear()
  {
    this->request.clear();
    this->response.clear();
  }
  void					setServerEndPoint(ZiaSocket* ie)
  {
    this->server_endpoint = ie;
  }
  void					setClientEndPoint(ZiaSocket* ie)
  {
    this->client_endpoint = ie;
  }
private:
  Request				request;
  Response				response;
  ZiaSocket*				client_endpoint;
  ZiaSocket*				server_endpoint;
};

#endif
