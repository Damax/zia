#ifndef   _WIN32
#include <iostream>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <signal.h>
#include <errno.h>
#include <typeinfo>

#include "IUSocket.hpp"
#include "ziasocket.hpp"

#include "../ModuleManager.hpp"
#include "../api/handler/INetworkHandler.hpp"

IUSocket::IUSocket(zia::api::network::IEndPoint* ep)
{
  this->socket = ep->getSocket();
  std::cout << inet_ntoa(this->_sockaddr.sin_addr) << std::endl;
  //  this->ip = ep->getIp();
}

IUSocket::IUSocket(int af, int type, int protocol)
{
  this->socket = INADDR_ANY;
  if (!this->createSocket(af, type, protocol))
    {
      std::cout << "Error in create socket" << std::endl;
      return ;
    }
}

IUSocket::IUSocket(const IUSocket& ref)
{
  this->socket = ref.socket;
  ::memcpy(&this->_sockaddr, &ref._sockaddr, sizeof(this->_sockaddr));
}

IUSocket::IUSocket(Socket& socket, int, int, int, int)
{
  this->socket = socket;
}

IUSocket::~IUSocket()
{
  if (this->socket != INADDR_ANY)
    this->close();
}

IUSocket&		IUSocket::operator=(const IUSocket& ref)
{
  this->socket = ref.socket;
  ::memcpy(&this->_sockaddr, &ref._sockaddr, sizeof(this->_sockaddr));
  return *this;
}

bool			IUSocket::createSocket(int af, int type, int protocol)
{
  this->socket = ::socket(af, type, protocol);
  if (this->socket == 0)
    {
      std::cerr << "[Error][createSocket] Socket was not created" << std::endl;
      this->socket = INADDR_ANY;
      return false;
    }
  int yes = 1;
  if (setsockopt(this->socket, SOL_SOCKET,SO_REUSEADDR, &yes, sizeof(int)) == -1)
    {
      return false;
    }
  return true;
}

void			IUSocket::close()
{
  if (this->socket == INADDR_ANY)
    return ;
  if (::close(this->socket) == -1)
    std::cout << "close failed" << std::endl;
  this->socket = INADDR_ANY;
}

bool			IUSocket::bind(int port)
{
  this->_sockaddr.sin_family = AF_INET;
  this->_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  this->_sockaddr.sin_port = htons(port);
  if (this->socket == INADDR_ANY)
    {
      std::cerr << "[Error][bind] socket not created" << std::endl;
      return false;
    }
  if (::bind(this->socket, (label*) & (this->_sockaddr), sizeof(label_in)) < 0)
    return false;
  return true;
}

bool			IUSocket::connect(const std::string &to_connect, int port)
{
  this->_sockaddr.sin_family = AF_INET;
  this->_sockaddr.sin_addr.s_addr = ::inet_addr(to_connect.data());
  this->_sockaddr.sin_port = port;
  if (::connect(this->socket, reinterpret_cast<struct sockaddr *>(&this->_sockaddr), sizeof(this->_sockaddr)) == -1)
    {
      std::cerr << "[Error][connect] Fail to connect to peer [" << to_connect << "]" <<std::endl;
      return false;
    }
  return true;
}

