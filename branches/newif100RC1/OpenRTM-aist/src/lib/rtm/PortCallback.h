// -*- C++ -*-
/*!
 * @file PortCallback.h
 * @brief PortCallback class
 * @date $Date: 2007-12-31 03:08:05 $
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

#ifndef RTC_PORTCALLBACK_H
#define RTC_PORTCALLBACK_H

namespace RTC
{
  //============================================================
  // callback functor base classes
  /*!
   * @if jp
   * @class ConnectCallback
   * @brief connect/notify_connect() ���Υ�����Хå���ݥ��饹
   *
   * Port���Ф���connect/notify_connect() �����ƤӽФ������˸ƤӽФ����
   * ������Хå��ե��󥯥��������� RTC::ConnectorProfile ���롣
   *
   * @param profile ConnectorProfile
   *
   * @since 1.0.0
   *
   * @else
   * @class ConnectCallback
   * @brief Callback functor abstract for connect/notify_connect() funcs
   *
   * This is the interface for callback functor for connect/notify_connect()
   * invocation in Port. Argument is RTC::ConnectorProfile that is given
   * these functions.
   *
   * @param profile ConnectorProfile
   *
   * @since 1.0.0
   *
   * @endif
   */
  class ConnectionCallback
  {
  public:
    virtual ~ConnectionCallback(void){}
    virtual void operator()(RTC::ConnectorProfile& profile) = 0;
  };

  /*!
   * @if jp
   * @class DisconnectCallback
   * @brief disconnect/notify_disconnect() ���Υ�����Хå���ݥ��饹
   *
   * Port���Ф���disconnect/notify_disconnect() �����ƤӽФ������˸ƤӽФ����
   * ������Хå��ե��󥯥�����������³ID���롣
   *
   * @param connector_id Connector ID
   *
   * @since 1.0.0
   *
   * @else
   * @class DisconnectCallback
   * @brief Callback functor abstract for disconnect/notify_disconnect() funcs
   *
   * This is the interface for callback functor for 
   * disconnect/notify_disconnect() invocation in Port.
   * Argument is connector ID is given these functions.
   *
   * @param connector_id Connector ID
   *
   * @since 1.0.0
   *
   * @endif
   */
  class DisconnectCallback
  {
  public:
    virtual ~DisconnectCallback(void){}
    virtual void operator()(const char* connector_id) = 0;
  };

  /*!
   * @if jp
   * @class OnWrite
   * @brief write() ���Υ�����Хå���ݥ��饹
   *
   * OutPort���Ф��ƥǡ�����write()�����ľ���˸ƤӽФ���륳����Хå���
   * �ե��󥯥���
   *
   * @param DataType �Хåե��˽񤭹���ǡ�����
   *
   * @since 0.4.0
   *
   * @else
   * @class OnWrite
   * @brief Callback abstract class on write()
   *
   * This is the interface for callback invoked immediately before
   * data is done write() into the DataPort's buffer.
   *
   * @param DataType Data type to write into the buffer
   *
   * @since 0.4.0
   *
   * @endif
   */
  template <class DataType>
  class OnWrite
  {
  public:
    virtual ~OnWrite(void){}
    
    /*!
     * @if jp
     *
     * @brief ������Хå��ؿ�
     *
     * �Хåե��˥ǡ������񤭹��ޤ��ľ���˸ƤӽФ���륳����Хå��ؿ�
     *
     * @param value �Хåե��˽񤭹��ޤ��ǡ���
     *
     * @else
     *
     * @brief Callback method
     *
     * This is the callback method invoked immediately before data is written
     * into the buffer.
     *
     * @param value Data that is written into the buffer
     *
     * @endif
     */
    virtual void operator()(const DataType& value) = 0;
  };
  
  /*!
   * @if jp
   * @class OnWriteConvert
   * @brief write() ���Υǡ����Ѵ�������Хå���ݥ��饹
   *
   * InPort/OutPort�ΥХåե��˥ǡ����� write()�������˸ƤӽФ����
   * ������Хå��ѥ��󥿡��ե�������
   * ���Υ�����Хå�������ͤ��Хåե��˳�Ǽ����롣
   *
   * @since 0.4.0
   *
   * @else
   * @class OnWriteConvert
   * @brief Data convert callback abstract class on write()
   *
   * This is the interface for callback invoked when data is done write()
   * into the InPort/OutPort's buffer.
   * The return value of this callback will be stored in the buffer.
   *
   * @since 0.4.0
   *
   * @endif
   */
  template <class DataType>
  struct OnWriteConvert
  {
    virtual ~OnWriteConvert(void){}
    
    /*!
     * @if jp
     *
     * @brief ������Хå��᥽�å�
     *
     * �Хåե��˥ǡ������񤭹��ޤ��ݤ˸ƤӽФ���륳����Хå��᥽�å�
     *
     * @param value �Ѵ����ǡ���
     *
     * @return �Ѵ���ǡ���
     *
     * @else
     *
     * @brief Callback method
     *
     * This is the callback method invoked when data is written into the
     * buffer.
     *
     * @param value Data before the convert
     *
     * @return Data after the convert
     *
     * @endif
     */
    virtual DataType operator()(const DataType& value) = 0;
  };
  
  /*!
   * @if jp
   * @class OnRead
   * @brief read() ���Υ�����Хå���ݥ��饹
   *
   * InPort/OutPort�ΥХåե�����ǡ����� read()�����ľ���˸ƤӽФ����
   * ������Хå��ѥ��󥿡��ե�������
   *
   * @since 0.4.0
   *
   * @else
   * @class OnRead
   * @brief Callback abstract class on read()
   *
   * This is the interface for callback invoked immediately before
   * data is done read() from the InPort/OutPort's buffer.
   *
   * @since 0.4.0
   *
   * @endif
   */
  template <class DataType>
  struct OnRead
  {
    /*!
     * @if jp
     *
     * @brief �ǥ��ȥ饯��
     *
     * �ǥ��ȥ饯��
     *
     * @else
     *
     * @brief Destructor
     *
     * Destructor
     *
     * @endif
     */
    virtual ~OnRead(void){}
    
    /*!
     * @if jp
     *
     * @brief ������Хå��᥽�å�
     *
     * �Хåե�����ǡ������ɤ߽Ф����ľ���˸ƤӽФ���륳����Хå��᥽�å�
     *
     * @else
     *
     * @brief Callback method
     *
     * This is the callback method invoked immediately before data is readout
     * from the buffer.
     *
     * @endif
     */
    virtual void operator()() = 0;
  };
  
  /*!
   * @if jp
   * @class OnReadConvert
   * @brief read() ���Υǡ����Ѵ�������Хå���ݥ��饹
   *
   * InPort/OutPort�ΥХåե�����ǡ����� read()�����ݤ˸ƤӽФ����
   * ������Хå��ѥ��󥿡��ե�������
   * ���Υ�����Хå�������ͤ�read()������ͤȤʤ롣
   *
   * @since 0.4.0
   *
   * @else
   * @class OnReadConvert
   * @brief Data convert callback abstract class on read()
   *
   * This is the interface for callback invoked when data is done read()
   * from the InPort/OutPort's buffer.
   * The return value of this callback will be the return value of read().
   *
   * @since 0.4.0
   *
   * @endif
   */
  template <class DataType>
  struct OnReadConvert
  {
    /*!
     * @if jp
     *
     * @brief �ǥ��ȥ饯��
     *
     * �ǥ��ȥ饯��
     *
     * @else
     *
     * @brief Destructor
     *
     * Destructor
     *
     * @endif
     */
    virtual ~OnReadConvert(void){}
    
    /*!
     * @if jp
     *
     * @brief ������Хå��᥽�å�
     *
     * �Хåե�����ǡ������ɤ߽Ф����ݤ˸ƤӽФ���륳����Хå��᥽�å�
     *
     * @else
     *
     * @brief Callback method
     *
     * This is the callback method invoked when data is readout from the
     * buffer.
     *
     * @endif
     */
    virtual DataType operator()(const DataType& value) = 0;
  };
  
  /*!
   * @if jp
   * @class OnOverflow
   * @brief �Хåե������С��ե������Υ�����Хå���ݥ��饹
   *
   * InPort/OutPort�ΥХåե��˥ǡ�����put()���������Хåե������С��ե�����
   * ���������˸ƤФ�륳����Хå��᥽�åɡ�
   *
   * @since 0.4.0
   *
   * @else
   * @class OnOverflow
   * @brief Callback abstract class when the buffer overflow occurs
   *
   * This is the callback method invoked when data is done put()
   * into the InPort/OutPort's buffer or when the buffer overflow occurs.
   *
   * @since 0.4.0
   *
   * @endif
   */
  template <class DataType>
  struct OnOverflow
  {
    /*!
     * @if jp
     *
     * @brief �ǥ��ȥ饯��
     *
     * �ǥ��ȥ饯��
     *
     * @else
     *
     * @brief Destructor
     *
     * Destructor
     *
     * @endif
     */
    virtual ~OnOverflow(void){}
    
    /*!
     * @if jp
     *
     * @brief ������Хå��᥽�å�
     *
     * �Хåե������С��ե������˸ƤӽФ���륳����Хå��᥽�å�
     *
     * @param value �Хåե��ؽ񤭹���ǡ���
     *
     * @else
     *
     * @brief Callback method
     *
     * This is the callback method invoked when the buffer overflow occurs.
     *
     * @param value Data that is written into the buffer
     *
     * @endif
     */
    virtual void operator()(const DataType& value) = 0;
  };
  
  /*!
   * @if jp
   * @class OnUnderflow
   * @brief Underflow ���Υ�����Хå���ݥ��饹
   *
   * @since 0.4.0
   *
   * InPort/OutPort�ΥХåե��˥ǡ�����read()�������ˡ��ɤ߽Ф��٤��ǡ�����
   * �ʤ����˸ƤӽФ���륳����Хå����󥿥ե�������
   * ���Υ�����Хå�������ͤ�read()������ͤȤʤ롣
   *
   * @else
   * @class OnUnderflow
   * @brief Callback abstract class on underflow
   *
   * @since 0.4.0
   *
   * This is the interface for callback invoked when data is done read()
   * into the InPort/OutPort's buffer and the readout data does not exist.
   * The return value of this callback will be the return value of read().
   *
   * @endif
   */
  template <class DataType>
  struct OnUnderflow
  {
    /*!
     * @if jp
     *
     * @brief �ǥ��ȥ饯��
     *
     * �ǥ��ȥ饯��
     *
     * @else
     *
     * @brief Destructor
     *
     * Destructor
     *
     * @endif
     */
    virtual ~OnUnderflow(void){}
    
    /*!
     * @if jp
     *
     * @brief ������Хå��᥽�å�
     *
     * �Хåե���������ե������˸ƤӽФ���륳����Хå��᥽�å�
     *
     * @return ���ؤȤʤ��ɤ߽Ф��ǡ���
     *
     * @else
     *
     * @brief Callback method
     *
     * This is the callback method invoked when the buffer underflow occurs.
     *
     * @return Substituted readout data
     *
     * @endif
     */
    virtual DataType operator()() = 0;
  };
  
  /*!
   * @if jp
   * @class OnWriteTimeout
   * @brief �����ॢ���Ȼ��Υ�����Хå���ݥ��饹
   *
   * InPort/OutPort�ΥХåե��˥ǡ�����write()����ݤˡ������ॢ���Ȥ�ȯ������
   * ���˸ƤӽФ���륳����Хå����󥿥ե�������
   *
   * @since 0.4.0
   *
   * @else
   * @class OnWriteTimeout
   * @brief Callback abstract class on timeout
   *
   * This is the interface for callback invoked when data is done write()
   * into the InPort/OutPort's buffer and the timeout occurred.
   *
   * @since 0.4.0
   *
   * @endif
   */
  template <class DataType>
  struct OnWriteTimeout
  {
    /*!
     * @if jp
     *
     * @brief �ǥ��ȥ饯��
     *
     * �ǥ��ȥ饯��
     *
     * @else
     *
     * @brief Destructor
     *
     * Destructor
     *
     * @endif
     */
    virtual ~OnWriteTimeout(void){}
    
    /*!
     * @if jp
     *
     * @brief ������Хå��᥽�å�
     *
     * �����ॢ����ȯ�����˸ƤӽФ���륳����Хå��᥽�å�
     *
     * @param value �Хåե��ؽ񤭹���ǡ���
     *
     * @else
     *
     * @brief Callback method
     *
     * This is the callback method invoked when the timeout occurs.
     *
     * @param value Data that is written into the buffer
     *
     * @endif
     */
    virtual void operator()(const DataType& value) = 0;
  };
  
  /*!
   * @if jp
   * @class OnReadTimeout
   * @brief �����ॢ���Ȼ��Υ�����Хå���ݥ��饹
   *
   * InPort/OutPort�ΥХåե��˥ǡ�����read()����ݤˡ������ॢ���Ȥ�ȯ������
   * ���˸ƤӽФ���륳����Хå����󥿥ե�������
   *
   * @since 0.4.0
   *
   * @else
   * @class OnReadTimeout
   * @brief OnReadTimeout abstract class
   *
   * This is the interface for callback invoked when data is done read()
   * into the InPort/OutPort's buffer and the timeout occurred.
   *
   * @since 0.4.0
   *
   * @endif
   */
  template <class DataType>
  struct OnReadTimeout
  {
    /*!
     * @if jp
     *
     * @brief �ǥ��ȥ饯��
     *
     * �ǥ��ȥ饯��
     *
     * @else
     *
     * @brief Destructor
     *
     * Destructor
     *
     * @endif
     */
    virtual ~OnReadTimeout(void){}
    
    /*!
     * @if jp
     *
     * @brief ������Хå��᥽�å�
     *
     * �����ॢ����ȯ�����˸ƤӽФ���륳����Хå��᥽�å�
     *
     * @return ���ؤȤʤ��ɤ߽Ф��ǡ���
     *
     * @else
     *
     * @brief Callback method
     *
     * This is the callback method invoked when the timeout occurs.
     *
     * @return Substituted readout data
     *
     * @endif
     */
    virtual DataType operator()() = 0;
  };


};
#endif // RTC_PORTCALLBACK_H