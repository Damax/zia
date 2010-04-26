#ifndef		__ILIBRARY_H__
#define		__ILIBRARY_H__

// Symbol NULL
#include	<stdlib.h>
#include	<string>

class		ILibrary
{
 public:
  virtual void*		open(const char* path) = 0;
  virtual void*		load(void* handle, const char* path) = 0;
  virtual bool		close(void* mod) = 0;
  virtual ~ILibrary(){};
  ILibrary(){};
};

#endif		/*__ILIBRARY_H__*/
