#ifndef SERVCLIENT_H
#define SERVCLIENT_H

#include <vector>
#include <iostream>

#include "Parser.hpp"
#include "ResponseBuilder.hpp"
#include "Transaction.hpp"

#include "tools/ziasocket.hpp"
#include "tools/ziathread.hpp"

#include "api/network/IEndPoint.hpp"
#include "api/IModule.hpp"

class Client: public ZiaThread
{
  Client(){};
  Client&	operator=(const Client&){return *this;};
public:
  ~Client();
  Client(ZiaSocket*, int);
  void		Run(void);
  void		doHookRequestHeaderModifier(Transaction&, std::istream&, std::ostream&);
  void		doHookRequestBodyModifier(Transaction&, std::istream&, std::ostream&);
  void		doHookResponseProcessor(Transaction&, std::istream&, std::ostream&);
  void		doHookResponseHeaderModifier(Transaction&, std::istream&, std::ostream&);
  void		doHookResponseBodyModifier(Transaction&, std::istream&, std::ostream&);
  bool		checkRequestValidity(Transaction&);
  void		checkResponseValidity(Transaction&);
  void		setResponseContentLength(Transaction&, std::iostream&);
  void		sendResponse(Transaction&, std::stringbuf&);
  void		setPathFromUri(Transaction&);
  int		getRequestedTimeout(Transaction&, int);
  void		exit(void);

private:
  ZiaSocket*	_socket;
  Parser	_parser;
  std::string	port;
  Transaction	transaction;
  bool		header_modified;
};

#endif // !SERVCLIENT_H
