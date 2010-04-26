#ifndef __HOOK__
#define __HOOK__

#include "Client.hpp"
#include "tools/ziathread.hpp"

/***
    Class running an handler in a Thread and using
    the streams correctly
 **/
typedef	void(Client::*MethType)(std::istream&, std::ostream&);

class Hooker: public ZiaThread
{
public:
  Hooker(){};
  Hooker(MethType, Transaction&, std::istream&, std::ostream&);
  Hooker(MethType);
  ~Hooker(){};
  void	Run(void);
private:
  MethType	meth;
};

#endif
