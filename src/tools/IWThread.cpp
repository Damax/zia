#ifdef _WIN32

#include <stdio.h>
#include <process.h>

#include "IWThread.hpp"

IWThread::IWThread(bool release)
 :m_thread(0)
 ,m_running(true)
 ,m_release(false)
 ,m_b_delete_on_exit(false)
 ,m_b_destructor(false)
{
  //	m_thread = ::CreateThread(NULL, 0, StartThread, this, 0, &m_dwThreadId);
  m_thread = (HANDLE)_beginthreadex(NULL, 0, &StartThread, this, 0, &m_dwThreadId);
  m_release = release;
}


IWThread::~IWThread()
{
  m_b_destructor = true;
  if (m_running)
    {
      SetRelease(true);
      SetRunning(false);
      Sleep(1000);
    }
  if (m_thread)
    ::CloseHandle(m_thread);
}


threadfunc_t STDPREFIX IWThread::StartThread(threadparam_t zz)
{
  IWThread *p = (IWThread *)zz;

  while (p -> m_running && !p -> m_release)
    {
      Sleep(1000);
    }
  if (p -> m_running)
    {
      p -> Run();
    }
  p -> SetRunning(false); // if return
  if (p -> DeleteOnExit() && !p -> IsDestructor())
    {
      delete p;
    }
  _endthreadex(0);
  return (threadfunc_t)NULL;
}


bool IWThread::IsRunning()
{
  return m_running;
}


void IWThread::SetRunning(bool x)
{
  m_running = x;
}


bool IWThread::IsReleased()
{
  return m_release;
}


void IWThread::SetRelease(bool x)
{
  m_release = x;
}


bool IWThread::DeleteOnExit()
{
  return m_b_delete_on_exit;
}


void IWThread::SetDeleteOnExit(bool x)
{
  m_b_delete_on_exit = x;
}


bool IWThread::IsDestructor()
{
  return m_b_destructor;
}

#endif
