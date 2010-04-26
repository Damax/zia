#ifndef __HEADER__
# define __HEADER__

#include <map>
#include <string>
#include <exception>

#include "api/http/IHeader.hpp"

class Header: public zia::api::http::IHeader
{
  Header(const Header&){};
  Header&	operator=(const Header&){return *this;};
public:
  Header(){};
  ~Header(){};
  virtual bool	getValue(const Key& k, Value & value) const
  {
    std::map<Key, Value>::const_iterator it;
    it = this->headers.find(k);
    if (it == this->headers.end())
      return false;
    value = it->second;
    return true;
  }
  virtual void	setValue(const Key & k, const Value & value)
  {
    this->headers[k] = value;
  }

  virtual const Value&	operator[](const Key & k) const
  {
    std::map<Key, Value>::const_iterator it;
    if ((it  = this->headers.find(k)) == this->headers.end())
      {
	std::exception e;
	throw e;
      }
    return it->second;
  }
  virtual Value&	operator[](const Key & k)
  {
    return this->headers[k];
  }
  virtual void	deleteKey(const Key& k)
  {
    std::map<Key, Value>::iterator it;
    if ((it = this->headers.find(k)) != this->headers.end())
      this->headers.erase(it);
  }
  void		clear(void)
  {
    this->headers.clear();
  }

  /**
     2010-04-13 18:28:20     @zo     bon, ce sera une methode getHasMap()
     2010-04-13 18:28:36     @zo     qui renvoie une const std::map<Key, Value> &
     2010-04-13 18:30:35     @zo     *getAsMap
  **/
  const std::map<Key, Value>&		getAsMap() const
  {
    return this->headers;
  }

private:
  std::map<Key, Value> headers;

};

#endif
