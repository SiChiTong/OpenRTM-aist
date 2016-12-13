// -*- C++ -*-
/*!
 * @file  SeqOut.h
 * @brief Sequence OutPort component
 * @date  $Date: 2007-02-04 16:48:05 $
 *
 * $Id: SeqOut.h,v 1.2 2007-02-04 16:48:05 n-ando Exp $
 */

#ifndef SEQOUT_H
#define SEQOUT_H

#include <rtm/Manager.h>
#include <rtm/DataFlowComponentBase.h>
#include <rtm/CorbaPort.h>
#include <rtm/DataInPort.h>
#include <rtm/DataOutPort.h>
#include <rtm/idl/BasicDataTypeSkel.h>

// Service implementation headers
// <rtc-template block="service_impl_h">

// </rtc-template>

// Service Consumer stub headers
// <rtc-template block="consumer_stub_h">

// </rtc-template>

using namespace RTC;

class SeqOut
  : public RTC::DataFlowComponentBase
{
 public:
  SeqOut(RTC::Manager* manager);
  ~SeqOut();

  // The initialize action (on CREATED->ALIVE transition)
  // formaer rtc_init_entry() 
  // virtual RTC::ReturnCode_t onInitialize();

  // The finalize action (on ALIVE->END transition)
  // formaer rtc_exiting_entry()
  // virtual RTC::ReturnCode_t onFinalize();

  // The startup action when ExecutionContext startup
  // former rtc_starting_entry()
  // virtual RTC::ReturnCode_t onStartup(RTC::UniqueId ec_id);

  // The shutdown action when ExecutionContext stop
  // former rtc_stopping_entry()
  // virtual RTC::ReturnCode_t onShutdown(RTC::UniqueId ec_id);

  // The activated action (Active state entry action)
  // former rtc_active_entry()
  // virtual RTC::ReturnCode_t onActivated(RTC::UniqueId ec_id);

  // The deactivated action (Active state exit action)
  // former rtc_active_exit()
  // virtual RTC::ReturnCode_t onDeactivated(RTC::UniqueId ec_id);

  // The execution action that is invoked periodically
  // former rtc_active_do()
  virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

  // The aborting action when main logic error occurred.
  // former rtc_aborting_entry()
  // virtual RTC::ReturnCode_t onAborting(RTC::UniqueId ec_id);

  // The error action in ERROR state
  // former rtc_error_do()
  // virtual RTC::ReturnCode_t onError(RTC::UniqueId ec_id);

  // The reset action that is invoked resetting
  // This is same but different the former rtc_init_entry()
  // virtual RTC::ReturnCode_t onReset(RTC::UniqueId ec_id);
  
  // The state update action that is invoked after onExecute() action
  // no corresponding operation exists in OpenRTm-aist-0.2.0
  // virtual RTC::ReturnCode_t onStateUpdate(RTC::UniqueId ec_id);

  // The action that is invoked when execution context's rate is changed
  // no corresponding operation exists in OpenRTm-aist-0.2.0
  // virtual RTC::ReturnCode_t onRateChanged(RTC::UniqueId ec_id);


 protected:
  // DataInPort declaration
  // <rtc-template block="inport_declare">
  
  // </rtc-template>


  // DataOutPort declaration
  // <rtc-template block="outport_declare">
  TimedShort m_Short;
  OutPort<TimedShort> m_ShortOut;
  TimedLong m_Long;
  OutPort<TimedLong> m_LongOut;
  TimedFloat m_Float;
  OutPort<TimedFloat> m_FloatOut;
  TimedDouble m_Double;
  OutPort<TimedDouble> m_DoubleOut;
  TimedShortSeq m_ShortSeq;
  OutPort<TimedShortSeq> m_ShortSeqOut;
  TimedLongSeq m_LongSeq;
  OutPort<TimedLongSeq> m_LongSeqOut;
  TimedFloatSeq m_FloatSeq;
  OutPort<TimedFloatSeq> m_FloatSeqOut;
  TimedDoubleSeq m_DoubleSeq;
  OutPort<TimedDoubleSeq> m_DoubleSeqOut;
  
  // </rtc-template>

  // CORBA Port declaration
  // <rtc-template block="corbaport_declare">
  
  // </rtc-template>

  // Service declaration
  // <rtc-template block="service_declare">
  
  // </rtc-template>

  // Consumer declaration
  // <rtc-template block="consumer_declare">
  
  // </rtc-template>

 private:
  int dummy;

};


extern "C"
{
  void SeqOutInit(RTC::Manager* manager);
};

#endif // SEQOUT_H