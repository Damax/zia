#ifndef _CABSWSOCKET_H_
# define _CABSWSOCKET_H_
#ifdef _WIN32
# include "IAbsSocket.h"
# include <Windows.h>
# include <iostream>

class IWSocket : public IAbsSocket
{
 public:
  IWSocket(int af = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);
  IWSocket(const IWSocket& ref);
  IWSocket(mysocket& socket);
  ~IWSocket(void);

  IWSocket&			operator=(const IWSocket& ref);
 protected:
  bool				createSocket(int af, int type, int protocol);
 public:
  void				close();
  bool				bind(int port);
  bool				connect(const std::string &to_connect, int port);
  IWSocket*			accept();
  int				send(const char *buff, int size_to_sendl);
  int				recv(char *buff, int size_max_to_recv);
  int			        recvfrom(char *buff, int size_to_recv, label* from);
  int				sendto(const char *buff, int size_to_send, label* to);
  mysocket&			getSocket();
  int				listen(int backlog = SOMAXCONN);
  int				waitForReading(int timeout);
 private:
  WSADATA			_wsaData;
  static int			_nb_socket;
  mysocket			_socket;
  label_in			_sockaddr;
};

# endif // WIN32
#endif // _CABSWSOCKET_H_
