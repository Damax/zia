#ifdef _WIN32

#ifndef IWFILESYSTEM_HPP_
# define IWFILESYSTEM_HPP_

# include <vector>
# include <string>

# include "IAbsFileSystem.hpp"

enum {
	R_OK = 0x02,
	W_OK = 0x04,
	X_OK = 0x06,
	F_OK = 0x08
};



class	IWFileSystem : public IAbsFileSystem
{
 public:
  int   access(const char *pathname, int mode);
  bool	create_dir(const char* path);
  void	directoryList(const std::string& directory_name, std::vector<std::string>& res);
};


#endif /* !IWFILESYSTEM_HPP_ */ 

#endif /* !_WIN32 */
