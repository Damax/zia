#include "ModLoggerHandler.hpp"
#include <iostream>
#include <fstream>

#include <arpa/inet.h>

#include <time.h>

void	ModLoggerHandler::setFile(std::ofstream *file)
{
    this->_file = file;
}

zia::api::handler::EHook    ModLoggerHandler::getHook(void) const
{
  return (zia::api::handler::RequestHeaderModifier);
}

zia::api::handler::ECode    ModLoggerHandler::operator()(zia::api::http::ITransaction &transac)
{
  std::cout << "BEGIN MODLOGGER" << std::endl;
  struct tm *current;
  time_t now;

  time(&now);
  current = localtime(&now);


  *(this->_file) << transac.getClientEndPoint().getIp() << " - ";
  *(this->_file) << "[" ;
  *(this->_file) << current->tm_mday << "/" << current->tm_mon+1 << "/" << current->tm_year+1900 << ":" << current->tm_hour << ":" << current->tm_min << ":" << current->tm_sec;
  *(this->_file) << " \"" << transac.getRequest().getMethod() << " " << transac.getRequest().getUri() << " " << transac.getRequest().getVersion() << "\"" << std::endl;
  std::cout << "END MODLOGGER" << std::endl;

  return (zia::api::handler::Ok);
}
