// -*- C++ -*-
/*!
 * @file CorbaConsumer.h
 * @brief CORBA Consumer class
 * @date $Date: 2007-12-31 03:08:02 $
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

#ifndef Consumer_h
#define Consumer_h
#include <iostream>
#ifdef ORB_IS_MICO
#include <CORBA.h>
#endif
#ifdef ORB_IS_OMNIORB
#include <omniORB4/CORBA.h>
#endif
#ifdef ORB_IS_ORBACUS
#include <OB/CORBA.h>
#endif
#ifdef ORB_IS_ORBIT2
#include <orbitcpp/orb-cpp/orbitcpp.h>
#endif
#ifdef ORB_IS_ORBIX
#include <CORBA.h>
#endif
#ifdef ORB_IS_TAO
#include <tao/corba.h>
#endif

/*!
 * @if jp
 * @namespace RTC
 *
 * @brief RT����ݡ��ͥ��
 *
 * @else
 *
 * @namespace RTC
 *
 * @brief RT-Component
 *
 * @endif
 */
namespace RTC
{
  /*!
   * @if jp
   * @class CorbaConsumerBase
   *
   * @brief ���֥������ȥ�ե���󥹤��ݻ�����ץ졼���ۥ�����쥯�饹
   *
   * �̿����ʤȤ��� CORBA �����򤷤����Υ��󥷥塼�޼����Τ���δ��쥯�饹
   *
   * @since 0.4.0
   *
   * @else
   *
   * @class ConsumerBase
   *
   * @brief Placeholder base class to hold remote object reference.
   *
   * A base class for consumer implementation when chose CORBA 
   * as a communication tool.
   *
   * @since 0.4.0
   *
   * @endif
   */
  class CorbaConsumerBase
  {
  public:
    /*!
     * @if jp
     *
     * @brief ���󥹥ȥ饯��
     *
     * @else
     *
     * @brief Consructor
     *
     * @endif
     */
    CorbaConsumerBase(){};
    
    /*!
     * @if jp
     *
     * @brief ���ԡ����󥹥ȥ饯��
     *
     * @param x ���ԡ�����CorbaConsumerBase���֥�������
     *
     * @else
     *
     * @brief Copy Consructor
     *
     * @param x A CorbaConsumerBase object of copy source
     *
     * @endif
     */
    CorbaConsumerBase(const CorbaConsumerBase& x)
    {
      m_objref = x.m_objref;
    }
    
    /*!
     * @if jp
     *
     * @brief �����黻��
     *
     * @param x ������
     *
     * @return �������
     *
     * @else
     *
     * @brief Assignment operator
     *
     * @param x Copy source.
     *
     * @return An assignment result
     *
     * @endif
     */
    CorbaConsumerBase& operator=(const CorbaConsumerBase& x)
    {
      m_objref = x.m_objref;
      return *this;
    }
    
    /*!
     * @if jp
     *
     * @brief ���ۥǥ��ȥ饯��
     * 
     * @else
     * 
     * @brief Virtual destructor
     * 
     * @endif
     */
    virtual ~CorbaConsumerBase(){};
    
    /*!
     * @if jp
     *
     * @brief CORBA���֥������Ȥ򥻥åȤ���
     *
     * Ϳ����줿���֥������ȥ�ե���󥹤ϡ�ConsumerBase ���֥����������
     * CORBA::Object_var ���Ȥ����ݻ�����롣 
     *
     * @param obj CORBA ���֥������ȤΥ�ե����
     *
     * @return obj �� nil ��ե���󥹤ξ�� false ���֤���
     *
     * @else
     *
     * @brief Set CORBA Object
     *
     * The given CORBA Object is held as CORBA::Object_var type in ConsumerBase
     * object.
     *
     * @param obj Object reference of CORBA object
     *
     * @return If obj is nil reference, it returns false.
     *
     * @endif
     */
    virtual bool setObject(CORBA::Object_ptr obj)
    {
      if (CORBA::is_nil(obj))
	{
	  return false;
	}
      m_objref = CORBA::Object::_duplicate(obj);
      return true;
    }
    
    /*!
     * @if jp
     *
     * @brief CORBA���֥������Ȥ��������
     *
     * ConsumerBase ���֥���������� CORBA::Object_var ���Ȥ����ݻ�����Ƥ���
     * ���֥������ȥ�ե���󥹤�������롣 
     *
     * @return obj CORBA ���֥������ȤΥ�ե����
     *
     * @else
     *
     * @brief Get CORBA Object
     *
     * Get the object reference held as CORBA::Object_var type in ConsumerBase
     * object.
     *
     * @return Object reference of CORBA object
     *
     * @endif
     */
    virtual CORBA::Object_ptr getObject()
    {
      return m_objref;
    }
    
