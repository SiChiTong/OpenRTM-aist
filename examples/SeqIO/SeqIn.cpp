// -*- C++ -*-
/*!
 * @file  SeqIn.cpp
 * @brief Sequence InPort component
 * $Date: 2007-04-13 15:02:42 $
 *
 * $Id$
 */

#include "SeqIn.h"
#include <iostream>

// Connector Listener Dump Flag
extern bool g_Listener_dump_enabled;

// Module specification
// <rtc-template block="module_spec">
static const char* seqin_spec[] =
  {
    "implementation_id", "SeqIn",
    "type_name",         "SequenceInComponent",
    "description",       "Sequence InPort component",
    "version",           "1.0",
    "vendor",            "Noriaki Ando, AIST",
    "category",          "example",
    "activity_type",     "DataFlowComponent",
    "max_instance",      "10",
    "language",          "C++",
    "lang_type",         "compile",
    ""
  };
// </rtc-template>

SeqIn::SeqIn(RTC::Manager* manager)
  : RTC::DataFlowComponentBase(manager),
    // <rtc-template block="initializer">
    m_ShortIn("Short", m_Short),
    m_LongIn("Long", m_Long),
    m_FloatIn("Float", m_Float),
    m_DoubleIn("Double", m_Double),
    m_ShortSeqIn("ShortSeq", m_ShortSeq),
    m_LongSeqIn("LongSeq", m_LongSeq),
    m_FloatSeqIn("FloatSeq", m_FloatSeq),
    m_DoubleSeqIn("DoubleSeq", m_DoubleSeq)
    
    // </rtc-template>
{
}

SeqIn::~SeqIn()
{
}



RTC::ReturnCode_t SeqIn::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("Short", m_ShortIn);
  addInPort("Long", m_LongIn);
  addInPort("Float", m_FloatIn);
  addInPort("Double", m_DoubleIn);
  addInPort("ShortSeq", m_ShortSeqIn);
  addInPort("LongSeq", m_LongSeqIn);
  addInPort("FloatSeq", m_FloatSeqIn);
  addInPort("DoubleSeq", m_DoubleSeqIn);
  
  // check m_LongIn port only
  m_LongIn.addConnectorDataListener(ON_BUFFER_WRITE,
                                    new DataListener("ON_BUFFER_WRITE"));
  m_LongIn.addConnectorDataListener(ON_BUFFER_FULL, 
                                    new DataListener("ON_BUFFER_FULL"));
  m_LongIn.addConnectorDataListener(ON_BUFFER_WRITE_TIMEOUT, 
                                    new DataListener("ON_BUFFER_WRITE_TIMEOUT"));
  m_LongIn.addConnectorDataListener(ON_BUFFER_OVERWRITE, 
                                    new DataListener("ON_BUFFER_OVERWRITE"));
  m_LongIn.addConnectorDataListener(ON_BUFFER_READ, 
                                    new DataListener("ON_BUFFER_READ"));
  m_LongIn.addConnectorDataListener(ON_SEND, 
                                    new DataListener("ON_SEND"));
  m_LongIn.addConnectorDataListener(ON_RECEIVED,
                                    new DataListener("ON_RECEIVED"));
  m_LongIn.addConnectorDataListener(ON_RECEIVER_FULL, 
                                    new DataListener("ON_RECEIVER_FULL"));
  m_LongIn.addConnectorDataListener(ON_RECEIVER_TIMEOUT, 
                                    new DataListener("ON_RECEIVER_TIMEOUT"));

  m_LongIn.addConnectorListener(ON_BUFFER_EMPTY,
                                    new ConnListener("ON_BUFFER_EMPTY"));
  m_LongIn.addConnectorListener(ON_BUFFER_READ_TIMEOUT,
                                    new ConnListener("ON_BUFFER_READ_TIMEOUT"));
  m_LongIn.addConnectorListener(ON_SENDER_EMPTY,
                                    new ConnListener("ON_SENDER_EMPTY"));
  m_LongIn.addConnectorListener(ON_SENDER_TIMEOUT,
                                    new ConnListener("ON_SENDER_TIMEOUT"));
  m_LongIn.addConnectorListener(ON_SENDER_ERROR,
                                    new ConnListener("ON_SENDER_ERROR"));
  m_LongIn.addConnectorListener(ON_CONNECT,
                                    new ConnListener("ON_CONNECT"));
  m_LongIn.addConnectorListener(ON_DISCONNECT,
                                    new ConnListener("ON_DISCONNECT"));

  // Set OutPort buffer
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  return RTC::RTC_OK;
}


