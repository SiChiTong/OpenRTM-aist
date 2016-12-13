// -*- C++ -*-
/*!
 * @file OutPort.h
 * @brief OutPort class
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

#ifndef OutPort_h
#define OutPort_h

#include <iostream>
#include <string>

#include <coil/TimeValue.h>
#include <coil/Time.h>
#include <coil/TimeMeasure.h>
#include <coil/OS.h>

#include <rtm/RTC.h>
#include <rtm/Typename.h>
#include <rtm/OutPortBase.h>
#include <rtm/CdrBufferBase.h>
#include <rtm/PortCallBack.h>
#include <rtm/OutPortConnector.h>

namespace RTC
{
  /*!
   * @if jp
   *
   * @class OutPort
   *
   * @brief OutPort �ƥ�ץ졼�ȥ��饹
   * 
   * OutPort �ƥ�ץ졼�ȥ��饹���ƥ�ץ졼�Ȥΰ����Ȥ��ơ�OutPort��
   * �����ǡ����� DataType, OutPort�ΥХåե��� BufferType ��Ȥ롣
   *
   * @since 0.2.0
   *
   * @else
   *
   * @brief OutPort template class
   * 
   * This is the OutPort template class. 
   * The data type "DateType" supported by OutPort and the buffer type
   * "BufferType" are used as template arguments.
   *
   * @since 0.2.0
   *
   * @endif
   */
  template <class DataType>
  class OutPort
    : public OutPortBase
  {
  public:
    /*!
     * @if jp
     *
     * @brief ���󥹥ȥ饯��
     *
     * ���󥹥ȥ饯��
     *
     * @param name �ݡ���̾
     * @param value ���Υݡ��Ȥ˥Х���ɤ����ǡ����ѿ�
     * @param length �Хåե�Ĺ(�ǥե������:8)
     *
     * @else
     *
     * @brief Constructor
     *
     * Constructor
     *
     * @param name Port's name
     * @param value Data variable bound to this port
     * @param length Buffer length (The default value:8)
     *
     * @endif
     */
    OutPort(const char* name, DataType& value)
      : OutPortBase(name, toTypename<DataType>()), m_value(value),
	m_timeoutTick(1000), // timeout tick: 1ms
	m_readBlock(false), m_readTimeout(0),
	m_writeBlock(false), m_writeTimeout(0),
	m_OnWrite(NULL), m_OnWriteConvert(NULL),
	m_OnRead(NULL),  m_OnReadConvert(NULL),
	m_OnOverflow(NULL), m_OnUnderflow(NULL)
    {
    }
    
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
    virtual ~OutPort(void)
    {
    }
    
    /*!
     * @if jp
     *
     * @brief �ǡ����񤭹���
     *
     * �ݡ��Ȥإǡ�����񤭹��ࡣ
     *
     * - ������Хå��ե��󥯥� OnWrite �����åȤ���Ƥ����硢
     *   OutPort ���ݻ�����Хåե��˽񤭹������� OnWrite ���ƤФ�롣
     * - OutPort ���ݻ�����Хåե��������С��ե����򸡽ФǤ���Хåե��Ǥ��ꡢ
     *   ���ġ��񤭹���ݤ˥Хåե��������С��ե����򸡽Ф�����硢
     *   ������Хå��ե��󥯥� OnOverflow ���ƤФ�롣
     * - ������Хå��ե��󥯥� OnWriteConvert �����åȤ���Ƥ����硢
     *   �Хåե��񤭹��߻��ˡ� OnWriteConvert �� operator() ������ͤ�
     *   �Хåե��˽񤭹��ޤ�롣
     *
     * @param value �񤭹����оݥǡ���
     *
     * @return �񤭹��߽������(�񤭹�������:true���񤭹��߼���:false)
     *
     * @else
     *
     * @brief Write data
     *
     * Write data in the port.
     *
     * - When callback functor OnWrite is already set, OnWrite will be
     *   invoked before writing into the buffer held by OutPort.
     * - When the buffer held by OutPort can detect the overflow,
     *   and when it detected the overflow at writing, callback functor
     *   OnOverflow will be invoked.
     * - When callback functor OnWriteConvert is already set, 
     *   the return value of operator() of OnWriteConvert will be written
     *   into the buffer at the writing.
     *
     * @param value The target data for writing
     *
     * @return Writing result (Successful:true, Failed:false)
     *
     * @endif
     */
    virtual bool write(DataType& value)
    {
      if (m_OnWrite != NULL)
	{
	  (*m_OnWrite)(value);
	}

      // check number of connectors
      size_t conn_size(m_connectors.size());
      if (!(conn_size > 0)) { return true; }
        
      // set timestamp
      coil::TimeValue tm(coil::gettimeofday());
      value.tm.sec  = tm.sec();
      value.tm.nsec = tm.usec() * 1000;

      // data -> (conversion) -> CDR stream
      m_cdr.rewindPtrs();
      if (m_OnWriteConvert != NULL)
        {
          ((*m_OnWriteConvert)(value)) >>= m_cdr;
        }
      else
        {
          value >>= m_cdr;
        }

      bool result(true);
      for (size_t i(0), len(conn_size); i < len; ++i)
        {
          ReturnCode ret;
          ret = m_connectors[i]->write(m_cdr);
          if (ret != PORT_OK)
            {
              result = false;
              if (ret == CONNECTION_LOST)
                {
                  disconnect(m_connectors[i]->id());
                }
            }
        }
      return result;
    }
    
    /*!
     * @if jp
     *
     * @brief �ǡ����񤭹���
     *
     * �ݡ��Ȥإǡ�����񤭹��ࡣ
     * �Х���ɤ��줿�ѿ������ꤵ�줿�ͤ�ݡ��Ȥ˽񤭹��ࡣ
     *
     * @return �񤭹��߽������(�񤭹�������:true���񤭹��߼���:false)
     *
     * @else
     *
     * @brief Write data
     *
     * Write data to the port.
     * Write the value, which was set to the bound variable, to the port.
     *
     * @return Writing result (Successful:true, Failed:false)
     *
     * @endif
     */
    bool write()
    {
      return write(m_value);
    }
    
    /*!
     * @if jp
     *
     * @brief �ǡ����񤭹���
     *
     * �ݡ��Ȥإǡ�����񤭹��ࡣ
     * ���ꤵ�줿�ͤ�ݡ��Ȥ˽񤭹��ࡣ
     *
     * @param value �񤭹����оݥǡ���
     *
     * @return �񤭹��߽������(�񤭹�������:true���񤭹��߼���:false)
     *
     * @else
     *
     * @brief Write data
     *
     * Write data to the port.
     * Write the set value to the port.
     *
     * @param value The target data for writing
     *
     * @return Writing result (Successful:true, Failed:false)
     *
     * @endif
     */
    bool operator<<(DataType& value)
    {
      return write(value);
    }
    
    
    /*!
     * @if jp
     *
     * @brief �ǡ����ɤ߽Ф������Υ֥��å��⡼�ɤ�����
     *
     * �ɤ߽Ф��������Ф��ƥ֥��å��⡼�ɤ����ꤹ�롣
     * �֥��å��⡼�ɤ���ꤷ����硢�ɤ߽Ф���ǡ�����������뤫�����ॢ����
     * ��ȯ������ޤǡ�read() �᥽�åɤθƤӤ������֥��å�����롣
     *
     * @param block �֥��å��⡼�ɥե饰
     *
     * @else
     *
     * @brief Set blocking mode of the data read processing
     *
     * Set the blocking mode for the readout.
     * When the block mode is specified, the invoke of read() method is
     * blocked until the readout data is received or timeout occurs.
     *
     * @param block Flag of blocking mode
     *
     * @endif
     */
    void setReadBlock(bool block)
    {
      m_readBlock = block;
    }
    
    /*!
     * @if jp
     *
     * @brief �ǡ����񤭹��߽����Υ֥��å��⡼�ɤ�����
     *
     * �񤭹��߽������Ф��ƥ֥��å��⡼�ɤ����ꤹ�롣
     * �֥��å��⡼�ɤ���ꤷ����硢�Хåե��˽񤭹����ΰ褬�Ǥ��뤫
     * �����ॢ���Ȥ�ȯ������ޤ� write() �᥽�åɤθƤӤ������֥��å�����롣
     *
     * @param block �֥��å��⡼�ɥե饰
     *
     * @else
     *
     * @brief Set blocking mode of the data writing processing
     *
     * Set the blocking mode for the writing.
     * When the block mode is specified, the invoke of write() method is
     * blocked until the area written into the buffer can be used or timeout
     * occurs.
     *
     * @param block Flag of blocking mode
     *
     * @endif
     */
    void setWriteBlock(bool block)
    {
      m_writeBlock = block;
    }
    
    /*!
     * @if jp
     *
     * @brief �ɤ߽Ф������Υ����ॢ���Ȼ��֤�����
     * 
     * read() �Υ����ॢ���Ȼ��֤� usec �����ꤹ�롣
     * read() �ϥ֥��å��⡼�ɤǤʤ���Фʤ�ʤ���
     *
     * @param timeout �����ॢ���Ȼ��� [usec]
     *
     * @else
     *
     * @brief Set timeout of the data read processing
     * 
     * Set the timeout period of read() with usec.
     * read() must be a block mode.
     *
     * @param timeout Timeout period[usec]
     *
     * @else
     *
     * @endif
     */
    void setReadTimeout(long int timeout)
    {
      m_readTimeout = timeout;
    }
    
    /*!
     * @if jp
     *
     * @brief �񤭹��߽����Υ����ॢ���Ȼ��֤�����
     * 
     * write() �Υ����ॢ���Ȼ��֤� usec �����ꤹ�롣
     * write() �ϥ֥��å��⡼�ɤǤʤ���Фʤ�ʤ���
     *
     * @param timeout �����ॢ���Ȼ��� [usec]
     *
     * @else
     *
     * @brief Set timeout of the data writing processing
     * 
     * Set the timeout period of write() with usec.
     * write() must be a block mode.
     *
     * @param timeout Timeout period[usec]
     *
     * @endif
     */
    void setWriteTimeout(long int timeout)
    {
      m_writeTimeout = timeout;
    }
    
    /*!
     * @if jp
     *
     * @brief OnWrite ������Хå�������
     *
     * �ǡ����񤭹���ľ���˸ƤФ�� OnWrite ������Хå��ե��󥯥������ꤹ�롣
     *
     * @param on_write OnWrite ������Хå��ե��󥯥�
     *
     * @else
     *
     * @brief Set OnWrite callback
     *
     * Set OnWrite callback functor invoked immediately before data is written.
     *
     * @param on_write OnWrite callback functor
     *
     * @endif
     */
    inline void setOnWrite(OnWrite<DataType>* on_write)
    {
      m_OnWrite = on_write;
    }
    
    /*!
     * @if jp
     *
     * @brief OnWriteConvert ������Хå�������
     *
     * �ǡ����񤭹��߻��˸ƤФ�� OnWriteConvert ������Хå��ե��󥯥�������
     * ���롣
     * ���Υ�����Хå��ؿ��ν�����̤��񤭹��ޤ�롣
     * ���Τ���񤭹��ߥǡ����Υե��륿��󥰤���ǽ�Ȥʤ롣
     *
     * @param on_wconvert OnWriteConvert ������Хå��ե��󥯥�
     *
     * @else
     *
     * @brief Set OnWriteConvert callback
     *
     * Set OnWriteConvert callback functor invoked when data is written.
     * The processing result of this callback function is written.
     * Therefore, filtering of the writing data will be possible.
     *
     * @param on_wconvert OnWriteConvert callback functor
     *
     * @endif
     */
    inline void setOnWriteConvert(OnWriteConvert<DataType>* on_wconvert)
    {
      m_OnWriteConvert = on_wconvert;
    }
    
    /*!
     * @if jp
     *
     * @brief OnOverflow ������Хå�������
     *
     * �Хåե��ե�ˤ��ǡ����񤭹��ߤ��Ǥ��ʤ����˸ƤӽФ���� OnOverflow
     * ������Хå��ե��󥯥������ꤹ�롣
     *
     * @param on_overflow OnOverflow ������Хå��ե��󥯥�
     *
     * @else
     *
     * @brief Set OnOverflow callback
     *
     * Set OnOverflow callback functor, which is invoked when data cannot be
     * written because of the buffer full.
     *
     * @param on_overflow OnOverflow callback functor
     *
     * @endif
     */
    inline void setOnOverflow(OnOverflow<DataType>* on_overflow)
    {
      m_OnOverflow = on_overflow;
    }
    
    /*!
     * @if jp
     *
     * @brief OnRead ������Хå�������
     *
     * �ǡ����ɤ߽Ф�ľ���˸ƤӽФ���� OnRead ������Хå��ե��󥯥�������
     * ���롣
     *
     * @param on_read OnRead ������Хå��ե��󥯥�
     *
     * @else
     *
     * @brief Set OnRead callback
     *
     * Set OnWrite callback functor invoked immediately before data is readout.
     *
     * @param on_read OnRead callback functor
     *
     * @endif
     */
    inline void setOnRead(OnRead<DataType>* on_read)
    {
      m_OnRead = on_read;
    }
    
    /*!
     * @if jp
     *
     * @brief OnReadConvert ������Хå�������
     *
     * �ǡ����ɤ߽Ф����˸ƤФ�� OnReadConvert ������Хå��ե��󥯥�������
     * ���롣
     * ���Υ�����Хå��ؿ��ν�����̤��ɤ߹��ޤ�롣
     * ���Τ����ɤ߹��ߥǡ����Υե��륿��󥰤���ǽ�Ȥʤ롣
     *
     * @param on_rconvert OnReadConvert ������Хå��ե��󥯥�
     *
     * @else
     *
     * @brief Set OnReadConvert callback
     *
     * Set OnReadConvert callback functor invoked when data is readout.
     * The processing result of this callback function is readout.
     * Therefore, filtering of the read data will be possible.
     *
     * @param on_rconvert OnReadConvert callback functor
     *
     * @endif
     */
    inline void setOnReadConvert(OnReadConvert<DataType>* on_rconvert)
    {
      m_OnReadConvert = on_rconvert;
    }
    
    /*!
     * @if jp
     *
     * @brief OnUnderflow ������Хå�������
     *
     * �Хåե�����ץƥ��ˤ���ɤ߽Ф���ǡ������ʤ����˸ƤӽФ����
     * ������Хå��ե��󥯥� OnUnderflow �����ꤹ�롣
     *
     * @param on_underflow OnUnderflow ������Хå��ե��󥯥�
     *
     * @else
     *
     * @brief Set OnUnderflow callback
     *
     * Set OnUnderflow callback functor, which is invoked when data cannot be
     * readout because of the buffer empty.
     *
     * @param on_underflow OnUnderflow callback functor
     *
     * @endif
     */
    inline void setOnUnderflow(OnUnderflow<DataType>* on_underflow)
    {
      m_OnUnderflow = on_underflow;
    }    


    inline void setOnConnect(OnConnect* on_connect)
    {
      m_OnConnect = on_connect;
    }
    
    inline void setOnDisconnect(OnConnect* on_disconnect)
    {
      m_OnDisconnect = on_disconnect;
    }
    
    virtual void onConnect(const char* id, PublisherBase* publisher)
    {
      std::cout << "onConnect(id = " << id << ")" << std::endl;
    }

    virtual void onDisconnect(const char* id)
    {
      std::cout << "onDisconnect(id = " << id << ")" << std::endl;
    }



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


  private:
    std::string m_typename;
    /*!
     * @if jp
     * @brief �Х���ɤ���� T �����ѿ��ؤλ���
     * @else
     * @brief The reference to type-T variable that is bound.
     * @endif
     */
    DataType& m_value;
    
    /*!
     * @if jp
     * @brief �����ॢ���ȤΥݡ���󥰼��� [usec]
     * @else
     * @brief Polling cycle of time-out [usec]
     * @endif
     */
    long int m_timeoutTick;
    
    /*!
     * @if jp
     * @brief �ɤ߹��߽������Υ֥��å�����֥��å��⡼�ɥե饰
     * @else
     * @brief Flag of read()'s blocking/non-blocking mode
     * @endif
     */
    bool m_readBlock;
    
    /*!
     * @if jp
     * @brief �ɤ߹��߽����Υ����ॢ���Ȼ��� [usec]
     * @else
     * @brief Timeout of read() [usec]
     * @endif
     */
    long int m_readTimeout;
    
    /*!
     * @if jp
     * @brief �񤭹��߽������Υ֥��å�����֥��å��⡼�ɥե饰
     * @else
     * @brief Flag of write()'s blocking/non-blocking mode
     * @endif
     */
    bool m_writeBlock;
    
    /*!
     * @if jp
     * @brief �񤭹��߽����Υ����ॢ���Ȼ��� [usec]
     * @else
     * @brief Timeout of write() [usec]
     * @endif
     */
    long int m_writeTimeout;
    
    /*!
     * @if jp
     * @brief OnWrite ������Хå��ե��󥯥��ؤΥݥ���
     * @else
     * @brief Pointer to OnWrite callback functor
     * @endif
     */
    OnWrite<DataType>* m_OnWrite;
    
    /*!
     * @if jp
     * @brief OnWriteConvert ������Хå��ե��󥯥��ؤΥݥ���
     * @else
     * @brief Pointer to OnWriteConvert callback functor
     * @endif
     */
    OnWriteConvert<DataType>* m_OnWriteConvert;
    
    /*!
     * @if jp
     * @brief OnRead ������Хå��ե��󥯥��ؤΥݥ���
     * @else
     * @brief Pointer to OnRead callback functor
     * @endif
     */
    OnRead<DataType>* m_OnRead;
    
    /*!
     * @if jp
     * @brief OnReadConvert ������Хå��ե��󥯥��ؤΥݥ���
     * @else
     * @brief Pointer to OnReadConvert callback functor
     * @endif
     */
    OnReadConvert<DataType>* m_OnReadConvert;
    
    /*!
     * @if jp
     * @brief OnOverflow ������Хå��ե��󥯥��ؤΥݥ���
     * @else
     * @brief Pointer to OnOverflow callback functor
     * @endif
     */
    OnOverflow<DataType>* m_OnOverflow;
    
    /*!
     * @if jp
     * @brief OnUnderflow ������Хå��ե��󥯥��ؤΥݥ���
     * @else
     * @brief Pointer to OnUnderflow callback functor
     *
     * @endif
     */
    OnUnderflow<DataType>* m_OnUnderflow;
    
    OnConnect* m_OnConnect;
    OnDisconnect* m_OnDisconnect;


    std::vector<OutPortProvider*> m_providers;
    std::vector<InPortConsumer*> m_consumers;
    //    std::vector<ConnectorBase*> m_connectors;
    coil::TimeMeasure m_cdrtime;
    static const long int usec_per_sec = 1000000;
    cdrMemoryStream m_cdr;
  };
}; // namespace RTC

#endif // OutPort_h