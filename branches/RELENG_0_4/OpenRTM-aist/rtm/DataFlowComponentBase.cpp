// -*- C++ -*-
/*!
 * @file DataFlowComponentBase.cpp
 * @brief DataFlowParticipant RT-Component base class
 * @date $Date: 2007-04-13 15:44:03 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2006
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id$
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