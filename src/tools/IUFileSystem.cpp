#ifndef _WIN32

# include <sys/stat.h>
# include <iostream>
# include <vector>
# include <dirent.h>
# include <unistd.h>

# include "IUFileSystem.hpp"

int	IUFileSystem::access(const char* pathname, int mode)
{
  return ::access(pathname, mode);
}

bool	IUFileSystem::create_dir(const char* path)
{
  if (mkdir (path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0)
    return true;
  return false;
}

void	IUFileSystem::directoryList(const std::string& directory_name, std::vector<std::string>& res)
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

#endif // _WIN32
