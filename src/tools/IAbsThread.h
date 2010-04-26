#ifndef _IAbsThread_H
#define _IAbsThread_H

#ifdef _WIN32
# include <windows.h>
 typedef HANDLE IThread;
 typedef unsigned threadfunc_t;
 typedef void * threadparam_t;
# define STDPREFIX __stdcall
#else // POSIX
# include <pthread.h>
 typedef pthread_t IThread;
 typedef void * threadfunc_t;
 typedef void * threadparam_t;
# define STDPREFIX
#endif // WIN32

class IAbsThread
{
 public:
  virtual ~IAbsThread(){};

  static threadfunc_t STDPREFIX StartThread(threadparam_t);

  virtual void Run() = 0;

  virtual IThread GetThread() = 0;
  virtual bool IsRunning() = 0;
  virtual void SetRunning(bool x) = 0;
  virtual bool IsReleased() = 0;
  virtual void SetRelease(bool x) = 0;
  virtual bool DeleteOnExit() = 0;
  virtual void SetDeleteOnExit(bool x = true) = 0;
  virtual bool IsDestructor() = 0;
};

#endif // _IAbsThread_H