    /*!
     * @if jp
     *
     * @brief CORBA���֥������Ȥ�����򥯥ꥢ����
     *
     * ���ꤵ��Ƥ��� CORBA ���֥������Ȥ򥯥ꥢ���롣
     * CORBA���֥������Ȥ��Τ�Τ��Ф��Ƥϲ������ʤ���
     *
     * @else
     *
     * @brief Clear CORBA object setting
     *
     * Clear CORBA object which is set.
     * Operate nothing for CORBA object itself.
     *
     * @endif
     */
    virtual void releaseObject()
    {
      m_objref = CORBA::Object::_nil();
    }
    
  protected:
    /*!
     * @if jp
     * @brief ���ꤵ�줿 CORBA ���֥�������
     * @else
     * @brief CORBA object which is set.
     * @endif
     */
    CORBA::Object_var m_objref;
  };
  
  /*!
   * @if jp
   *
   * @class CorbaConsumer
   * @brief ���֥������ȥ�ե���󥹤��ݻ�����ץ졼���ۥ���ƥ�ץ졼�ȥ��饹
   * 
   * �ƥ�ץ졼�Ȱ�����Ϳ����줿����CORBA���֥������Ȥ��ݻ����롣
   * ���֥������Ȥ����åȤ��줿�Ȥ��ˡ�Ϳ����줿���� narrow �����Τǡ�
   * _ptr() �Ǽ��������ե���󥹤ϡ�narrow �ѤߤΥ�ե���󥹤Ǥ��롣
   * ����Ū�ʻ��ѤΤ���ˡ�_ptr ��, _var����Ʊ���˥ƥ�ץ졼�Ȱ����Ȥ���
   * Ϳ����ɬ�פ����롣(�������ջ��໲��)
   * <br>  
   * ����: ObjectTypePtr = ObjectType::_ptr_type �Ȥ��Ƥ��뤫��
   *       _ptr_type ��ɸ��Ǥϵ��ꤵ��Ƥ��ʤ���
   *       ��������omniORB, TAO, MICO �Ǥϡ��ץ��������饹�����ǡ�
   *       Type_ptr ���� _ptr_type �� typedef ���Ƥ���Τǡ�
   *       �ƥ�ץ졼�Ȥ���2��������ꤷ�ʤ��Ƥ⥳��ѥ�����̤롣
   *
   * @param ObjectType ���Υۥ�����ݻ����륪�֥������Ȥη�
   * @param ObjectTypePtr ���Υۥ�����ݻ����� _ptr ��
   * @param ObjectTypeVar ���Υۥ�����ݻ����� _var ��
   *
   * @since 0.4.0
   *
   * @else
   *
   * @class Consumer
   * @brief Placeholder template class to hold remote object reference.
   *
   * This class holds a type of object that given by template parameter.
   * For internal use, _ptr type and _var type should be given as template
   * parameter. (Please refere the following notation.)
   *
   * Note: ObjectTypePtr's default value is defined as ObjectType::_ptr_type,
   *       although _ptr_type is not defined as normative type.
   *       However, omniORB, TAO, MICO, ORBit, ORBacus define _ptr_type and
   *       _var_type as _ptr type and _var type in stub code.
   *       Usually, you don't need to specify 2nd and 3rd template arguments.
   *       
   * @since 0.4.0
   *
   * @endif
   */
  template <class ObjectType,
	    typename ObjectTypePtr = typename ObjectType::_ptr_type,
	    typename ObjectTypeVar = typename ObjectType::_var_type>
  class CorbaConsumer
    : public CorbaConsumerBase
  {
  public:
    /*!
     * @if jp
     *
     * @brief ���󥹥ȥ饯��
     *
     * @else
     *
     * @brief Consructor
     *
     * @endif
     */
    CorbaConsumer(){};
    
    /*!
     * @if jp
     *
     * @brief ���ԡ����󥹥ȥ饯��
     *
     * @param x ���ԡ���
     *
     * @else
     *
     * @brief Copy constructor
     *
     * @param x Copy source.
     *
     * @endif
     */
    CorbaConsumer(const CorbaConsumer& x)
    {
      m_var = x.m_var;
    }
    
    /*!
     * @if jp
     *
     * @brief �����黻��
     *
     * @param x ������
     *
     * @return �������
     *
     * @else
     *
     * @brief Assignment operator
     *
     * @param x Copy source.
     *
     * @return An assignment result
     *
     * @endif
     */
    CorbaConsumer& operator=(const CorbaConsumer& x)
    {
      m_var = x.m_var;
    }
    
    /*!
     * @if jp
     *
     * @brief ���ۥǥ��ȥ饯��
     *
     * @else
     *
     * @brief Virtual destructor
     *
     * @endif
     */
    virtual ~CorbaConsumer()
    {
    };
    
    /*!
     * @if jp
     * @brief ���֥������Ȥ򥻥åȤ���
     *
     * ConsumerBase �Υ����С��饤�ɡ�CORBA::Object_var �˥��֥������Ȥ򥻥å�
     * ����ȤȤ�ˡ�template�ѥ�᡼���η��� narrow �������֥������Ȥ�
     * �����ѿ����ݻ����롣
     *
     * @param obj CORBA Objecct
     *
     * @return ���֥�������������
     *         �����оݥ��֥������Ȥ� null �ξ��� false ���֤äƤ���
     * 
     * @else
     * @brief Set Object
     *
     * Override function of ConsumerBase. This operation set an Object to 
     * CORBA:Object_var in the class, and this object is narrowed to
     * given template parameter and stored in the member variable.
     *
     * @param obj CORBA Objecct
     *
     * @return An object setting result.
     *         If target object is null, it returns false.
     *
     * @endif
     */
    virtual bool setObject(CORBA::Object_ptr obj)
    {
      if (CorbaConsumerBase::setObject(obj))
	{
	  m_var = ObjectType::_duplicate(ObjectType::_narrow(m_objref));
	  if (!CORBA::is_nil(m_var))
	    return true;
	}
      return false; // object is nil
    }
    
    /*!
     * @if jp
     * @brief ObjectType ���Υ��֥������ȤΥ�ե���󥹤����
     *
     * ObjectType �� narrow�ѤߤΥ��֥������ȤΥ�ե���󥹤�������롣
     * ���֥������ȥ�ե���󥹤���Ѥ���ˤϡ�setObject() �ǥ��åȺѤߤ�
     * �ʤ���Фʤ�ʤ���
     * ���֥������Ȥ����åȤ���Ƥ��ʤ���С�nil ���֥������ȥ�ե���󥹤�
     * �֤���롣
     *
     * @return ObjectType �� narrow �ѤߤΥ��֥������ȤΥ�ե����
     * 
     * @else
     * @brief Get Object reference narrowed as ObjectType
     *
     * This operation returns object reference narrowed as ObjectType.
     * To use the returned object reference, reference have to be set by
     * setObject().
     * If object is not set, this operation returns nil object reference.
     *
     * @return The object reference narrowed as ObjectType
     *
     * @endif
     */
    inline ObjectTypePtr _ptr()
    {
      return m_var;
    }
    
    /*!
     * @if jp
     * @brief ObjectType ���Υ��֥������ȤΥ�ե���󥹤����
     *
     * ObjectType �� narrow�ѤߤΥ��֥������ȤΥ�ե���󥹤�������롣
     * ���֥������ȥ�ե���󥹤���Ѥ���ˤϡ�setObject() �ǥ��åȺѤߤ�
     * �ʤ���Фʤ�ʤ���
     * ���֥������Ȥ����åȤ���Ƥ��ʤ���С�nil ���֥������ȥ�ե���󥹤�
     * �֤���롣
     *
     * @return ObjectType �� narrow �ѤߤΥ��֥������ȤΥ�ե����
     * 
     * @else
     * @brief Get Object reference narrowed as ObjectType
     *
     * This operation returns object reference narrowed as ObjectType.
     * To use the returned object reference, reference have to be set by
     * setObject().
     * If object is not set, this operation returns nil object reference.
     *
     * @return The object reference narrowed as ObjectType
     *
     * @endif
     */
    inline ObjectTypePtr operator->()
    {
      return m_var;
    }
    
    /*!
     * @if jp
     *
     * @brief CORBA���֥������Ȥ�����򥯥ꥢ����
     *
     * ���ꤵ��Ƥ��� CORBA ���֥������Ȥ򥯥ꥢ���롣
     * CORBA���֥������Ȥ��Τ�Τ��Ф��Ƥϲ������ʤ���
     *
     * @else
     *
     * @brief Clear CORBA object setting
     *
     * Clear CORBA object which is set.
     * Operate nothing for CORBA object itself.
     *
     * @endif
     */
    virtual void releaseObject()
    {
      CorbaConsumerBase::releaseObject();
      m_var = ObjectType::_nil();
    }
    
  protected:
    /*!
     * @if jp
     * @brief ���ꤵ�줿 CORBA ���֥�������
     * @else
     * @brief CORBA object which has been set.
     * @endif
     */
    ObjectTypeVar m_var;
  };
}; // namespace RTC
#endif // Consumer_h