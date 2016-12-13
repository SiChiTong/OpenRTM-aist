// -*- C++ -*-
/*!
 * @file  InPortCorbaCdrProvider.h
 * @brief InPortCorbaCdrProvider class
 * @date  $Date: 2008-01-14 07:49:59 $
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
 * $Id: InPortCorbaCdrProvider.h 1244 2009-03-13 07:25:42Z n-ando $
 *
 */

#include <rtm/InPortCorbaCdrProvider.h>
#include <rtm/idl/BasicDataTypeSkel.h>
namespace RTC
{
  /*!
   * @if jp
   * @brief ���󥹥ȥ饯��
   * @else
   * @brief Constructor
   * @endif
   */
  InPortCorbaCdrProvider::InPortCorbaCdrProvider(void)
   : m_buffer(0) 
  {
    rtclog.setLevel("PARANOID");
    // PortProfile setting
    setInterfaceType("corba_cdr");
    
    // ConnectorProfile setting
    m_objref = this->_this();
    
    // set InPort's reference
    CORBA::ORB_ptr orb = ::RTC::Manager::instance().getORB();
    CORBA_SeqUtil::
      push_back(m_properties,
                NVUtil::newNV("dataport.corba_cdr.inport_ior",
                              orb->object_to_string(m_objref.in())));
    CORBA_SeqUtil::
      push_back(m_properties,
                NVUtil::newNV("dataport.corba_cdr.inport_ref",
                              m_objref));
  }
  
  /*!
   * @if jp
   * @brief �ǥ��ȥ饯��
   * @else
   * @brief Destructor
   * @endif
   */
  InPortCorbaCdrProvider::~InPortCorbaCdrProvider(void)
  {
    PortableServer::ObjectId_var oid;
    oid = _default_POA()->servant_to_id(this);
    _default_POA()->deactivate_object(oid);
  }

  void InPortCorbaCdrProvider::init(coil::Properties& prop)
  {
  }

  void InPortCorbaCdrProvider::
  setBuffer(BufferBase<cdrMemoryStream>* buffer)
  {
    m_buffer = buffer;
  }


  /*!
   * @if jp
   * @brief �Хåե��˥ǡ�����񤭹���
   * @else
   * @brief Write data into the buffer
   * @endif
   */
  ::OpenRTM::PortStatus
  InPortCorbaCdrProvider::put(const ::OpenRTM::CdrData& data)
    throw (CORBA::SystemException)
  {
    RTC_PARANOID(("put()"));

    if (m_buffer == 0)
      {
        return ::OpenRTM::PORT_ERROR;
      }

    if (m_buffer->full())
      {
        RTC_WARN(("buffer full"));
        return ::OpenRTM::BUFFER_FULL;
      }

    RTC_PARANOID(("received data size: %d", data.length()))
    cdrMemoryStream cdr;
    cdr.put_octet_array(&(data[0]), data.length());

    RTC_PARANOID(("converted CDR data size: %d", cdr.bufSize()));
    BufferStatus::Enum ret = m_buffer->write(cdr);

    switch(ret)
      {
      case BufferStatus::BUFFER_OK:
        return ::OpenRTM::PORT_OK;
        break;
      case BufferStatus::BUFFER_ERROR:
        return ::OpenRTM::PORT_ERROR;
        break;
      case BufferStatus::BUFFER_FULL:
        return ::OpenRTM::BUFFER_FULL;
        break;
      case BufferStatus::BUFFER_EMPTY:
        return ::OpenRTM::BUFFER_EMPTY;
        break;
      case BufferStatus::TIMEOUT:
        return ::OpenRTM::BUFFER_TIMEOUT;
        break;
      default:
        return ::OpenRTM::UNKNOWN_ERROR;
      }
    return ::OpenRTM::UNKNOWN_ERROR;
  }
};     // namespace RTC


extern "C"
{
  void InPortCorbaCdrProviderInit(void)
  {
    RTC::InPortProviderFactory& factory(RTC::InPortProviderFactory::instance());
    factory.addFactory("corba_cdr",
                       ::coil::Creator< ::RTC::InPortProvider,
                                        ::RTC::InPortCorbaCdrProvider>,
                       ::coil::Destructor< ::RTC::InPortProvider,
                                           ::RTC::InPortCorbaCdrProvider>);
  }
};