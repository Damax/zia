#ifndef _WIN32

#ifndef IUFILESYSTEM_HPP_
# define IUFILESYSTEM_HPP_

# include <unistd.h>
# include <vector>
# include <string>

# include "IAbsFileSystem.hpp"

class	IUFileSystem : public IAbsFileSystem
{
public:
  int	access(const char *pathname, int mode);
  bool	create_dir(const char* path);
  void	directoryList(const std::string& directory_name, std::vector<std::string>& res);
  
};

#endif /* !IUFILESYSTEM_HPP_ */ 

#endif /* _WIN32 */
