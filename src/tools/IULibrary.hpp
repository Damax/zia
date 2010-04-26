#ifndef		__CLIBRARYU_HPP__
#define		__CLIBRARYU_HPP__
# ifndef	_WIN32

# include	"ILibrary.hpp"

# include	<map>
# include	<string>
# include	<stdlib.h>
# include	<dlfcn.h>
# include	<sys/stat.h>

class		IULibrary : public ILibrary
{
 public:
  void*			open(const char* path);
  void*			load(void* handle, const char * path);
  bool			close(void* handle);

  IULibrary();
  IULibrary(const IULibrary&);
  IULibrary&		operator=(const IULibrary&);
  ~IULibrary();

 private:
};

# endif	/*_WIN32*/
#endif	/*__CLIBRARYU_HPP__*/