IUSocket*		IUSocket::accept()
{
  if (::listen(this->socket, 0xC) == -1)
    {
      std::cerr << "[Error][listen] Fail to listen socket" << std::endl;
      return NULL;
    }
  socklen_t len = sizeof(this->_sockaddr);
  fd_set fdread;
  FD_ZERO(&fdread);
  FD_SET(this->socket, &fdread);
  ::select(this->socket+1, &fdread, NULL, NULL, NULL);
  int yes = 1;
  if (setsockopt(this->socket, SOL_SOCKET,SO_REUSEADDR, &yes, sizeof(int)) == -1)
    return NULL;

  std::map<std::string, zia::api::IModule*>::iterator it;
  std::map<std::string, zia::api::IModule* > modules = ModuleManager::Inst().getModules();
  for (it = modules.begin(); it != modules.end(); ++it)
    {
      std::vector<zia::api::handler::IHandler *> handlers;
      std::vector<zia::api::handler::IHandler *>::iterator handler_it;
      std::cout << it->first << std::endl;
      handlers = it->second->getHandlers();
      for (handler_it = handlers.begin(); handler_it != handlers.end(); ++handler_it)
	{
	  if ((*handler_it)->getHook() == zia::api::handler::NetworkModifier)
	    {
	      zia::api::handler::INetworkHandler* in =  dynamic_cast<zia::api::handler::INetworkHandler *>(*handler_it);
	      if (in != NULL && in->getNetworkHandlers().sock_accept != NULL)
		return new ZiaSocket(in->getNetworkHandlers().sock_accept(*this));
	    }
	}
    }
  Socket newSocket = ::accept(this->socket, reinterpret_cast<struct sockaddr*> (&this->_sockaddr), &len);
  if (newSocket < 0)
    {
      std::cerr << "[Error][accept] Fail to accept peer" << std::endl;
      return NULL;
    }
  IUSocket* so  = new IUSocket(newSocket, 0, 0, 0, 0);
  so->setIp(this->_sockaddr.sin_addr.s_addr);
  return so;
}

int			IUSocket::send(const char *buff, int size_to_send)
{
  if (this->socket == INADDR_ANY)
    {
      std::cerr << "[Error][send] socket is not configured" << std::endl;
      return -1;
    }
  std::map<std::string, zia::api::IModule*>::iterator it;
  std::map<std::string, zia::api::IModule* > modules = ModuleManager::Inst().getModules();
  for (it = modules.begin(); it != modules.end(); ++it)
    {
      std::vector<zia::api::handler::IHandler *> handlers;
      std::vector<zia::api::handler::IHandler *>::iterator handler_it;
      handlers = it->second->getHandlers();
      for (handler_it = handlers.begin(); handler_it != handlers.end(); ++handler_it)
	{
	  if ((*handler_it)->getHook() == zia::api::handler::NetworkModifier)
	    {
	      zia::api::handler::INetworkHandler* in =  dynamic_cast<zia::api::handler::INetworkHandler *>(*handler_it);
	      if (in != NULL && in->getNetworkHandlers().sock_write != NULL)
		{
		  // TODO louiz verif this
		  return in->getNetworkHandlers().sock_write(*this, buff, size_to_send);
		}
	    }
	}
    }
  return ::send(this->socket, buff, size_to_send, 0);
}

int			IUSocket::recv(char *buff, int size_to_recv)
{
  if (this->socket == INADDR_ANY)
    {
      std::cerr << "[Error][send] socket is not configure" << std::endl;
      return -1;
    }
  std::map<std::string, zia::api::IModule*>::iterator it;
  std::map<std::string, zia::api::IModule* > modules = ModuleManager::Inst().getModules();
  for (it = modules.begin(); it != modules.end(); ++it)
    {
      std::vector<zia::api::handler::IHandler *> handlers;
      std::vector<zia::api::handler::IHandler *>::iterator handler_it;
      handlers = it->second->getHandlers();
      for (handler_it = handlers.begin(); handler_it != handlers.end(); ++handler_it)
	{
	  if ((*handler_it)->getHook() == zia::api::handler::NetworkModifier)
	    {
	      zia::api::handler::INetworkHandler* in =  dynamic_cast<zia::api::handler::INetworkHandler *>(*handler_it);
	      if (in != NULL && in->getNetworkHandlers().sock_read != NULL)
		{
		  // TODO louiz verif this
		  return in->getNetworkHandlers().sock_read(*this, buff, size_to_recv);
		}
	    }
	}
    }
  std::cout << this->socket << std::endl;
  return ::recv(this->socket, buff, size_to_recv, 0);
}

void			IUSocket::setSocket(mysocket socket)
{
  this->socket = socket;
}

int			IUSocket::listen(int backlog)
{
  return ::listen(this->socket, backlog);
}

int			IUSocket::waitForReading(int timeout)
{
  fd_set readfs;
  FD_ZERO(&readfs);
  FD_SET(this->socket, &readfs);

  struct timeval time;
  int ready;
  time.tv_sec = timeout;
  time.tv_usec = 0;
  ready = select(this->socket+1, &readfs, NULL, NULL, &time);
  return ready;
}

#endif
