#ifdef		_WIN32
#include    <string>
#include    <cstring>
#include    <iostream>
#include	<Windows.h>
#include    "IWLibrary.hpp"

void*		IWLibrary::open(const char* path)
{
  void* handle = static_cast<void *> (LoadLibrary((reinterpret_cast<LPCWSTR>(path))));

  if (!handle)
    {
      std::cerr << "Cannot open library: " << path << std::endl;
      return NULL;
    }
  return (handle);
}

void*		IWLibrary::load(void *handle, const char* path)
{
  FARPROC mod = (GetProcAddress(static_cast<HMODULE>(handle), path));
  if (!mod)
    {
      std::cerr << "Cannot load symbol '" << path << "'" << std::endl;
      return NULL;
    }
  return ((void*)mod);
}

bool		IWLibrary::close(void *mod)
{
  if(FreeLibrary(static_cast<HMODULE>(mod)) == 0)
    return true;
  return false;
}

IWLibrary::IWLibrary()
{
}

IWLibrary::IWLibrary(const IWLibrary&)
{
}

IWLibrary&		IWLibrary::operator=(const IWLibrary&)
{
  return *this;
}

IWLibrary::~IWLibrary()
{
}

#endif
