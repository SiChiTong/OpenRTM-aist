// -*- C++ -*-
/*!
 * @file OutPortPullConnector.h
 * @brief OutPortPull type connector class
 * @date $Date$
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2009
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

#ifndef RTC_OUTPORTPULLCONNECTOR_H
#define RTC_OUTPORTPULLCONNECTOR_H

#include <rtm/OutPortConnector.h>
#include <rtm/ConnectorListener.h>

namespace RTC
{
  class OutPortProvider;

  /*!
   * @if jp
   * @class OutPortPullConnector
   * @brief OutPortPullConnector ���饹
   *
   * OutPort �� Pull ���ǡ����ե����Τ���� Connector
   *
   * @since 1.0.0
   *
   * @else
   * @class OutPortPullConnector
   * @brief OutPortPullConnector base class
   *
   * A connector class for pull type dataflow of OutPort
   *
   * @since 1.0.0
   *
   * @endif
   */
  class OutPortPullConnector
    : public OutPortConnector
  {
  public:
    DATAPORTSTATUS_ENUM

    /*!
     * @if jp
     * @brief ���󥹥ȥ饯��
     *
     * OutPortPullConnector �� OutPortProvider �ν�ͭ������ġ�
     * �������äơ�OutPortPullConnector ������ˤϡ�OutPortProvider ��Ʊ����
     * ���Ρ��������롣
     *
     * @param profile pointer to a ConnectorProfile
     * @param provider pointer to an OutPortProvider
     * @param buffer pointer to a buffer
     *
     * @elsek
     * @brief Constructor
     *
     * OutPortPullConnector assume ownership of InPortConsumer.
     * Therefore, OutPortProvider will be deleted when OutPortPushConnector
     * is destructed.
     *
     * @param profile pointer to a ConnectorProfile
     * @param provider pointer to an OutPortProvider
     * @param buffer pointer to a buffer
     *
     * @endif
     */
    OutPortPullConnector(ConnectorInfo info,
                         OutPortProvider* provider,
                         ConnectorListeners& listeners,
                         CdrBufferBase* buffer = 0);

    /*!
     * @if jp
     * @brief �ǥ��ȥ饯��
     *
     * disconnect() ���ƤФ졢provider, buffer �����Ρ��������롣
     *
     * @else
     *
     * @brief Destructor
     *
     * This operation calls disconnect(), which destructs and deletes
     * the consumer, the publisher and the buffer.
     *
     * @endif
     */
    virtual ~OutPortPullConnector();

    /*!
     * @if jp
     * @brief �ǡ����ν񤭹���
     *
     * Publisher���Ф��ƥǡ�����񤭹��ߡ�����ˤ���б�����InPort��
     * �ǡ�����ž������롣
     *
     * @else
     *
     * @brief Writing data
     *
     * This operation writes data into publisher and then the data
     * will be transferred to correspondent InPort.
     *
     * @endif
     */
    virtual ReturnCode write(const cdrMemoryStream& data);

    /*!
     * @if jp
     * @brief ��³���
     *
     * consumer, publisher, buffer �����Ρ��������롣
     *
     * @else
     *
     * @brief disconnect
     *
     * This operation destruct and delete the consumer, the publisher
     * and the buffer.
     *
     * @endif
     */
    virtual ReturnCode disconnect();

    /*!
     * @if jp
     * @brief Buffer ���������
     *
     * Connector ���ݻ����Ƥ��� Buffer ���֤�
     *
     * @else
     * @brief Getting Buffer
     *
     * This operation returns this connector's buffer
     *
     * @endif
     */
    virtual CdrBufferBase* getBuffer();

    /*!
     * @if jp
     * @brief �����ƥ��ֲ�
     *
     * ���Υ��ͥ����򥢥��ƥ��ֲ�����
     *
     * @else
     *
     * @brief Connector activation
     *
     * This operation activates this connector
     *
     * @endif
     */
    virtual void activate(){}; // do nothing

    /*!
     * @if jp
     * @brief �󥢥��ƥ��ֲ�
     *
     * ���Υ��ͥ������󥢥��ƥ��ֲ�����
     *
     * @else
     *
     * @brief Connector deactivation
     *
     * This operation deactivates this connector
     *
     * @endif
     */
    virtual void deactivate(){}; // do nothing

    /*!
     * @if jp
     * @brief Buffer������
     * @else
     * @brief create buffer
     * @endif
     */
    CdrBufferBase* createBuffer(ConnectorInfo& info);

    /*!
     * @if jp
     * @brief ��³��Ω���˥�����Хå���Ƥ�
     * @else
     * @brief Invoke callback when connection is established
     * @endif
     */
    void onConnect();

    /*!
     * @if jp
     * @brief ��³���ǻ��˥�����Хå���Ƥ�
     * @else
     * @brief Invoke callback when connection is destroied
     * @endif
     */
    void onDisconnect();

  protected:

    /*!
     * @if jp
     * @brief OutPortProvider �ؤΥݥ���
     * @else
     * @brief the pointer to the OutPortProvider
     * @endif
     */
    OutPortProvider* m_provider;

    /*!
     * @if jp
     * @brief ConnectorListenrs �ؤλ���
     * @else
     * @brief A reference to a ConnectorListener
     * @endif
     */
    ConnectorListeners& m_listeners;

    /*!
     * @if jp
     * @brief Buffer �ؤΥݥ���
     * @else
     * @brief the pointer to the buffer
     * @endif
     */
    CdrBufferBase* m_buffer;
  };
}; // namespace RTC

#endif  // RTC_PULL_CONNECTOR_H