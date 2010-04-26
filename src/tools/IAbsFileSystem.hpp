#ifndef IABSFILESYSTEM_HPP_
# define IABSFILESYSTEM_HPP_

# include <vector>
# include <string>

class	IAbsFileSystem
{
public:
  virtual int	access(const char *pathname, int mode) = 0;
  virtual bool	create_dir(const char* path) = 0;
  virtual void	directoryList(const std::string& directory_name, std::vector<std::string>& res) = 0;
};

#endif /* !IABSFILESYSTEM_HPP_ */ 
