// -*- C++ -*-
/*!
 * @file DataFlowComponentBase.cpp
 * @brief DataFlowParticipant RT-Component base class
 * @date $Date: 2007-01-21 14:02:37 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2006
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id: DataFlowComponentBase.cpp,v 1.3 2007-01-21 14:02:37 n-ando Exp $
 *
 */

/*
 * $Log: not supported by cvs2svn $
 * Revision 1.2  2007/01/14 19:39:04  n-ando
 * The debugging messages to stdout were deleted.
 *
 * Revision 1.1  2007/01/09 09:47:15  n-ando
 * DataFlowParticipant RT-Component base class
 *
 */

#include <rtm/DataFlowComponentBase.h>
#include <rtm/Manager.h>
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
  DataFlowComponentBase::DataFlowComponentBase(Manager* manager)
    : RTObject_impl(manager)
  {
    m_ref = this->_this();
    m_objref = m_ref;
    
    m_pec = new PeriodicExecutionContext(m_ref, 1000);
    m_pec->add(m_ref);
  }


  /*!
   * @if jp
   * @brief �ǥ��ȥ饯��
   * @else
   * @brief Destructor
   * @endif
   */
  DataFlowComponentBase::~DataFlowComponentBase()
  {
  }


  /*!
   * @if jp
   * @brief �����
   * @else
   * @brief Initialization
   * @endif
   */
  void DataFlowComponentBase::init()
  {
  }


}; // namespace RTC