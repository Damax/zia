#ifndef _WIN32
# include <unistd.h>
#endif
#include <string>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sstream>

#include "Client.hpp"
#include "Parser.hpp"
#include "Hooker.hpp"
#include "ModuleManager.hpp"
#include "Conf.hpp"
#include "ResponseBuilder.hpp"

#include "tools/ziasocket.hpp"
#include "common/utils.hpp"
#include "common/messages.hpp"
#include "api/network/IEndPoint.hpp"
#include "api/handler/IHeaderHandler.hpp"
#include "api/handler/IBodyHandler.hpp"


#include <typeinfo>

Client::Client(ZiaSocket* s, int p):
  ZiaThread(false),
  _socket(s)
{
  this->port = inttostr(p);
  this->transaction.setClientEndPoint(s);
  this->SetRelease(true);
}

Client::~Client(void)
{
  if (this->_socket != NULL)
    delete this->_socket;
}

void	Client::Run(void)
{
  char			buf[1024];
  Parser::State		state;
  std::stringbuf*	bufi = new std::stringbuf;
  std::stringbuf*	bufo = new std::stringbuf;
  std::iostream		is(NULL);
  std::iostream		os(NULL);
  os.rdbuf(bufo);
  is.rdbuf(bufi);
  int			timeout = atoi(Conf::Inst().get("connectiontimeout", "300").data());
  int			select_result;

  this->_parser.setState(Parser::parserequest);
  this->header_modified = false;
  while ((timeout > 0) && ((select_result = this->_socket->waitForReading(timeout)) > 0))
    {
      ::memset(buf, 0, 1024);
      if (this->_socket->recv(buf, 4096) <= 0)
  	{
	  this->exit();
  	  return ;
  	}
      state = this->_parser.Consume(buf, this->transaction, is);
      if ((state == Parser::parsebody && this->header_modified == false) || (state == Parser::finished))
  	{
	  timeout = this->getRequestedTimeout(this->transaction, timeout);
  	  this->doHookRequestHeaderModifier(this->transaction, is, os);
	  this->header_modified = true;
	}
      if ((state == Parser::finished))
	{
	  std::cout << is << std::endl;
	  this->doHookRequestBodyModifier(this->transaction, is, os);
	  if (this->checkRequestValidity(this->transaction) == true)
	    {
	      this->doHookResponseProcessor(this->transaction, is, os);
	    }
	  this->doHookResponseHeaderModifier(this->transaction, is, os);
	  this->setResponseContentLength(this->transaction, os);
	  this->doHookResponseBodyModifier(this->transaction, is, os);
	  this->sendResponse(this->transaction, *bufo);
	  this->transaction.clear();
	  delete bufi;
	  delete bufo;
	}
      bufi = new std::stringbuf;
      bufo = new std::stringbuf;
      is.rdbuf(bufi);
      os.rdbuf(bufo);
      this->_parser.setState(Parser::parserequest);
      this->header_modified = false;
    }
  if (select_result == -1)
    {
      std::cerr << "Error on select before read" << std::endl;
      this->exit();
      return ;
    }
  else if (select_result == 0)
    {
      // Client timeout, close it
      std::cout << "Client timeout BYE" << std::endl;
      this->exit();
      return;
    }
}

int	Client::getRequestedTimeout(Transaction& trans, int timeout)
{
  // Ici on check les headers:
  std::string result;
  if ((trans.getRequest().getHeaders().getValue("CONNECTION", result) == true) && (result == "close"))
    return -1;
  return timeout;
}

void	Client::checkResponseValidity(Transaction& trans)
{
  if (trans.getResponse().getStatusCode() <= 0)
    {
      trans.getResponse().setStatusCode(500);
      trans.getResponse().setStatusMessage(MESSAGE_500);
      trans.getResponse().getHeaders().setValue("CONTENT-LENGTH", "0");
    }
}

