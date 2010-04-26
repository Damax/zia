#ifndef ZIAFILESYSTEM_HPP_
# define ZIAFILESYSTEM_HPP_

# ifdef _WIN32
#  include "IWFileSystem.hpp"
   typedef IWFileSystem	ZiaFileSystem;
# else
#  include "IUFileSystem.hpp"
   typedef IUFileSystem	ZiaFileSystem;
# endif // !_WIN32

#endif /* !ZIAFILESYSTEM_HPP_ */ 
