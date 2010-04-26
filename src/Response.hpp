#ifndef __RESPONSE__
#define __RESPONSE__

#include <string>

#include "api/http/IResponse.hpp"
#include "api/http/IHeader.hpp"
#include "Header.hpp"
#include "common/utils.hpp"

class Response: public zia::api::http::IResponse
{
  Response(const Response&){};
  Response&				operator=(const Response&){return *this;};
public:
  Response(){
    this->version = "HTTP/1.1";};
  ~Response(){};
  StatusCode				getStatusCode(void) const
  {
    return this->status_code;
  }
  const std::string&			getStatusMessage(void) const
  {
    return this->status_message;
  }
  const std::string&			getVersion(void) const
  {
    return this->version;
  }
  void					setStatusCode(StatusCode code)
  {
    this->status_code = code;
  }
  void					setStatusMessage(const std::string & msg)
  {
    this->status_message = msg;
  }
  void					setVersion(const std::string & v)
  {
    this->version = v;
  }
  const zia::api::http::IHeader&	getHeaders(void) const
  {
    return this->headers;
  }
  zia::api::http::IHeader&		getHeaders(void)
  {
    return this->headers;
  }
  void					clear()
  {
    this->status_code = 0;
    this->status_message.clear();
    this->version = "HTTP/1.1";
    this->headers.clear();
  }
  std::string				dump() const
  {
    std::cout << "dans le dump: " << this->version << " " << inttostr(this->status_code) << this->status_message << std::endl;
    std::string str(this->version + " ");
    str += inttostr(this->status_code) + " ";
    str += this->status_message + "\r\n";
    std::map<std::string, std::string>::iterator it;
    std::map<std::string, std::string>	headers = this->headers.getAsMap();
    for (it = headers.begin();it != headers.end(); ++it)
      {
	str += (*it).first;
	str += ": ";
	str += (*it).second;
	str += "\r\n";
      }
    str += "\r\n";
    return str;
  }
private:
  StatusCode				status_code;
  std::string				status_message;
  std::string				version;
  Header				headers;
};

#endif
