#ifndef __ADMINCONSOLE__
# define __ADMINCONSOLE__

#include "Zia.hpp"

class AdminConsole
{
public:
  AdminConsole(Zia*);
  ~AdminConsole(){};
  void	readLine();
  void	execCommand(const std::string&, const std::string&);
private:
  AdminConsole& operator=(const AdminConsole&);
  AdminConsole(const AdminConsole&);
  Zia* _zia;
};

#endif /* __ADMINCONSOLE__ */
