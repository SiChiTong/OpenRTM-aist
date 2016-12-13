// -*- C++ -*-
#ifndef OpenHRPExecutionContext_h
#define OpenHRPExecutionContext_h

#include <rtm/RTC.h>

#include <coil/Task.h>

#include <rtm/Manager.h>
#include <rtm/PeriodicExecutionContext.h>

// for Windows DLL export 
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#   define DLL_EXPORT __declspec(dllexport)
#else 
#	define DLL_EXPORT 
#endif /* Windows */

#ifdef WIN32
#pragma warning( disable : 4290 )
#endif

namespace RTC
{
  class OpenHRPExecutionContext
    : public virtual PeriodicExecutionContext
  {
  public:
    OpenHRPExecutionContext();
    virtual ~OpenHRPExecutionContext(void);
    virtual void tick(void)
      throw (CORBA::SystemException);
    virtual int svc(void);

  private:
  };  // class OpenHRPExecutionContext
};  // namespace RTC

#ifdef WIN32
#pragma warning( default : 4290 )
#endif


extern "C"
{
  DLL_EXPORT void OpenHRPExecutionContextInit(RTC::Manager* manager);
};

#endif // OpenHRPExecutionContext_h
