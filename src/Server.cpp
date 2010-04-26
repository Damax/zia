#include "Server.hpp"
#include "Conf.hpp"

#include "tools/ziasocket.hpp"
#include "tools/ziathread.hpp"

#include <iostream>
#include <sstream>

#include "Client.hpp"

Server::Server(int p):
  ZiaThread(false),
  port(p)
{
  this->SetRelease(true);
}


Server::Server()
{
}

Server::~Server(void)
{
  this->s_listen.close();
  this->exit();
}

void		Server::Run()
{
  if (this->s_listen.bind(this->port) == false)
    {
      std::cerr << "Bind failed. Port is busy" << std::endl;
      return ;
    }
  std::cout << "Starting new server on port " << this->port << std::endl;
  this->WaitForClientLoop();
}

void		Server::doHookAccept(ZiaSocket* s)
{
  std::map<std::string, zia::api::IModule* > modules = ModuleManager::Inst().getModules();
  std::map<std::string, zia::api::IModule* >::iterator it_modules;
  for (it_modules = modules.begin(); it_modules != modules.end(); ++it_modules)
    {
      if (it_modules->second != NULL)
	{
	  ModConf* conf = Conf::Inst().getModConf(it_modules->second->getName());
	  if (conf == NULL)
	    return;
	  zia::api::IConfig::ValueMap map = conf->getEntries();
	  std::multimap<std::string, std::string> multi = map["conf"];
	  std::multimap<std::string, std::string>::iterator it;
	  for (it = multi.begin(); it != multi.end(); ++it)
	    if ((*it).first == "hosts" && atoi((*splitString(it->second, ":"))[1].data()) == s->getPort())
	      {
		//	        hostsList.push_back(atoi((*splitString(it->second, ":"))[1].data()));
		std::vector<zia::api::handler::IHandler *> handlers = it_modules->second->getHandlers();
		std::vector<zia::api::handler::IHandler *>::iterator it_handlers;
		for (it_handlers = handlers.begin(); it_handlers != handlers.end(); ++it_handlers)
		  {
		    if ((*it_handlers)->getHook() == zia::api::handler::NetworkModifier)
		      {
			zia::api::handler::INetworkHandler* handler;
			if ((handler = static_cast<zia::api::handler::INetworkHandler*>(*it_handlers))
			    && (handler->getNetworkHandlers().sock_accept != NULL))
			  {
			    *(handler->getNetworkHandlers().sock_accept)(*s);
			    return;
			  }
		      }
		  }
	      }
	}
    }
  return;
}

  void		Server::WaitForClientLoop()
  {
    ZiaSocket*	s = this->s_listen.accept();
    s->setPort(this->port);
    this->doHookAccept(s);

    if (s != NULL)
      {
	this->putOnThread(s);
	this->WaitForClientLoop();
      }
    else
      {
	std::cerr << "Accept client failed!" << std::endl;
	return ;
      }
  }

  void		Server::putOnThread(ZiaSocket* s)
  {
    Client* serv = new Client(s, this->port);
    this->clients.push_back(serv);
  }

  int		Server::getPort(void)
  {
    return this->port;
  }

  std::string	Server::getPortStr(void)
  {
    std::stringstream out;
    out << this->port;
    return out.str();
  }

  void		Server::setPort(int p)
  {
    this->port = p;
  }

  void		Server::exit(void)
  {
  }
