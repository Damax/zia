
#include <iostream>
#include <vector>

#ifdef _WIN32
# include <stdlib.h>
# include <windows.h>
#else 
# include <dirent.h>
#endif // !_WIN32


#ifdef WIN32


// TODO: NOT USE, SHOULD BE REMOVE

void		directoryList2(const std::string& directory_name, std::vector<std::string>& res)
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

#else

void		directoryList2(const std::string& directory_name, std::vector<std::string>& res)
{
  DIR*	dir = opendir(directory_name.data());
  if (dir == NULL)
    return ;
  struct dirent *file;
  while ((file = readdir(dir)))
    {
      if (file->d_name[0] == '.')
	continue;
      res.push_back(file->d_name);
    }
  closedir(dir);
}

#endif // !WIN32