/*
RTC::ReturnCode_t SeqIn::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SeqIn::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SeqIn::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SeqIn::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SeqIn::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t SeqIn::onExecute(RTC::UniqueId ec_id)
{
  m_DoubleIn.read();
  m_FloatIn.read();
  m_LongIn.read();
  m_ShortIn.read();
  m_DoubleSeqIn.read();
  m_FloatSeqIn.read();
  m_LongSeqIn.read();
  m_ShortSeqIn.read();
  
  int h_col(4);
  int col(13);
  int all_col = h_col + (col * 4);
  int all_row(0);
  
  int d_size(1);
  int f_size(1);
  int l_size(1);
  int s_size(1);
  
  int ds_size(m_DoubleSeq.data.length());
  int fs_size(m_FloatSeq.data.length());
  int ls_size(m_LongSeq.data.length());
  int ss_size(m_ShortSeq.data.length());
  
  int max_size;
  
  std::vector<int> in_size;
  std::vector<int>::iterator it;
  in_size.reserve(8);
  in_size.push_back(d_size);
  in_size.push_back(f_size);
  in_size.push_back(l_size);
  in_size.push_back(s_size);
  in_size.push_back(ds_size);
  in_size.push_back(fs_size);
  in_size.push_back(ls_size);
  in_size.push_back(ss_size);
  
  it = std::max_element(in_size.begin(), in_size.end());
  
  max_size = *it;
  
  std::cout.width(h_col + (col * 4));
  std::cout.fill('-');
  std::cout << "-" << std::endl; all_row++;
  
  std::cout.width(h_col);
  std::cout.setf(std::ios::right, std::ios::adjustfield);
  std::cout.fill(' ');
  std::cout << " ";   
  std::cout.width(col);
  std::cout.setf(std::ios::right, std::ios::adjustfield);
  std::cout.fill(' ');
  std::cout << "Double";
  std::cout.width(col);
  std::cout.setf(std::ios::right, std::ios::adjustfield);
  std::cout.fill(' ');
  std::cout << "Float";
  std::cout.width(col);
  std::cout.setf(std::ios::right, std::ios::adjustfield);
  std::cout.fill(' ');
  std::cout << "Long";
  std::cout.width(col);
  std::cout.setf(std::ios::right, std::ios::adjustfield);
  std::cout.fill(' ');
  std::cout << "Short";
  std::cout << std::endl; all_row++;
  
  std::cout.width(all_col);
  std::cout.fill('-');
  std::cout << "-" << std::endl;  all_row++;
  
  for (int i = 0; i < (all_col - 10)/ 2; i++)
    std::cout << " ";
  std::cout << "Basic type";
  for (int i = 0; i < (all_col - 10)/ 2; i++)
    std::cout << " ";
  std::cout << std::endl; all_row++;
  
  std::cout.width(h_col + (col * 4));
  std::cout.fill('-');
  std::cout << "-" << std::endl; all_row++;
  
  std::cout.width(h_col);
  std::cout.setf(std::ios::right, std::ios::adjustfield);
  std::cout.fill(' ');
  std::cout << " ";   
  std::cout.width(col);
  std::cout.setf(std::ios::right, std::ios::adjustfield);
  std::cout.fill(' ');
  std::cout << m_Double.data;
  std::cout.width(col);
  std::cout.setf(std::ios::right, std::ios::adjustfield);
  std::cout.fill(' ');
  std::cout << m_Float.data;
  std::cout.width(col);
  std::cout.setf(std::ios::right, std::ios::adjustfield);
  std::cout.fill(' ');
  std::cout << m_Long.data;
  std::cout.width(col);
  std::cout.setf(std::ios::right, std::ios::adjustfield);
  std::cout.fill(' ');
  std::cout << m_Short.data;
  std::cout << std::endl; all_row++;
  
  std::cout.width(all_col);
  std::cout.fill('-');
  std::cout << "-" << std::endl; all_row++;
  
  for (int i = 0; i < (all_col - 13)/ 2; i++)
    std::cout << " ";
  std::cout << "Sequence type";
  for (int i = 0; i < (all_col - 13)/ 2; i++)
    std::cout << " ";
  std::cout << std::endl; all_row++;
  
  std::cout.width(all_col);
  std::cout.fill('-');
  std::cout << "-" << std::endl; all_row++;
  
  for (int i = 0; i < max_size; i++)
    {
      std::cout.width(h_col - 2);
      std::cout.setf(std::ios::right, std::ios::adjustfield);
      std::cout.fill(' ');
      std::cout << i;
      std::cout.width(2);
      std::cout << ": ";
      
      std::cout.width(col);
      std::cout.setf(std::ios::right, std::ios::adjustfield);
      std::cout.fill(' ');
      if (i < ds_size)
	{
	  std::cout << m_DoubleSeq.data[i];
	}
      else
	{
	  std::cout << "-";
	}
      
      std::cout.width(col);
      std::cout.setf(std::ios::right, std::ios::adjustfield);
      std::cout.fill(' ');
      if (i < fs_size)
	{
	  std::cout << m_FloatSeq.data[i];
	}
      else
	{
	  std::cout << "-";
	}
      
      std::cout.width(col);
      std::cout.setf(std::ios::right, std::ios::adjustfield);
      std::cout.fill(' ');
      if (i < ls_size)
	{
	  std::cout << m_LongSeq.data[i];
	}
      else
	{
	  std::cout << "-";
	}
      
      std::cout.width(col);
      std::cout.setf(std::ios::right, std::ios::adjustfield);
      std::cout.fill(' ');
      if (i < ss_size)
	{
	  std::cout << m_ShortSeq.data[i];
	}
      else
	{
	  std::cout << "-";
	}
      
      std::cout << std::endl; all_row++;
    }
  
  // Connector Listener Dump check
  if(!g_Listener_dump_enabled)
    {
      for (int i = 0 ; i < all_row; i++)
        {
          std::cout << "[A\r";
        }
    }

  return RTC::RTC_OK;
}


/*
RTC::ReturnCode_t SeqIn::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SeqIn::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SeqIn::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SeqIn::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SeqIn::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void SeqInInit(RTC::Manager* manager)
  {
    coil::Properties profile(seqin_spec);
    manager->registerFactory(profile,
                             RTC::Create<SeqIn>,
                             RTC::Delete<SeqIn>);
  }
  
};
