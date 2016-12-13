// -*- C++ -*-
/*!
 * @file ExtTrigExecutionContext.cpp
 * @brief ExtTrigExecutionContext class
 * @date $Date: 2007-04-13 16:08:18 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2007
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id: ExtTrigExecutionContext.cpp,v 1.1 2007-04-13 16:08:18 n-ando Exp $
 *
 */

/*
 * $Log: not supported by cvs2svn $
 */

#include <rtm/ExtTrigExecutionContext.h>
#include <rtm/ECFactory.h>

namespace RTC
{
  ExtTrigExecutionContext::ExtTrigExecutionContext()
    : PeriodicExecutionContext()
  {
  }

  ExtTrigExecutionContext::~ExtTrigExecutionContext()
  {
  }

  void ExtTrigExecutionContext::tick()
  {
    std::cout << "tick!!!" << std::endl;
    if (m_worker._mutex.tryacquire() != 0)
      {
	return;
      }
    m_worker._called = true;
    m_worker._cond.signal();
    m_worker._mutex.release();
    return;
  }

  int ExtTrigExecutionContext::svc(void)
  {
    do
      {
	ACE_Time_Value tv(0, m_usec); // (s, us)

	m_worker._mutex.acquire();
	while (!m_worker._called && m_running)
	  {
	    m_worker._cond.wait();
	  }
	if (m_worker._called)
	  {
	    m_worker._called = false;
	    std::for_each(m_comps.begin(), m_comps.end(), invoke_worker());
	    while (!m_running) {ACE_OS::sleep(tv);}
	    ACE_OS::sleep(tv);
	  }
	m_worker._mutex.release();
      } while (m_running);

    return 0;
  }
};


extern "C"
{
  void ExtTrigExecutionContextInit(RTC::Manager* manager)
  {
    manager->registerECFactory("ExtTrigExecutionContext",
			       RTC::ECCreate<RTC::ExtTrigExecutionContext>,
			       RTC::ECDelete<RTC::ExtTrigExecutionContext>);
    
  }
};