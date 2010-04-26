#ifndef _CABSUSOCKET_H_
# define _CABSUSOCKET_H_
#ifndef     _WIN32
# include <string>
# include <netinet/in.h>
# include <sys/types.h>

# include "IAbsSocket.h"
# include "../api/network/IEndPoint.hpp"

class IUSocket : public IAbsSocket
{
 public:
  IUSocket(int af = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);
  IUSocket(zia::api::network::IEndPoint*);
  IUSocket(const IUSocket& ref);
  IUSocket(Socket&, int, int, int, int);
  ~IUSocket();

  IUSocket&		operator=(const IUSocket& ref);
 protected:
  bool			createSocket(int af, int type, int protocol);
 public:
  void			close();
  bool			bind(int port);
  bool			connect(const std::string &to_connect, int port);
  IUSocket*		accept();
  int			send(const char *buff, int size_to_sendl);
  int			recv(char *buff, int size_max_to_recv);
  void			setSocket(mysocket);
  int			listen(int);
  int			waitForReading(int timeout);

private:
  label_in		_sockaddr;
};

#endif // WIN32
#endif // _CABSUSOCKET_H_