bool	Client::checkRequestValidity(Transaction& trans)
{
  std::string tmp;
  std::string version;


  if (trans.getRequest().getUri().length() > 255)
    {
      trans.getResponse().setStatusCode(414);
      trans.getResponse().setStatusMessage(MESSAGE_414);
      return (false);
    }
  if (trans.getRequest().getHeaders().getValue("HOST", tmp) == false)
    {
      trans.getResponse().setStatusCode(400);
      trans.getResponse().setStatusMessage(MESSAGE_400);
      return(false);
    }
  if (trans.getRequest().getVersion().substr(0, 7) != "HTTP/1.")
    {
      trans.getResponse().setStatusCode(400);
      trans.getResponse().setStatusMessage(MESSAGE_400);
      return(false);
    }
  return (true);
}

void	Client::sendResponse(Transaction& trans, std::stringbuf& responsebody)
{
  std::cout << "ici le dump:====\n" << trans.getResponse().dump() << std::endl;
  std::string str = trans.getResponse().dump();
  str += responsebody.str();
  str += "\r\n\r\n";
  this->_socket->send(str.data(), str.size()-1);
}

void	Client::doHookResponseHeaderModifier(Transaction& trans, std::istream&, std::ostream&)
{
  std::map<std::string, zia::api::IModule* > modules = ModuleManager::Inst().getModules();
  std::map<std::string, zia::api::IModule* >::iterator it_modules;
  for (it_modules = modules.begin(); it_modules != modules.end(); ++it_modules)
    {
      if (it_modules->second != NULL)
	{
	  std::vector<zia::api::handler::IHandler *> handlers = it_modules->second->getHandlers();
	  std::vector<zia::api::handler::IHandler *>::iterator it_handlers;
	  for (it_handlers = handlers.begin(); it_handlers != handlers.end(); ++it_handlers)
	    {
	      if ((*it_handlers)->getHook() == zia::api::handler::ResponseHeaderModifier)
		{
		  // appel du handler
		  zia::api::handler::IHeaderHandler* hand = static_cast<zia::api::handler::IHeaderHandler*>(*it_handlers);
		  (*hand)(trans);
		}
	    }
	}
    }
  // check if the status_code and status_message is set.
  // if not, put an error code 500.
  this->checkResponseValidity(trans);
}

void	Client::setResponseContentLength(Transaction& trans, std::iostream& os)
{
  os.seekg(0, std::ios::end);
  int os_length = os.tellg();
  os_length--;
  os.seekg(0, std::ios::beg);
  trans.getResponse().getHeaders().setValue("CONTENT-LENGTH", inttostr(os_length));
}

void	Client::doHookResponseBodyModifier(Transaction& trans, std::istream& is, std::ostream& os)
{
  std::map<std::string, zia::api::IModule* > modules = ModuleManager::Inst().getModules();
  std::map<std::string, zia::api::IModule* >::iterator it_modules;
  for (it_modules = modules.begin(); it_modules != modules.end(); ++it_modules)
    {
      if (it_modules->second != NULL)
	{
	  std::vector<zia::api::handler::IHandler *> handlers = it_modules->second->getHandlers();
	  std::vector<zia::api::handler::IHandler *>::iterator it_handlers;
	  for (it_handlers = handlers.begin(); it_handlers != handlers.end(); ++it_handlers)
	    {
	      if ((*it_handlers)->getHook() == zia::api::handler::ResponseBodyModifier)
		{
		  // appel du handler
		  zia::api::handler::IBodyHandler* hand = static_cast<zia::api::handler::IBodyHandler*>(*it_handlers);
		  if ((*hand)(trans, is, os) == zia::api::handler::Ok)
		    return ;
		}
	    }
	}
    }
}

void	Client::doHookResponseProcessor(Transaction& trans, std::istream& is, std::ostream& os)
{
  std::map<std::string, zia::api::IModule* > modules = ModuleManager::Inst().getModules();
  std::map<std::string, zia::api::IModule* >::iterator it_modules;
  for (it_modules = modules.begin(); it_modules != modules.end(); ++it_modules)
    {
      if (it_modules->second != NULL)
	{
	  std::vector<zia::api::handler::IHandler *> handlers = it_modules->second->getHandlers();
	  std::vector<zia::api::handler::IHandler *>::iterator it_handlers;
	  for (it_handlers = handlers.begin(); it_handlers != handlers.end(); ++it_handlers)
	    {
	      if ((*it_handlers)->getHook() == zia::api::handler::ResponseProcessor)
		{
		  // appel du handler
		  zia::api::handler::IBodyHandler* hand = static_cast<zia::api::handler::IBodyHandler*>(*it_handlers);
		  if ((*hand)(trans, is, os) == zia::api::handler::Ok)
		    return ;
		}
	    }
	}
    }
  // if no module processed the response, we do the default processing.
  ResponseBuilder builder(trans, is, os);
  builder.buildResponse();
}

