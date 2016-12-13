// -*- C++ -*-
/*!
 * @file  InPortConsumer.h
 * @brief InPortConsumer class
 * @date  $Date: 2007-12-31 03:08:03 $
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

#ifndef InPortConsumer_h
#define InPortConsumer_h

#include <coil/Factory.h>
#include <rtm/DataPortStatus.h>

namespace coil
{
  class Properties;
};

namespace SDOPackage
{
  class NVList;
};

class cdrMemoryStream;

namespace RTC
{
  /*!
   * @if jp
   *
   * @class InPortConsumer
   *
   * @brief InPortConsumer ��ݥ��饹
   *
   * ���ϥݡ��ȥ��󥷥塼�ޤΤ������ݥ��󥿡��ե��������饹
   * �ƶ�ݥ��饹�ϡ��ʲ��ν�貾�۴ؿ��μ������󶡤��ʤ���Фʤ�ʤ���
   * - push(): �ǡ�������
   * - clone(): �ݡ��ȤΥ��ԡ�
   * - subscribeInterface(): �ǡ����������Τؤ���Ͽ
   * - unsubscribeInterface(): �ǡ����������Τ���Ͽ���
   *
   * @since 0.4.0
   *
   * @else
   * @class InPortConsumer
   *
   * @brief InPortConsumer abstract class
   *
   * This is the abstract interface class for the input port Consumer.
   * Concrete classes must implement the following pure virtual functions.
   * - push(): Send data
   * - clone(): Copy ports
   * - subscribeInterface(): Subscribe the data send notification
   * - unsubscribeInterface(): Unsubscribe the data send notification
   *
   * @since 0.4.0
   *
   * @endif
   *
   */
  class InPortConsumer
    : public DataPortStatus
  {
  public:
    DATAPORTSTATUS_ENUM
    /*!
     * @if jp
     * @brief ���󥹥ȥ饯��
     *
     * ���󥹥ȥ饯��
     *
     * @else
     * @brief Constructor
     *
     * Constructor
     *
     * @endif
     */
    //InPortConsumer();
    
    /*!
     * @if jp
     * @brief �ǥ��ȥ饯��
     *
     * �ǥ��ȥ饯��
     *
     * @else
     * @brief Destructor
     *
     * Destructor
     *
     * @endif
     */
    virtual ~InPortConsumer(void){};

    /*!
     * @if jp
     * @brief ��������
     *
     * InPortConsumer�γƼ������Ԥ�
     *
     * @else
     * @brief Initializing configuration
     *
     * This operation would be called to configure this consumer
     * in initialization.
     *
     * @endif
     */
    virtual void init(coil::Properties& prop) = 0;

    /*!
     * @if jp
     * @brief ��³��ؤΥǡ�������
     *
     * ��³��Υݡ��Ȥإǡ������������뤿��ν�貾�۴ؿ���
     *
     * @else
     * @brief Send data to the destination port
     *
     * Pure virtual function to send data to the destination port.
     *
     * @endif
     */
    virtual ReturnCode put(const cdrMemoryStream& data) = 0;

    /*!
     * @if jp
     * @brief InterfaceProfile������������
     *
     * InterfaceProfile�����������롣
     * �����ǻ��ꤹ��ץ��ѥƥ�������� NameValue ���֥������Ȥ�
     * dataport.interface_type �ͤ�Ĵ�١������ݡ��Ȥ����ꤵ��Ƥ���
     * ���󥿡��ե����������פȰ��פ�����Τ߾����������롣
     *
     * @param properties InterfaceProfile�����������ץ��ѥƥ�
     *
     * @else
     * @brief Publish InterfaceProfile information
     *
     * Publish interfaceProfile information.
     * Check the dataport.interface_type value of the NameValue object 
     * specified by an argument in property information and get information
     * only when the interface type of the specified port is matched.
     *
     * @param properties Properties to get InterfaceProfile information
     *
     * @endif
     */
    virtual void publishInterfaceProfile(SDOPackage::NVList& properties) = 0;

    /*!
     * @if jp
     * @brief �ǡ����������μ������ؤ���Ͽ
     *
     * ���ꤵ�줿�ץ��ѥƥ������Ƥ˴�Ť��ơ��ǡ����������Τμ���������Ͽ����
     * ����ν�貾�۴ؿ���
     *
     * @param properties ��Ͽ���˻��Ȥ���ץ��ѥƥ�
     *
     * @return ��Ͽ�������
     *
     * @else
     * @brief Subscribe the data send notification
     *
     * Pure virtual function to subscribe the data send notification
     * based on specified property information.
     *
     * @param properties Properties for reference when subscribing
     *
     * @return Subscription result
     *
     * @endif
     */
    virtual bool subscribeInterface(const SDOPackage::NVList& properties) = 0;
    
    /*!
     * @if jp
     * @brief �ǡ����������μ�����꤫�����Ͽ���
     *
     * �ǡ����������Τμ�����꤫����Ͽ������뤿��ν�貾�۴ؿ���
     *
     * @param properties ��Ͽ������˻��Ȥ���ץ��ѥƥ�
     *
     * @else
     * @brief Unsubscribe the data send notification
     *
     * Pure virtual function to unsubscribe the data send notification.
     *
     * @param properties Properties for reference when unsubscribing
     *
     * @endif
     */
    virtual void unsubscribeInterface(const SDOPackage::NVList& properties) = 0;
    
    /*!
     * @if jp
     * @brief ���󥿡��ե������ץ��ե������������뤿�Υե��󥯥�
     * @else
     * @brief Functor to publish interface profile
     * @endif
     */
    struct publishInterfaceProfileFunc
    {
      publishInterfaceProfileFunc(SDOPackage::NVList& prop) : m_prop(prop) {}
      void operator()(InPortConsumer* consumer)
      {
        consumer->publishInterfaceProfile(m_prop);
      }
      SDOPackage::NVList& m_prop;
    };

    /*!
     * @if jp
     * @brief ���󥿡��ե������ץ��ե������������뤿�Υե��󥯥�
     * @else
     * @brief Functor to publish interface profile
     * @endif
     */
    struct subscribeInterfaceFunc
    {
      subscribeInterfaceFunc(SDOPackage::NVList& prop) : m_prop(prop) {}
      bool operator()(InPortConsumer* consumer)
      {
        return consumer->subscribeInterface(m_prop);
      }
      SDOPackage::NVList& m_prop;
    };

  
  };

  typedef ::coil::GlobalFactory<InPortConsumer> InPortConsumerFactory;
};     // namespace RTC

template class ::coil::GlobalFactory<RTC::InPortConsumer>;

#endif // InPortConsumer_h