// -*- C++ -*-
/*!
 * @file DataOutPort.h
 * @brief Base class of OutPort
 * @date $Date: 2007-04-13 15:45:08 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2006
 *     Noriaki Ando
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id: DataOutPort.h,v 1.7 2007-04-13 15:45:08 n-ando Exp $
 *
 */

/*
 * $Log: not supported by cvs2svn $
 * Revision 1.6  2007/01/21 09:45:31  n-ando
 * To advertise interface profile, publishInterfaceProfile() is called now.
 *
 * Revision 1.5  2007/01/14 22:57:54  n-ando
 * A bug fix about template argument for buffer-type in constructor.
 *
 * Revision 1.4  2007/01/12 14:30:01  n-ando
 * A trivial bug fix.
 *
 * Revision 1.3  2007/01/06 17:44:11  n-ando
 * The behavior on notify_connect() and notify_disconnect() are now
 * implemented in protected functions(ex. publisherInterfaces()).
 *
 * Revision 1.2  2006/12/02 18:29:15  n-ando
 * Now OutPortCorbaProvider and InPortCorbaConsumer are used.
 *
 * Revision 1.1  2006/11/27 09:44:37  n-ando
 * The first commitment.
 *
 *
 */

#ifndef DataOutPort_h
#define DataOutPort_h


#include <rtm/PortBase.h>
#include <rtm/BufferBase.h>
#include <rtm/OutPortCorbaProvider.h>
#include <rtm/InPortCorbaConsumer.h>
#include <rtm/OutPort.h>
#include <rtm/NVUtil.h>
#include <rtm/PublisherFactory.h>