void	Client::doHookRequestBodyModifier(Transaction& trans, std::istream& is, std::ostream& os)
{
  std::map<std::string, zia::api::IModule* > modules = ModuleManager::Inst().getModules();
  std::map<std::string, zia::api::IModule* >::iterator it_modules;
  for (it_modules = modules.begin(); it_modules != modules.end(); ++it_modules)
    {
      if (it_modules->second != NULL)
	{
	  std::vector<zia::api::handler::IHandler *> handlers = it_modules->second->getHandlers();
	  std::vector<zia::api::handler::IHandler *>::iterator it_handlers;
	  for (it_handlers = handlers.begin(); it_handlers != handlers.end(); ++it_handlers)
	    {
	      if ((*it_handlers)->getHook() == zia::api::handler::RequestBodyModifier)
		{
		  // appel du handler
		  zia::api::handler::IBodyHandler* hand = static_cast<zia::api::handler::IBodyHandler*>(*it_handlers);
		  if ((*hand)(trans, is, os) == zia::api::handler::Ok)
		    return ;
		}
	    }
	}
    }
}

void	Client::doHookRequestHeaderModifier(Transaction& trans, std::istream&, std::ostream&)
{
  this->setPathFromUri(trans);

  std::cout << "doHookRequestHeaderModifier BEGIN" << std::endl;

  std::map<std::string, zia::api::IModule* > modules = ModuleManager::Inst().getModules();
  std::map<std::string, zia::api::IModule* >::iterator it_modules;
  for (it_modules = modules.begin(); it_modules != modules.end(); ++it_modules)
    {
      if (it_modules->second != NULL)
	{
	  std::vector<zia::api::handler::IHandler *> handlers = it_modules->second->getHandlers();
	  std::vector<zia::api::handler::IHandler *>::iterator it_handlers;
	  for (it_handlers = handlers.begin(); it_handlers != handlers.end(); ++it_handlers)
	    {
	      if ((*it_handlers)->getHook() == zia::api::handler::RequestHeaderModifier)
		{
		  zia::api::handler::IHeaderHandler* hand = static_cast<zia::api::handler::IHeaderHandler*>(*it_handlers);
		  std::cout << "========= Name after: " << typeid(hand).name() << std::endl;
		  if (hand == NULL)
		    {
		      std::cout << "DYNAMIC_CAST QUI CHIE" << std::endl;
		      return;
		    }
		  if ( (*hand)(trans) == zia::api::handler::Ok)
		    return ;
		  std::cout << "It works" << std::endl;
		}
	    }
	}
    }
  std::cout << "doHookRequestHeaderModifier END" << std::endl;
}

void	Client::setPathFromUri(Transaction&  trans)
{
  std::string uri = trans.getRequest().getUri();
  std::string res;

  if (uri.substr(0, 7) == "http://")
    {
      size_t slash_pos = uri.find("/", 8);
      if (slash_pos == std::string::npos)
	res = "/";
      else
	res = uri.substr(slash_pos);
    }
  else
    res = uri;
  res = urldecode(res);
  std::string final;
  std::string port = inttostr(trans.getClientEndPoint().getPort());
  std::string host = trans.getRequest().getHeaders()["HOST"];
  final = Conf::Inst().get("documentroot", "/var/www", port, host);
  final += res;
  size_t inter_pos = final.find("?", 8);
  if (inter_pos != std::string::npos)
    {
      final = final.substr(0, inter_pos);
      std::string args = final.substr(inter_pos - 1);
    }
  trans.getRequest().setPath(final);
}

void	Client::exit(void)
{
  this->_socket->close();
}
