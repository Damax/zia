#ifndef		__CLIBRARYW_HPP__
#define		__CLIBRARYW_HPP__
#ifdef		_WIN32


#include	<map>
#include	<string>
#include	<stdlib.h>
#include	"ILibrary.hpp"

class		IWLibrary : public ILibrary
{
public:
  void*			open(const char* path);
  void*			load(void* handle, const char * path);
  bool			close(void* handle);
  bool			create_dir(const char* path);

  IWLibrary();
  ~IWLibrary();
  IWLibrary(const IWLibrary&);
  IWLibrary&		operator=(const IWLibrary&);
};

#endif	/*_WIN32*/
#endif	/*__CLIBRARYU_HPP__*/