namespace RTC
{
  /*!
   * @if jp
   * @class DataOutPort
   * @brief InPort �� Port
   * @else
   * @class DataOutPort
   * @brief InPort abstruct class
   * @endif
   */
  class DataOutPort
    : public PortBase
  {
  public:
    /*!
     * @if jp
     * @brief ���󥹥ȥ饯��
     * @else
     * @brief Constructor
     * @endif
     */
    template <class DataType, template <class DataType> class Buffer>
    DataOutPort(const char* name, OutPort<DataType, Buffer>& outport)
      : PortBase(name), m_outport(outport)
    {
      // PortProfile::properties ������
      addProperty("port.port_type", "DataOutPort");
      
      m_providers.push_back(new OutPortCorbaProvider<DataType>(outport));
      m_providers.back()->publishInterfaceProfile(m_profile.properties);
      m_consumers.push_back(new InPortCorbaConsumer<DataType>(outport));
    }


    /*!
     * @if jp
     * @brief �ǥ��ȥ饯��
     * @else
     * @brief Destructor
     * @endif
     */
    virtual ~DataOutPort();

    
    /*!
     * @if jp
     * @brief [CORBA interface] Port ����³��Ԥ�
     *
     * OutPort �� InPort �Ȥ���³��Ԥ���
     *
     * OutPort ¦�� connect() �Ǥϰʲ��Υ������󥹤ǽ������Ԥ��롣
     *
     * 1. OutPort �˴�Ϣ���� connector �������������ӥ��å�
     *
     * 2. InPort�˴�Ϣ���� connector ����μ���
     *  - ConnectorProfile::properties["dataport.corba_any.inport_ref"]��
     *    OutPortAny �Υ��֥������ȥ�ե���󥹤����ꤵ��Ƥ����硢
     *    ��ե���󥹤��������Consumer���֥������Ȥ˥��åȤ��롣
     *    ��ե���󥹤����åȤ���Ƥ��ʤ����̵�뤷�Ʒ�³��
     *    (OutPort��connect() �ƤӽФ��Υ���ȥ�ݥ���Ȥξ��ϡ�
     *    InPort�Υ��֥������ȥ�ե���󥹤ϥ��åȤ���Ƥ��ʤ��Ϥ��Ǥ��롣)
     * 3. PortBase::connect() �򥳡���
     *    Port����³�δ��ܽ������Ԥ��롣
     * 4. �嵭2.��InPort�Υ�ե���󥹤������Ǥ��ʤ���С�����InPort��
     *    ��Ϣ���� connector �����������롣
     *
     * 5. ConnectorProfile::properties ��Ϳ����줿���󤫤顢
     *    OutPort¦�ν����������Ԥ���
     *
     * - [dataport.interface_type]
     * -- CORBA_Any �ξ��: 
     *    InPortAny ���̤��ƥǡ����򴹤���롣
     *    ConnectorProfile::properties["dataport.corba_any.inport_ref"]��
     *    InPortAny �Υ��֥������ȥ�ե���󥹤򥻥åȤ��롣
     * -- RawTCP �ξ��: Raw TCP socket ���̤��ƥǡ����򴹤���롣
     *    ConnectorProfile::properties["dataport.raw_tcp.server_addr"]
     *    ��InPort¦�Υ����Х��ɥ쥹�򥻥åȤ��롣
     *
     * - [dataport.dataflow_type]
     * -- Push�ξ��: Subscriber���������롣Subscriber�Υ����פϡ�
     *    dataport.subscription_type �����ꤵ��Ƥ��롣
     * -- Pull�ξ��: InPort¦���ǡ�����Pull���Ǽ������뤿�ᡢ
     *    �ä˲��⤹��ɬ�פ�̵����
     *
     * - [dataport.subscription_type]
     * -- Once�ξ��: SubscriberOnce���������롣
     * -- New�ξ��: SubscriberNew���������롣
     * -- Periodic�ξ��: SubscriberPeriodic���������롣
     *
     * - [dataport.push_interval]
     * -- dataport.subscription_type=Periodic�ξ����������ꤹ�롣
     *
     * 6. �嵭�ν����Τ�����ĤǤ⥨�顼�Ǥ���С����顼�꥿���󤹤롣
     *    ����˽������Ԥ�줿����RTC::RTC_OK�ǥ꥿���󤹤롣
     *  
     * @else
     * @brief [CORBA interface] Connect the Port
     * @endif
     */
    
  protected:
    /*!
     * @if jp
     *
     * @brief Interface ������������
     *
     * ���Υ��ڥ졼�����ϡ�notify_connect() �����������󥹤λϤ�˥�����
     * ������貾�۴ؿ��Ǥ��롣
     * notify_connect() �Ǥϡ�
     *
     * - publishInterfaces()
     * - connectNext()
     * - subscribeInterfaces()
     * - updateConnectorProfile()
     *
     * �ν�� protected �ؿ��������뤵����³�������Ԥ��롣
     * <br>
     * ���Υ��ڥ졼�����ϡ������� connector_id ���Ф��Ƥ���³��������
     * ��¸�� connector_id ���Ф��ƤϹ�����Ŭ�ڤ˹Ԥ���ɬ�פ����롣
     *
     * @param connector_profile ��³�˴ؤ���ץ��ե��������
     * @return ReturnCode_t ���Υ꥿���󥳡���
     *
     * @else
     *
     * @brief Publish interface information
     *
     * This operation is pure virutal method that would be called at the
     * beginning of the notify_connect() process sequence.
     * In the notify_connect(), the following methods would be called in order.
     *
     * - publishInterfaces()
     * - connectNext()
     * - subscribeInterfaces()
     * - updateConnectorProfile() 
     *
     * This operation should create the new connection for the new
     * connector_id, and should update the connection for the existing
     * connection_id.
     *
     * @param connector_profile The connection profile information
     * @return The return code of ReturnCode_t type.
     *
     * @endif
     */
    virtual ReturnCode_t
    publishInterfaces(ConnectorProfile& connector_profile);
    

    /*! @if jp
     *
     * @brief Interface ������������
     *
     * ���Υ��ڥ졼�����ϡ�notify_connect() �����������󥹤���֤˥�����
     * ������貾�۴ؿ��Ǥ��롣
     * notify_connect() �Ǥϡ�
     *
     * - publishInterfaces()
     * - connectNext()
     * - subscribeInterfaces()
     * - updateConnectorProfile()
     *
     * �ν�� protected �ؿ��������뤵����³�������Ԥ��롣
     *
     * @param connector_profile ��³�˴ؤ���ץ��ե��������
     * @return ReturnCode_t ���Υ꥿���󥳡���
     *
     * @else
     *
     * @brief Publish interface information
     *
     * This operation is pure virutal method that would be called at the
     * mid-flow of the notify_connect() process sequence.
     * In the notify_connect(), the following methods would be called in order.
     *
     * - publishInterfaces()
     * - connectNext()
     * - subscribeInterfaces()
     * - updateConnectorProfile()
     *
     * @param connector_profile The connection profile information
     * @return The return code of ReturnCode_t type.
     *
     * @endif
     */
    virtual ReturnCode_t
    subscribeInterfaces(const ConnectorProfile& connector_profile);
    

    /*!
     * @if jp
     *
     * @brief Interface ����³��������
     *
     * ���Υ��ڥ졼�����ϡ�notify_disconnect() �����������󥹤ν����˥�����
     * ������貾�۴ؿ��Ǥ��롣
     * notify_disconnect() �Ǥϡ�
     * - disconnectNext()
     * - unsubscribeInterfaces()
     * - eraseConnectorProfile()
     * �ν�� protected �ؿ��������뤵����³����������Ԥ��롣
     *
     * @param connector_profile ��³�˴ؤ���ץ��ե��������
     *
     * @else
     *
     * @brief Disconnect interface connection
     *
     * This operation is pure virutal method that would be called at the
     * end of the notify_disconnect() process sequence.
     * In the notify_disconnect(), the following methods would be called.
     * - disconnectNext()
     * - unsubscribeInterfaces()
     * - eraseConnectorProfile() 
     *
     * @param connector_profile The connection profile information
     *
     * @endif
     */
    virtual void
    unsubscribeInterfaces(const ConnectorProfile& connector_profile);


    
    struct publish
    {
      publish(SDOPackage::NVList& prop) : m_prop(prop) {}
      void operator()(OutPortProvider* provider)
      {
	provider->publishInterface(m_prop);
      }
      SDOPackage::NVList& m_prop;
    };

    struct unsubscribe
    {
      unsubscribe(const SDOPackage::NVList& prop) : m_prop(prop) {}
      void operator()(InPortConsumer* consumer)
      {
	consumer->unsubscribeInterface(m_prop);
      }
      const SDOPackage::NVList& m_prop;
    };

  private:
    std::vector<OutPortProvider*> m_providers;
    std::vector<InPortConsumer*> m_consumers;
    OutPortBase& m_outport;

    PublisherFactory m_pf;

    struct subscribe
    {
      subscribe(const ConnectorProfile& prof)
	: m_prof(&prof), _consumer(NULL) 
      {
      }

      subscribe(const subscribe& subs)
	: m_prof(subs.m_prof),
	  _consumer(subs._consumer)
      {
      }

      subscribe& operator=(const subscribe& subs)
      {
	if (this == &subs) return *this;
	m_prof = subs.m_prof;
	_consumer = subs._consumer;
	return *this;
      }

      void operator()(InPortConsumer* cons)
      {
	if (cons->subscribeInterface(m_prof->properties))
	  {
	    _consumer = cons;
	  }
      }
      const ConnectorProfile* m_prof;
      InPortConsumer* _consumer;
    };


  };
}; // namespace RTC

#endif // DataOutPort_h