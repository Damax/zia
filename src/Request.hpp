#ifndef __REQUEST__
#define __REQUEST__

#include <string>

#include "api/http/IRequest.hpp"
#include "Header.hpp"

class Request: public zia::api::http::IRequest
{
  Request(const Request&){};
  Request&				operator=(const Request&){return *this;};
public:
  Request(){};
  ~Request(){};

  const std::string&			getMethod(void) const
  {
    return this->method;
  }
  void					setMethod(const std::string & m)
  {
    this->method = m;
  }
  const std::string&			getUri(void) const
  {
    return this->uri;
  }
  void					setUri(const std::string & u)
  {
    this->uri = u;
  }
  const std::string&			getPath(void) const
  {
    return this->path;
  }
  void					setPath(const std::string& path)
  {
    this->path = path;
  }
  const std::string&			getVersion(void) const
  {
    return this->version;
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
  void					clear(void)
  {
    this->method.clear();
    this->uri.clear();
    this->version.clear();
    this->headers.clear();
  }
  std::string				dump() const
  {
    std::string str(this->method + " ");
    str += this->uri + " ";
    str += this->version + "\r\n";
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
  std::string	method;
  std::string	uri;
  std::string	version;
  std::string	path;
  Header	headers;

};

#endif
