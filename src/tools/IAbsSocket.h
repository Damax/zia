#ifndef _IABSSOCKET_H_
#define _IABSSOCKET_H_
#ifdef _WIN32
# include <windows.h>
 typedef SOCKET	mysocket;
 typedef SOCKADDR    label;
 typedef SOCKADDR_IN    label_in;
#else // POSIX
 typedef int	mysocket;
 typedef struct sockaddr    label;
 typedef struct sockaddr_in    label_in;
#endif

#include "../api/network/IEndPoint.hpp"

class IAbsSocket: public zia::api::network::IEndPoint
{
 public:
  virtual		~IAbsSocket(){};
  virtual bool		createSocket(int af, int type, int protocol) = 0;
  virtual void		close() = 0;
  virtual bool		bind(int port) = 0;
  virtual int		send(const char *buff, int size_to_sendl) = 0;
  virtual int		recv(char *buff, int size_max_to_recv) = 0;
  virtual int		listen(int backlog) = 0;
  virtual int		waitForReading(int timeout) = 0;
  /* const Ip&		getIp() */
  /* {return 0;} */
  Port			getPort() const
  {
    return this->port;
  }
  Socket		getSocket() const
  {
    return this->socket;
  }
  const Ip&		getIp() const
  {
    return this->ip;
  }
  void			setPort(Port p)
  {
    this->port = p;
  }
  void			setIp(int ip)
  {
    this->ip = ip;
  }
 protected:
  Port	port;
  Ip	ip;
  Socket socket;
};


#endif // WIN32
