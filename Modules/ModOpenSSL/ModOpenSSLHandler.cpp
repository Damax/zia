#include <iostream>
#include <fstream>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "ModOpenSSLHandler.hpp"


ModOpenSSLHandler::ModOpenSSLHandler()
{

}

zia::api::handler::EHook    ModOpenSSLHandler::getHook(void) const
{
  return (zia::api::handler::NetworkModifier);
}

zia::api::handler::ECode    ModOpenSSLHandler::operator()(zia::api::http::ITransaction &transac)
{
  return (zia::api::handler::Ok);
}

zia::api::handler::INetworkHandler::sPtrFunc	ModOpenSSLHandler::getNetworkHandlers(void)
{
  sPtrFunc s;
  s.sock_accept = &my_sock_accept;
  s.sock_read  = &my_sock_read;
//   s.sock_write = &my_sock_write;
//   s.sock_close = &my_sock_close;
  return s;
}
bool	initSSL(ModSocketSSL* server)
{
//   std::multimap<std::string, std::string>::iterator it;
//   for (it = conf.begin(); it != conf.end(); ++it)
//     if ((*it).first == "hosts")
//       hostsList.push_back(atoi((*splitString(it->second, ":"))[1].data()));
  SSL_library_init();
  OpenSSL_add_all_algorithms();
  SSL_load_error_strings();
  server->setMethod(SSLv23_server_method());
  if (server->getMethod() == NULL)
    {
      std::cout << "[SSL] Error during CTX allocation" <<std::endl;
      unsigned long ErrCode = ERR_get_error();
      char ErrBuf[120];
      ERR_error_string(ErrCode, ErrBuf);
      //      fprintf(stderr, "\t%s\n", ErrBuf);
      std::cerr << "\t" << ErrBuf << std::endl;
      return false;
    }
  server->setCTX(SSL_CTX_new(server->getMethod()));
  if (server->getCTX() == NULL)
    {
      std::cout << "[SSL] Error during CTX allocation" <<std::endl;
      unsigned long ErrCode = ERR_get_error();
      char ErrBuf[120];
      ERR_error_string(ErrCode, ErrBuf);
      //      fprintf(stderr, "\t%s\n", ErrBuf);
      std::cerr << "\t" << ErrBuf << std::endl;
      return false;
    }
  return true;
}

zia::api::network::IEndPoint*	my_sock_accept(zia::api::network::IEndPoint& server)
{
  //   for (std::vector<int>::iterator it = hostsList.begin(); it != hostsList.end() ; ++it)
  //     if (*it == server.getPort())
  //       {
  ModSocketSSL* serv = static_cast<ModSocketSSL*>(&server);
  initSSL(serv);
  loadCert(serv, "certificate/cert", "certificate/pkey");
  serv->setSSL(SSL_new(serv->getCTX()));
  SSL_set_fd(serv->getSSL(), serv->getSocket());
  if ((SSL_accept(serv->getSSL())) == -1)
    ERR_print_errors_fp(stderr);
  std::cout << "SSL_ACCEPT !!!!" << std::endl;
  // 	return (&server);
  //       }
  return (&server);
}


ssize_t             my_sock_read(zia::api::network::IEndPoint& client, void* buffer, size_t len)
{
  std::cout << "SSL_READ !!!!" << std::endl;
  ModSocketSSL* serv = static_cast<ModSocketSSL*>(&client);
  ssize_t l;
  if ((l = SSL_read(serv->getSSL(), buffer, len)) < 0)
    ERR_print_errors_fp(stderr);
  return (l);
}

void	loadCert(ModSocketSSL* server, const std::string& cFile, const std::string& kFile)
{
  if ( SSL_CTX_use_certificate_chain_file(server->getCTX(), cFile.c_str()) <= 0)
    {
      ERR_print_errors_fp(stderr);
      _exit(1); // TODO exit ca pue VRAIMENT faire le truc cool de jacky pushback + flag
    }
  if ( SSL_CTX_use_PrivateKey_file(server->getCTX(), kFile.c_str(), SSL_FILETYPE_PEM) <= 0)
    {
      ERR_print_errors_fp(stderr);
      _exit(1);
    }

  // Verify that the two keys goto together.
  if (!SSL_CTX_check_private_key(server->getCTX()))
    {
      fprintf(stderr, "Private key is invalid.\n");
      _exit(1);
    }
}
