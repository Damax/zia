#include "Hooker.hpp"

Hooker::Hooker(MethType method, Transaction& trans, std::istream& is, std::ostream& os):
  ZiaThread(false),
  meth(method)
{
  this->SetRelease(true);
}

void	Hooker::Run(void)
{
  std::cout << "Running" << std::endl;
}
