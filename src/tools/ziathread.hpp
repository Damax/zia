#ifndef __ZIA_THREAD__
#define __ZIA_THREAD__

#ifdef	_WIN32
# include "IWThread.hpp"
typedef IWThread ZiaThread;
#else
# include "IUThread.hpp"
typedef IUThread ZiaThread;
#endif /* WIN32 */

#endif
