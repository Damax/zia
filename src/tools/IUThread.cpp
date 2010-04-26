#ifndef _WIN32

#include "IUThread.hpp"

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

IUThread::IUThread(bool release)
  :m_thread(0)
  ,m_running(true)
  ,m_release(false)
  ,m_b_delete_on_exit(false)
  ,m_b_destructor(false)
{
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
  if (pthread_create(&m_thread,&attr, StartThread,this) == -1)
    {
      perror("Thread: create failed");
      SetRunning(false);
    }
  m_release = release;
}

IUThread::~IUThread()
{
  m_b_destructor = true;
  if (m_running)
    {
      SetRelease(true);
      SetRunning(false);
    }
}

threadfunc_t STDPREFIX IUThread::StartThread(threadparam_t zz)
{
  IUThread *p = (IUThread *)zz;

  while (p -> m_running && !p -> m_release)
    {
      usleep(5000);
    }
  if (p -> m_running)
    {
      p -> Run();
    }
  p -> SetRunning(false);
  if (p -> DeleteOnExit() && !p -> IsDestructor())
    {
      delete p;
    }
  return (threadfunc_t)NULL;
}

bool IUThread::IsRunning()
{
  return m_running;
}

void IUThread::SetRunning(bool x)
{
  m_running = x;
}

bool IUThread::IsReleased()
{
  return m_release;
}

void IUThread::SetRelease(bool x)
{
  m_release = x;
}

bool IUThread::DeleteOnExit()
{
  return m_b_delete_on_exit;
}

void IUThread::SetDeleteOnExit(bool x)
{
	m_b_delete_on_exit = x;
}

bool IUThread::IsDestructor()
{
  return m_b_destructor;
}

#endif
