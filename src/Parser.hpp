#ifndef __PARSER__
# define __PARSER__

#include <string>
#include <iostream>

#include "Transaction.hpp"
#include "Request.hpp"
#include "Response.hpp"

class Parser
{
  Parser(const Parser&){};
  Parser	operator=(const Parser&){return *this;};
public:
  enum State
    {
      parserequest,
      parseheader,
      parsebody,
      finished
    };

  Parser();
  ~Parser(){};
  State		Consume(const char*, Transaction&, std::iostream&);
  void		parseRequest(std::string, Transaction&);
  void		parseHeader(std::string, Transaction&);
  void		parseBody(const std::string&, Transaction&, std::iostream&);
  void		initInfo(void);
  void		setState(State);
private:
  std::string buffer;
  Request	info;
  State		state;

};

#endif
