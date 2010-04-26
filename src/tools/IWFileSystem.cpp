
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include "IWFileSystem.hpp"

int	IWFileSystem::access(const char* pathname, int mode)
{
	if (mode & R_OK)
	{
	    std::ifstream	file;
		file.open(pathname, std::ios_base::in);
		if (file.is_open() == false)
			return 1;
		file.close();
	}
	if (mode & W_OK)
	{
		std::ofstream	file;
		file.open(pathname, std::ios::out);
		if (file.is_open() == false)
			return 1;
		file << "mode & R_OK4" << std::endl;
		file.close();

	}
	if (mode & X_OK)
	{
		std::cerr << "Windows' Zia version don't support fully FileSystem::access (X_OK) yet" << std::endl;
		return 0;
	}
	if (mode & F_OK)
	{
		std::cerr << "Zia's Windows version don't support fully FileSystem::access (F_OK) feature yet" << std::endl;
		return 0;
	}
	return 0;
}


bool	IWFileSystem::create_dir(const char* path)
{
  if (CreateDirectory(reinterpret_cast<LPCWSTR>(path), NULL) == false)
    return true;
  return false;
}


void	IWFileSystem::directoryList(const std::string& directory_name, std::vector<std::string>& res)
{
  WIN32_FIND_DATA File;
  HANDLE hSearch;

  hSearch = FindFirstFile(reinterpret_cast<LPCWSTR>("*.*"), &File);
  if (hSearch != INVALID_HANDLE_VALUE)
    {
      do
  	  {
	    if (File.cFileName[0] == '.')
	      continue;
	    res.push_back(reinterpret_cast<char*>(File.cFileName));
      }
      while (FindNextFile(hSearch, &File));
      FindClose(hSearch);
    }
}