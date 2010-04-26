#include "Parser.hpp"

#include "common/utils.hpp"

#include <stdlib.h>
#include <iostream>
#include <string>

Parser::Parser():
  state(parserequest)
{
}

void		Parser::setState(State s)
{
  this->state = s;
}

Parser::State	Parser::Consume(const char *str, Transaction& transaction, std::iostream& is)
{
  std::string  	line;
  size_t	found;

  std::string tmp(str);
  std::cout << str << std::endl; // TODO
  if (tmp.find("\n") == tmp.find_first_not_of(" \r"))
    return this->state;
  this->buffer += str;
  while ((found = this->buffer.find("\n")) != std::string::npos)
    {
      line = this->buffer.substr(0, found+1);
      this->buffer = this->buffer.substr(found+1);
      if (this->state == parserequest)
	this->parseRequest(line, transaction);
      else if (this->state == parseheader)
  	this->parseHeader(line, transaction);
      else if (this->state == parsebody)
  	this->parseBody(line, transaction, is);
    }
  return this->state;
}

void	Parser::initInfo(void)
{
}

void	Parser::parseBody(const std::string& str, Transaction& trans, std::iostream& is)
{
  unsigned int	missing_length;
  std::string	value;
  int is_length;

  trans.getRequest().getHeaders().getValue("CONTENT-LENGTH", value);
  is.seekg(0, std::ios::end);
  is_length = is.tellg();
  is.seekg(0, std::ios::beg);
  missing_length = atoi(value.data()) - is_length;
  if (missing_length <= str.length())
    {
      is << str.substr(0, missing_length);
      this->state = finished;
    }
  else
    is << str;
}

void	Parser::parseHeader(std::string str, Transaction& trans)
{
  // check si c'est pas la ligne vide qui signifie la fin des headers
  if (str.find("\n") == str.find_first_not_of(" \r"))
    {
      std::string value;
      if (((trans.getRequest().getHeaders().getValue("CONTENT-LENGTH", value)  == true) &&
	   (atoi(value.data()) != 0)) ||
	  (trans.getRequest().getHeaders().getValue("TRANSFER-ENCODING", value)  == true))
	{ // voir "4.3 Message Body" de la rfc. Si y'a pas ces headers, on n'attend pas de body
	  this->state = parsebody;
	  return ;
	}
      else
	{
	  this->state = finished;
	  return ;
	}
    }
  size_t	found;
  if ((found = str.find(":")) == std::string::npos)
    return ;
  size_t begin  = str.find_first_not_of(" ");
  std::string header_name = str.substr(begin, str.find_first_of(" :", begin));
  begin = str.find_first_not_of(" ", found+1);
  str = str.substr(begin);
  std::string header_value = str.substr(0, str.find_first_of("\n\r", begin));
  trans.getRequest().getHeaders().setValue(struppercase(header_name), header_value);
}

void	Parser::parseRequest(std::string str, Transaction& trans)
{
  size_t	found;
  int		i;

  for (i = 0; i != 3; ++i)
    {
      if (str.size() <= 0)
	return ;
      str = str.substr(str.find_first_not_of(" "));
      if ((found = str.find_first_of(" \r\n")) == std::string::npos)
	  return ;
      if (i == 0)
	trans.getRequest().setMethod(str.substr(0, found));
      else if (i == 1)
	{
	  trans.getRequest().setUri(str.substr(0, found));
	}
      else if (i == 2)
	trans.getRequest().setVersion(str.substr(0, found));
      str = str.substr(found+1);
    }
  this->state = parseheader;
}
