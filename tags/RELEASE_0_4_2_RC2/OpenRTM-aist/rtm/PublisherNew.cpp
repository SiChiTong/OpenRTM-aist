// -*- C++ -*-
/*!
 * @file  PublisherNew.cpp
 * @brief PublisherNew class
 * @date  $Date: 2007-12-31 03:08:06 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2006-2008
 *     Noriaki Ando
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id$
 *
 */

#include <rtm/PublisherNew.h>
#include <rtm/InPortConsumer.h>
#include <rtm/Properties.h>
#include <rtm/RTC.h>
#include <iostream>


namespace RTC
{
  /*!
   * @if jp
   * @brief ���󥹥ȥ饯��
   * @else
   * @brief Constructor
   * @endif
   */
  PublisherNew::PublisherNew(InPortConsumer* consumer,
			     const Properties& property)
    : m_consumer(consumer), m_running(true), m_data()
  {
    open(0);
  }

  /*!
   * @if jp
   * @brief �ǥ��ȥ饯��
   * @else
   * @brief Destructor
   * @endif
   */
  PublisherNew::~PublisherNew()
  {
    delete m_consumer;
  }
  
  /*!
   * @if jp
   * @brief Observer�ؿ�
   * @else
   * @brief Observer function
   * @endif
   */
  void PublisherNew::update()
  {
    if (m_data._mutex.tryacquire() != 0)
      {
	return;
      }
    m_data._updated = true;
    m_data._cond.signal();
    m_data._mutex.release();
#ifdef WIN32
    ACE_OS::thr_yield();
#else
    pthread_yield();
#endif
    return;
  }
  
  /*!
   * @if jp
   * @brief PublisherNew::����åɼ¹Դؿ�
   * @else
   * @brief Thread execution function
   * @endif
   */
  int PublisherNew::svc(void)
  {
    while (m_running)
      {
	m_data._mutex.acquire();
	
	// Waiting for new data updated
	while (!m_data._updated && m_running)
	  {
	    m_data._cond.wait();
	  }
	
	if (m_data._updated)
	  {
	    m_consumer->push(); 
	    m_data._updated = false;
	  }
	
	m_data._mutex.release();	
      }
    return 0;
  }

  
  /*!
   * @if jp
   * @brief ����������
   * @else
   * @brief Task start function
   * @endif
   */
  int PublisherNew::open(void *args)
  {
    m_running = true;
    this->activate();
    return 0;
  }
  
  
  /*!
   * @if jp
   * @brief ��������λ�ؿ�
   * @else
   * @brief Task terminate function
   * @endif
   */
  void PublisherNew::release()
  {
    if (m_data._mutex.acquire() != 0)
      {
	return;
      }
    m_running = false;
    m_data._cond.signal(); //broadcast();
    m_data._mutex.release();
    wait();
  }
  
}; // namespace RTC