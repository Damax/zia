#ifndef		_WIN32
#include    "IULibrary.hpp"

#include    <string>
#include    <cstring>
#include    <unistd.h>
#include    <errno.h>
#include    <iostream>


void*		IULibrary::open(const char* path)
{
  void* mod = dlopen(path, RTLD_LAZY);

  if (!mod)
    {
      std::cerr << "Cannot open library: " << dlerror() << std::endl;
      return NULL;
    }
  return (mod);
}

void*		IULibrary::load(void *handle, const char* path)
{
  dlerror();
  void* mod = (void*) dlsym(handle, path);
  const char *dlsym_error = dlerror();
  if (dlsym_error)
    {
      std::cerr << "Cannot load symbol 'create': " << dlsym_error << std::endl;
      return NULL;
    }
  return (mod);
}

bool		IULibrary::close(void *mod)
{
  if (dlclose(mod) == 0)
    return true;
  return false;
}

IULibrary::IULibrary()
{
}

IULibrary::IULibrary(const IULibrary&)
{
}

IULibrary&		IULibrary::operator=(const IULibrary&)
{
  return *this;
}

IULibrary::~IULibrary()
{
}

#endif
