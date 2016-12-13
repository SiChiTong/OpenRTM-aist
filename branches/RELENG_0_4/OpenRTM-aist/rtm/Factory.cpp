// -*- C++ -*-
/*!
 * @file Factory.h
 * @brief RT component manager class
 * @date $Date: 2007-12-31 03:08:03 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2003-2008
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

#include <rtm/Factory.h>

namespace RTC 
{
  
  /*!
   * @if jp
   * @brief ���󥹥ȥ饯��
   * @else
   * @brief Constructor
   * @endif
   */
  FactoryBase::FactoryBase(const Properties& profile)
    : m_Profile(profile), m_Number(-1)
  {
  }
  
  /*!
   * @if jp
   * @brief �ǥ��ȥ饯��
   * @else
   * @brief Destructor
   * @endif
   */
  FactoryBase::~FactoryBase()
  {
  }
  
  /*!
   * @if jp
   * @brief ����ݡ��ͥ�ȥץ��ե�����μ���
   * @else
   * @brief Get the component profile
   * @endif
   */
  Properties& FactoryBase::profile()
  {
    return m_Profile;
  }
  
  /*!
   * @if jp
   * @brief ���ߤΥ��󥹥��󥹿��μ���
   * @else
   * @brief Get the number of current instances
   * @endif
   */
  int FactoryBase::number()
  {
    return m_Number;
  }
  
  /*!
   * @if jp
   * @brief ���󥹥ȥ饯��
   * @else
   * @brief Constructor
   * @endif
   */
  FactoryCXX::FactoryCXX(const Properties& profile,
			 RtcNewFunc new_func,
			 RtcDeleteFunc delete_func,
			 NumberingPolicy* policy)
    : FactoryBase(profile),
      m_New(new_func),
      m_Delete(delete_func),
      m_policy(policy)
  {
  }
  
  /*!
   * @if jp
   * @brief ����ݡ��ͥ�Ȥ�����
   * @else
   * @brief Create RT-Components
   * @endif
   */
  RtcBase* FactoryCXX::create(Manager* mgr)
  {
    try
      {
	RtcBase* rtobj(m_New(mgr));
	if (rtobj == 0) return NULL;
	
	++m_Number;
	rtobj->setProperties(this->profile());
	
	// create instance_name
	std::string instance_name(rtobj->getTypeName());
	instance_name.append(m_policy->onCreate(rtobj));
	rtobj->setInstanceName(instance_name.c_str());
	
	return rtobj;
      }
    catch (...)
      {
	return NULL;
      }
  }
  
  /*!
   * @if jp
   * @brief ����ݡ��ͥ�Ȥ��˴�
   * @else
   * @brief Destroy RT-Components
   * @endif
   */
  void FactoryCXX::destroy(RtcBase* comp)
  {
    --m_Number;
    m_policy->onDelete(comp);
    m_Delete(comp);
  }
};