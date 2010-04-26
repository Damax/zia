#ifndef __ZIA_SOCKET__
#define __ZIA_SOCKET__

#ifdef	_WIN32
# include "IWSocket.hpp"
typedef IWSocket ZiaSocket;
#else
# include "IUSocket.hpp"
typedef IUSocket ZiaSocket;
#endif /* WIN32 */

#endif
