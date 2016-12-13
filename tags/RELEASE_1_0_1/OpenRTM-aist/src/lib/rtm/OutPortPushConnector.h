// -*- C++ -*-
/*!
 * @file OutPortPushConnector.h
 * @brief Push type connector class
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

#ifndef RTC_OUTPORTPUSHCONNECTOR_H
#define RTC_OUTPORTPUSHCONNECTOR_H

#include <rtm/OutPortConnector.h>
#include <rtm/InPortConsumer.h>
#include <rtm/PublisherBase.h>

namespace RTC
{
  class ConnectorListeners;

  /*!
   * @if jp
   * @class OutPortPushConnector
   * @brief OutPortPushConnector ���饹
   *
   *
   * @else
   * @class OutPortPushConnector
   * @brief OutPortPushConnector class
   *
   *
   * @endif
   */
  class OutPortPushConnector
    : public OutPortConnector
  {
  public:
    DATAPORTSTATUS_ENUM

    /*!
     * @if jp
     * @brief ���󥹥ȥ饯��
     *
     * OutPortPushConnector �� InPortConsumer �ν�ͭ������ġ�
     * �������äơ�OutPortPushConnector ������ˤϡ�InPortConsumer��Ʊ����
     * ���Ρ��������롣
     *
     * @param profile ConnectorProfile
     * @param consumer InPortConsumer
     *
     * @elsek
     * @brief Constructor
     *
     * OutPortPushConnector assume ownership of InPortConsumer.
     * Therefore, InPortConsumer will be deleted when OutPortPushConnector
     * is destructed.
     *
     * @param profile ConnectorProfile
     * @param consumer InPortConsumer
     *
     * @endif
     */
    OutPortPushConnector(ConnectorInfo info,
                         InPortConsumer* consumer,
                         ConnectorListeners& listeners,
                         CdrBufferBase* buffer = 0);

    /*!
     * @if jp
     * @brief �ǥ��ȥ饯��
     *
     * disconnect() ���ƤФ졢consumer, publisher, buffer �����Ρ��������롣
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
    virtual ~OutPortPushConnector();

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
    virtual void activate();

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
    virtual void deactivate();

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

  protected:
    /*!
     * @if jp
     * @brief Publisher������
     * @else
     * @brief create publisher
     * @endif
     */
    virtual PublisherBase* createPublisher(ConnectorInfo& info);

    /*!
     * @if jp
     * @brief Buffer������
     * @else
     * @brief create buffer
     * @endif
     */
    virtual CdrBufferBase* createBuffer(ConnectorInfo& info);

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

  private:
    /*!
     * @if jp
     * @brief InPortConsumer �ؤΥݥ���
     * @else
     * @brief A pointer to an InPortConsumer
     * @endif
     */
    InPortConsumer* m_consumer;

    /*!
     * @if jp
     * @brief Publisher �ؤΥݥ���
     * @else
     * @brief A pointer to a publisher
     * @endif
     */
    PublisherBase* m_publisher;

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
     * @brief A pointer to a buffer
     * @endif
     */
    CdrBufferBase* m_buffer;

  };
}; // namespace RTC

#endif  // RTC_PUSH_CONNECTOR_H