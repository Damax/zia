#ifdef _WIN32
#ifndef _IWThread_H_
#define _IWThread_H_

#include "IAbsThread.h"
#include <windows.h>

typedef unsigned threadfunc_t;
typedef void * threadparam_t;

#define STDPREFIX __stdcall

class IWThread : public IAbsThread
{
 public:
  IWThread(bool release = true);
  ~IWThread();

  static threadfunc_t STDPREFIX StartThread(threadparam_t);

  virtual void Run() = 0;

  HANDLE GetThread() { return m_thread; }
  unsigned GetThreadId() { return m_dwThreadId; }

  bool IsRunning();
  void SetRunning(bool x);
  bool IsReleased();
  void SetRelease(bool x);
  bool DeleteOnExit();
  void SetDeleteOnExit(bool x = true);
  bool IsDestructor();

 protected:
  HANDLE m_thread;
  unsigned m_dwThreadId;

 private:
  IWThread(const IWThread& ) {}
  IWThread& operator=(const IWThread& ) { return *this; }
  bool m_running;
  bool m_release;
  bool m_b_delete_on_exit;
  bool m_b_destructor;
};

#endif // _IWThread_H_
#endif // _WIN32


