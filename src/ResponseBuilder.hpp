#ifndef __RESPONSEBUILDER__
# define __RESPONSEBUILDER__

#include <string>
#include <map>
#include <iostream>
#include <vector>

#include "Parser.hpp"
#include "Transaction.hpp"

class ResponseBuilder;

typedef  bool (ResponseBuilder::*method_ptr)();

class ResponseBuilder
{
  ResponseBuilder(){};
  ResponseBuilder&	operator=(const ResponseBuilder&){return *this;};
public:
  ResponseBuilder(Transaction&, std::istream&, std::ostream&);
  ~ResponseBuilder(){};
  void		buildResponse(void);
  void		defaultURI(void);
  bool		checkHttpVersion(void);
  std::string	createBody(const char *) const;
  bool		checkImplementedMethod(void);
  bool		checkPermission(const std::string&);
  void		notFound(const std::string&);
  void		movedPermanently(const std::string&);
  void		permissionDenied(const std::string&);
  std::string	buildFilename(void);
  void		directoryListing(const std::string&);
  const std::string	tryDefaultFile(const std::string&);
  void		sendFile(const std::string&);
  bool		doGet(void);
  bool		doHead(void);
private:
  std::map<std::string, method_ptr> methods;
  Transaction* trans;
  std::ostream*	os;
  std::istream*	is;
};

#endif
