#ifndef _WIN32

#ifndef _CAbsUThread_H_
# define _CAbsUThread_H_
# include <pthread.h>
# include "IAbsThread.h"
 typedef void * threadfunc_t;
 typedef void * threadparam_t;
# define STDPREFIX

class IUThread : IAbsThread
{
 public:
  IUThread(bool release = true);
  virtual ~IUThread();

  static threadfunc_t STDPREFIX StartThread(threadparam_t);

  virtual void Run() = 0;

  pthread_t GetThread() { return m_thread; }

  bool IsRunning();
  void SetRunning(bool x);
  bool IsReleased();
  void SetRelease(bool x);
  bool DeleteOnExit();
  void SetDeleteOnExit(bool x = true);
  bool IsDestructor();

 protected:
  pthread_t m_thread;


 private:
  IUThread(const IUThread& ) {}
  IUThread& operator=(const IUThread& ) { return *this; }
  bool m_running;
  bool m_release;
  bool m_b_delete_on_exit;
  bool m_b_destructor;
};

# endif // _Thread_H
#endif // _WIN32


