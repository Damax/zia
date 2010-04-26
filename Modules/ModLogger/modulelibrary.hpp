#ifndef MODULELIBRARY_HPP_
# define MODULELIBRARY_HPP_

# ifdef  _WIN32
#  include "IWFileSystem.hpp"
typedef IWFileSystem ZiaFileSystem;
# else
#  include "IUFileSystem.hpp"
typedef IUFileSystem ZiaFileSystem;

# endif /* WIN32 */

#endif /* !MODULELIBRARY_HPP_ */ 
