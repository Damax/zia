#ifndef __ZIALIBRARY_HPP__
#define __ZIALIBRARY_HPP__

#ifdef	_WIN32
# include "IWLibrary.hpp"
typedef IWLibrary ZiaLibrary;
#else
# include "IULibrary.hpp"
typedef IULibrary ZiaLibrary;
#endif /* WIN32 */

#endif // !__ZIALIBRARY_HPP__
