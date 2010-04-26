#ifdef      _WIN32
#include <iostream>
//#include <Windows.h>
#include <Winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include "IWSocket.hpp"

int IWSocket::_nb_socket = 0;

IWSocket::IWSocket(int af, int type, int protocol) : _socket(INVALID_SOCKET)
{
  if (this->_nb_socket == 0)
    {
      int err = WSAStartup(MAKEWORD(2, 2), &(this->_wsaData));
      if (err != 0)
	std::cerr << "WSAStartup failed with error: " << err << std::endl;
    }
  if (!this->createSocket(af, type, protocol))
    return ;
  ++this->_nb_socket;
}

IWSocket::IWSocket(const IWSocket& ref) : _socket(ref._socket)
{
  memcpy(&this->_sockaddr, &ref._sockaddr, sizeof(this->_sockaddr));
  ++this->_nb_socket;
}

IWSocket::IWSocket(mysocket& socket) : _socket(socket)
{
  ++this->_nb_socket;
}

IWSocket::~IWSocket(void)
{
  --this->_nb_socket;
  this->close();
  if (this->_nb_socket == 0)
      WSACleanup();
}

IWSocket&	IWSocket::operator=(const IWSocket& ref)
{
  this->_socket = ref._socket;
  memcpy(&this->_sockaddr, &ref._sockaddr, sizeof(this->_sockaddr));
  return *this;
}

bool		IWSocket::createSocket(int af, int type, int protocol)
{
  this->_socket = WSASocket(af, type, protocol, NULL, 0, WSA_FLAG_OVERLAPPED);
  if (this->_socket == INVALID_SOCKET)
    {
      std::cerr << "[Win] Failed to create socket" << std::endl;
      return false;
    }
  return true;
}

void		IWSocket::close()
{
  if (this->_socket == INVALID_SOCKET)
    return ;
  closesocket(this->_socket);
  this->_socket = INVALID_SOCKET;
}

bool		IWSocket::bind(int port)
{
  this->_sockaddr.sin_family = AF_INET;
  this->_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  this->_sockaddr.sin_port = port;
  u_short     tmp;
  if (WSAHtons(this->_socket, port, &tmp) == SOCKET_ERROR)
    return false;
  this->_sockaddr.sin_port = tmp;
  if (::bind(this->_socket, reinterpret_cast<sockaddr *> (&(this->_sockaddr)), sizeof(this->_sockaddr)) == SOCKET_ERROR)
    return false;
  return true;
}

bool		IWSocket::connect(const std::string &to_connect, int port)
{
  if (WSAConnect(this->_socket,
		 reinterpret_cast<sockaddr*> (&(this->_sockaddr)),
		 sizeof(this->_sockaddr),
		 NULL, NULL, NULL, NULL) == SOCKET_ERROR)
    return false;
  return true;
}

IWSocket*	IWSocket::accept()
{
  if (::listen(this->_socket, 0xA2) == SOCKET_ERROR)
    return NULL;
  int size_addr = sizeof(this->_sockaddr);
  mysocket s = WSAAccept(this->_socket, reinterpret_cast<sockaddr*> (&this->_sockaddr), &size_addr, NULL, NULL);
  if (s == SOCKET_ERROR)
    return NULL;
  return new IWSocket(s);
}

int		IWSocket::send(const char *buff, int size_to_send)
{
  int		nbSent = 0;
  int		err = 0;
  WSABUF	buf;
  buf.buf = const_cast<char *> (buff);
  buf.len = size_to_send;
  int rc = WSASend(this->_socket, &buf, 1,
		   reinterpret_cast<LPDWORD> (&nbSent), 0, NULL, NULL);
  if ((rc == SOCKET_ERROR) && (WSA_IO_PENDING != (err = WSAGetLastError())))
    {
      std::cerr << "WSASend failed: " << err << std::endl;
      return -1;
    }
  return nbSent;
}

int		IWSocket::recvfrom(char *buff, int size_to_recv, label* from)
{
  int     sizeSockaddr = sizeof(label_in);
  if (this->_socket == INADDR_ANY)
    {
      std::cerr << "[Error][send] socket is not configure" << std::endl;
      return -1;
    }
  return ::recvfrom(this->_socket, buff, size_to_recv, 0, from, &sizeSockaddr);
}

int		IWSocket::sendto(const char *buff, int size_to_send, label* to)
{
  int       sizeSockaddr = sizeof(label_in);

  if (this->_socket == INADDR_ANY)
    {
      std::cerr << "[Error][send] socket is not configure" << std::endl;
      return -1;
    }
  return ::sendto(this->_socket, buff, size_to_send, 0,  to, sizeSockaddr);
}

mysocket&	IWSocket::getSocket()
{
  return this->_socket;
}

int		IWSocket::recv(char *buff, int size_to_recv)
{
  int		nbRecv = 0;
  int		err = 0;
  DWORD		Flags = 0;
  WSABUF	buf;

  buf.buf = buff;
  buf.len = size_to_recv;
  int rc = WSARecv(this->_socket, &buf, 1,
		   reinterpret_cast<LPDWORD> (&nbRecv), &Flags, NULL, NULL);
  if ((rc == SOCKET_ERROR) && (WSA_IO_PENDING != (err = WSAGetLastError()) ) )
    {
      std::cerr << "WSARecv failed: " << err << std::endl;
      return -1;
    }
  return nbRecv;
}

int		IWSocket::listen(int backlog)
{
  return ::listen(this->_socket, backlog);
}

int		IWSocket::waitForReading(int timeout)
{
  fd_set readfs;
  FD_ZERO(&readfs);
  FD_SET(this->_socket, &readfs);

  struct timeval time;

  int ready;
  if (timeout >= 0)
    {
      time.tv_sec = timeout;
      time.tv_usec = 0;
      ready = select(2, &readfs, NULL, NULL, &time);
    }
  else
    {
      ready = select(2, &readfs, NULL, NULL, NULL);
    }
  return ready;
}


#endif
