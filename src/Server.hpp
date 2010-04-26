#ifndef SERVERZIA_H
#define SERVERZIA_H

#include "Client.hpp"

#include "tools/ziasocket.hpp"
#include "tools/ziathread.hpp"
#include "ModuleManager.hpp"
#include "handler/INetworkHandler.hpp"
#include <stdlib.h>
#include <vector>

/***
   This class represents a port (with one or more virtual_host)
   (it does bind, listen on a specific port and then waits for
   clients (with accept))
   When a new Client connects, it is added in the clients list.
   For example, if the configuration is like
   one.localhost:80
   two.localhost:80
   localhost:80
   loalhost:443
   There will be 2 Server. The one on port 80 will then choose
   the correct virtual_host using the host name.
 **/

class Server: public ZiaThread
{
public:

  Server(int p);
  Server();
  Server(const Server& ref);
  ~Server(void);
  void			Run(void);
  int			getPort(void);
  std::string		getPortStr(void);
  void			setPort(int);
  void			exit(void);
  void			doHookAccept(ZiaSocket*);
private:
  void			WaitForClientLoop();
  void			putOnThread(ZiaSocket*);

  std::vector<Client*> clients;
  ZiaSocket	s_listen;
  int		port;

  Server&		operator=(const Server& ref);
};

#endif // !SERVERZIA_H
