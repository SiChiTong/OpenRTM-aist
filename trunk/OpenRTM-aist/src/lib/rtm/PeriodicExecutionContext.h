// -*- C++ -*-
/*!
 * @file PeriodicExecutionContext.h
 * @brief PeriodicExecutionContext class
 * @date $Date: 2008-01-14 07:53:05 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2006-2008,2012
 *     Noriaki Ando
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id$
 *
 */

#ifndef RTC_PERIODICEXECUTIONCONTEXT2_H
#define RTC_PERIODICEXECUTIONCONTEXT2_H

#include <vector>
#include <iostream>

#include <coil/Task.h>
#include <coil/Mutex.h>
#include <coil/Condition.h>

#include <rtm/ExecutionContextBase.h>

#define NUM_OF_LIFECYCLESTATE 4

#ifdef WIN32
#pragma warning( disable : 4290 )
#endif

namespace RTC_exp
{
  /*!
   * @if jp
   * @class PeriodicExecutionContext
   * @brief PeriodicExecutionContext ���饹
   *
   * Periodic Sampled Data Processing(�����¹���)ExecutionContext���饹��
   *
   * @since 0.4.0
   *
   * @else
   * @class PeriodicExecutionContext
   * @brief PeriodicExecutionContext class
   *
   * Periodic Sampled Data Processing (for the execution cycles)
   * ExecutionContext class
   *
   * @since 0.4.0
   *
   * @endif
   */
  class PeriodicExecutionContext
    : public virtual POA_RTC::ExecutionContextService,
      public virtual PortableServer::RefCountServantBase,
      public RTC::ExecutionContextBase,
      public coil::Task
  {
    typedef coil::Guard<coil::Mutex> Guard;
  public:
    /*!
     * @if jp
     * @brief �ǥե���ȥ��󥹥ȥ饯��
     *
     * �ǥե���ȥ��󥹥ȥ饯��
     * �ץ��ե�����˰ʲ��ι��ܤ����ꤹ�롣
     *  - kind : PERIODIC
     *  - rate : 0.0
     *
     * @else
     * @brief Default Constructor
     *
     * Default Constructor
     * Set the following items to profile.
     *  - kind : PERIODIC
     *  - rate : 0.0
     *
     * @endif
     */
    PeriodicExecutionContext();

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
    virtual ~PeriodicExecutionContext(void);

    /*!
     * @if jp
     * @brief ExecutionContext�ν������Ԥ�
     *
     * ExecutionContext�ν��������
     *
     * @else
     * @brief Initialize the ExecutionContext
     *
     * This operation initialize the ExecutionContext
     *
     * @endif
     */
    virtual  void init(coil::Properties& props);

    /*!
     * @if jp
     * @brief ExecutionContext�ѥ����ƥ��ӥƥ�����åɤ���������
     *
     * Executioncontext �Ѥ����������ƥ��ӥƥ�����åɤ���������ư���롣
     * ����� coil::Task �����ӥ����饹�᥽�åɤΥ����С��饤�ɡ�
     *
     * @param args �̾��0
     *
     * @return ���������¹Է��
     *
     * @else
     *
     * @brief Generate internal activity thread for ExecutionContext
     *
     * Generate internal activity thread and run.  This is coil::Task
     * class method's override.
     *
     * @param args Usually give 0
     *
     * @return The generation result
     *
     * @endif
     */
    virtual int open(void *args);

    /*!
     * @if jp
     * @brief ExecutionContext �ѤΥ���åɼ¹Դؿ�
     *
     * ExecutionContext �ѤΥ���åɼ¹Դؿ�����Ͽ���줿����ݡ��ͥ��
     * �ν�����ƤӽФ���
     *
     * @return �¹Է��
     *
     * @else
     * @brief Thread execution function for ExecutionContext
     *
     * Thread execution function for ExecutionContext.  Invoke the
     * registered components operation.
     *
     * @return The execution result
     *
     * @endif
     */
    virtual int svc(void);

    /*!
     * @if jp
     * @brief ExecutionContext �ѤΥ���åɼ¹Դؿ�
     *
     * ExecutionContext �ѤΥ���åɽ�λ���˸ƤФ�롣����ݡ��ͥ�ȥ�
     * �֥������Ȥ��󥢥��ƥ��ֲ����ޥ͡�����ؤ����Τ�Ԥ��������
     * coil::Task �����ӥ����饹�᥽�åɤΥ����С��饤�ɡ�
     *
     * @param flags ��λ�����ե饰
     *
     * @return ��λ�������
     *
     * @else
     *
     * @brief Thread execution function for ExecutionContext
     *
     * This function is invoked when activity thread for
     * ExecutionContext exits.  Deactivate the component object and
     * notify it to manager.  This is coil::Task class method's
     * override.
     *
     * @param flags Flag of the close
     *
     * @return The close result
     *
     * @endif
     */
    virtual int close(unsigned long flags);

    //============================================================
    // ExecutionContext
    //============================================================
    /*!
     * @if jp
     * @brief ExecutionContext �¹Ծ��ֳ�ǧ�ؿ�
     *
     * �������� ExecutionContext �� Runnning ���֤ξ��� true ���֤���
     * Executioncontext �� Running �δ֡����� Executioncontext �˻��ä�
     * �Ƥ������ƤΥ����ƥ���RT����ݡ��ͥ�Ȥ���ExecutionContext �μ�
     * �Լ���˱����Ƽ¹Ԥ���롣
     *
     * @return ���ֳ�ǧ�ؿ�(ư����:true�������:false)
     *
     * @else
     *
     * @brief Check for ExecutionContext running state
     *
     * This operation shall return true if the context is in the
     * Running state.  While the context is Running, all Active RTCs
     * participating in the context shall be executed according to the
     * context��s execution kind.
     *
     * @return Check state function (Running:true��Stopping:false)
     *
     * @endif
     */
    virtual CORBA::Boolean is_running(void)
      throw (CORBA::SystemException);

    /*!
     * @if jp
     * @brief ExecutionContext �μ¹Ԥ򳫻�
     *
     * ExecutionContext �μ¹Ծ��֤� Runnning �Ȥ��뤿��Υꥯ�����Ȥ�
     * ȯ�Ԥ��롣ExecutionContext �ξ��֤����ܤ����
     * ComponentAction::on_startup ���ƤӽФ���롣���ä��Ƥ���RT����ݡ�
     * �ͥ�Ȥ�������������ޤ� ExecutionContext �򳫻Ϥ��뤳�ȤϤǤ�
     * �ʤ���ExecutionContext ��ʣ���󳫻�/��ߤ򷫤��֤����Ȥ��Ǥ��롣
     *
     * @return ReturnCode_t ���Υ꥿���󥳡���
     *
     * @else
     *
     * @brief Start the ExecutionContext
     *
     * Request that the context enter the Running state.  Once the
     * state transition occurs, the ComponentAction::on_startup
     * operation will be invoked.  An execution context may not be
     * started until the RT-Components that participate in it have
     * been initialized.  An execution context may be started and
     * stopped multiple times.
     *
     * @return The return code of ReturnCode_t type
     *
     * @endif
     */
    virtual RTC::ReturnCode_t start(void)
      throw (CORBA::SystemException);

    /*!
     * @if jp
     * @brief ExecutionContext �μ¹Ԥ����
     *
     * ExecutionContext �ξ��֤� Stopped �Ȥ��뤿��Υꥯ�����Ȥ�ȯ�Ԥ�
     * �롣���ܤ�ȯ���������ϡ�ComponentAction::on_shutdown ���Ƥӽ�
     * ����롣���ä��Ƥ���RT����ݡ��ͥ�Ȥ���λ��������
     * ExecutionContext ����ߤ���ɬ�פ����롣ExecutionContext ��ʣ����
     * ����/��ߤ򷫤��֤����Ȥ��Ǥ��롣
     *
     * @return ReturnCode_t ���Υ꥿���󥳡���
     *
     * @else
     *
     * @brief Stop the ExecutionContext
     *
     * Request that the context enter the Stopped state.  Once the
     * transition occurs, the ComponentAction::on_shutdown operation
     * will be invoked.  An execution context must be stopped before
     * the RT components that participate in it are finalized.  An
     * execution context may be started and stopped multiple times.
     *
     * @return The return code of ReturnCode_t type
     *
     * @endif
     */
    virtual RTC::ReturnCode_t stop(void)
      throw (CORBA::SystemException);

    /*!
     * @if jp
     * @brief ExecutionContext �μ¹Լ���(Hz)���������
     *
     * Active ���֤ˤ�RT����ݡ��ͥ�Ȥ��¹Ԥ�������(ñ��:Hz)�������
     * �롣
     *
     * @return ��������(ñ��:Hz)
     *
     * @else
     *
     * @brief Get execution rate(Hz) of ExecutionContext
     *
     * This operation shall return the rate (in hertz) at which its
     * Active participating RTCs are being invoked.
     *
     * @return Execution cycle(Unit:Hz)
     *
     * @endif
     */
    virtual CORBA::Double get_rate(void)
      throw (CORBA::SystemException);

    /*!
     * @if jp
     * @brief ExecutionContext �μ¹Լ���(Hz)�����ꤹ��
     *
     * Active ���֤ˤ�RT����ݡ��ͥ�Ȥ��¹Ԥ�������(ñ��:Hz)�����ꤹ
     * �롣�¹Լ������ѹ��ϡ�DataFlowComponentAction ��
     * on_rate_changed �ˤ�äƳ�RT����ݡ��ͥ�Ȥ���ã����롣
     *
     * @param rate ��������(ñ��:Hz)
     *
     * @return ReturnCode_t ���Υ꥿���󥳡���
     *
     * @else
     *
     * @brief Set execution rate(Hz) of ExecutionContext
     *
     * This operation shall set the rate (in hertz) at which this
     * context��s Active participating RTCs are being called.  If the
     * execution kind of the context is PERIODIC, a rate change shall
     * result in the invocation of on_rate_changed on any RTCs
     * realizing DataFlowComponentAction that are registered with any
     * RTCs participating in the context.
     *
     * @param rate Execution cycle(Unit:Hz)
     *
     * @return The return code of ReturnCode_t type
     *
     * @endif
     */
    virtual RTC::ReturnCode_t  set_rate(CORBA::Double rate)
      throw (CORBA::SystemException);

    /*!
     * @if jp
     * @brief RT����ݡ��ͥ�Ȥ򥢥��ƥ��ֲ�����
     *
     * Inactive ���֤ˤ���RT����ݡ��ͥ�Ȥ�Active �����ܤ����������ƥ�
     * �ֲ����롣�������ƤФ줿��̡�on_activate ���ƤӽФ���롣��
     * �ꤷ��RT����ݡ��ͥ�Ȥ����üԥꥹ�Ȥ˴ޤޤ�ʤ����ϡ�
     * BAD_PARAMETER ���֤���롣���ꤷ��RT����ݡ��ͥ�Ȥξ��֤�
     * Inactive �ʳ��ξ��ϡ�PRECONDITION_NOT_MET ���֤���롣
     *
     * @param comp �����ƥ��ֲ��о�RT����ݡ��ͥ��
     *
     * @return ReturnCode_t ���Υ꥿���󥳡���
     *
     * @else
     *
     * @brief Activate an RT-component
     *
     * The given participant RTC is Inactive and is therefore not
     * being invoked according to the execution context��s execution
     * kind. This operation shall cause the RTC to transition to the
     * Active state such that it may subsequently be invoked in this
     * execution context.  The callback on_activate shall be called as
     * a result of calling this operation. This operation shall not
     * return until the callback has returned, and shall result in an
     * error if the callback does.
     *
     * @param comp The target RT-Component for activation
     *
     * @return The return code of ReturnCode_t type
     *
     * @endif
     */
    virtual RTC::ReturnCode_t
    activate_component(RTC::LightweightRTObject_ptr comp)
      throw (CORBA::SystemException);
    
    /*!
     * @if jp
     * @brief RT����ݡ��ͥ�Ȥ��󥢥��ƥ��ֲ�����
     *
     * Inactive ���֤ˤ���RT����ݡ��ͥ�Ȥ��󥢥��ƥ��ֲ�����Inactive
     * �����ܤ����롣�������ƤФ줿��̡�on_deactivate ���ƤӽФ���
     * �롣���ꤷ��RT����ݡ��ͥ�Ȥ����üԥꥹ�Ȥ˴ޤޤ�ʤ����ϡ�
     * BAD_PARAMETER ���֤���롣���ꤷ��RT����ݡ��ͥ�Ȥξ��֤�
     * Active �ʳ��ξ��ϡ�PRECONDITION_NOT_MET ���֤���롣
     *
     * @param comp �󥢥��ƥ��ֲ��о�RT����ݡ��ͥ��
     *
     * @return ReturnCode_t ���Υ꥿���󥳡���
     *
     * @else
     *
     * @brief Deactivate an RT-component
     *
     * The given RTC is Active in the execution context. Cause it to
     * transition to the Inactive state such that it will not be
     * subsequently invoked from the context unless and until it is
     * activated again.  The callback on_deactivate shall be called as
     * a result of calling this operation. This operation shall not
     * return until the callback has returned, and shall result in an
     * error if the callback does.
     *
     * @param comp The target RT-Component for deactivate
     *
     * @return The return code of ReturnCode_t type
     *
     * @endif
     */
    virtual RTC::ReturnCode_t
    deactivate_component(RTC::LightweightRTObject_ptr comp)
      throw (CORBA::SystemException);

    /*!
     * @if jp
     * @brief RT����ݡ��ͥ�Ȥ�ꥻ�åȤ���
     *
     * Error ���֤�RT����ݡ��ͥ�Ȥ��������ߤ롣�������ƤФ줿��
     * �̡�on_reset ���ƤӽФ���롣���ꤷ��RT����ݡ��ͥ�Ȥ����üԥ�
     * ���Ȥ˴ޤޤ�ʤ����ϡ�BAD_PARAMETER ���֤���롣���ꤷ��RT����
     * �ݡ��ͥ�Ȥξ��֤� Error �ʳ��ξ��ϡ�PRECONDITION_NOT_MET ����
     * ����롣
     *
     * @param comp �ꥻ�å��о�RT����ݡ��ͥ��
     *
     * @return ReturnCode_t ���Υ꥿���󥳡���
     *
     * @else
     *
     * @brief Reset the RT-component
     *
     * Attempt to recover the RTC when it is in Error.  The
     * ComponentAction::on_reset callback shall be invoked. This
     * operation shall not return until the callback has returned, and
     * shall result in an error if the callback does. If possible, the
     * RTC developer should implement that callback such that the RTC
     * may be returned to a valid state.
     *
     * @param comp The target RT-Component for reset
     *
     * @return The return code of ReturnCode_t type
     *
     * @endif
     */
    virtual RTC::ReturnCode_t
    reset_component(RTC::LightweightRTObject_ptr comp)
      throw (CORBA::SystemException);

    /*!
     * @if jp
     * @brief RT����ݡ��ͥ�Ȥξ��֤��������
     *
     * ���ꤷ��RT����ݡ��ͥ�Ȥξ���(LifeCycleState)��������롣���ꤷ
     * ��RT����ݡ��ͥ�Ȥ����üԥꥹ�Ȥ˴ޤޤ�ʤ����ϡ�
     * UNKNOWN_STATE ���֤���롣
     *
     * @param comp ���ּ����о�RT����ݡ��ͥ��
     *
     * @return ���ߤξ���(LifeCycleState)
     *
     * @else
     *
     * @brief Get RT-component's state
     *
     * This operation shall report the LifeCycleState of the given
     * participant RTC.  UNKNOWN_STATE will be returned, if the given
     * RT-Component is not inclued in the participant list.
     *
     * @param comp The target RT-Component to get the state
     *
     * @return The current state of the target RT-Component(LifeCycleState)
     *
     * @endif
     */
    virtual RTC::LifeCycleState
    get_component_state(RTC::LightweightRTObject_ptr comp)
      throw (CORBA::SystemException);

    /*!
     * @if jp
     * @brief ExecutionKind ���������
     *
     * �� ExecutionContext �� ExecutionKind ���������
     *
     * @return ExecutionKind
     *
     * @else
     *
     * @brief Get the ExecutionKind
     *
     * This operation shall report the execution kind of the execution
     * context.
     *
     * @return ExecutionKind
     *
     * @endif
     */
    virtual RTC::ExecutionKind get_kind(void)
      throw (CORBA::SystemException);

    /*!
     * @if jp
     * @brief RT����ݡ��ͥ�Ȥ��ɲä���
     *
     * ���ꤷ��RT����ݡ��ͥ�Ȥ򻲲üԥꥹ�Ȥ��ɲä��롣�ɲä��줿RT��
     * ��ݡ��ͥ�Ȥ� attach_context ���ƤФ졢Inactive ���֤����ܤ��롣
     * ���ꤵ�줿RT����ݡ��ͥ�Ȥ�null�ξ��ϡ�BAD_PARAMETER ���֤���
     * �롣���ꤵ�줿RT����ݡ��ͥ�Ȥ� DataFlowComponent �ʳ��ξ��ϡ�
     * BAD_PARAMETER ���֤���롣
     *
     * @param comp �ɲ��о�RT����ݡ��ͥ��
     *
     * @return ReturnCode_t ���Υ꥿���󥳡���
     *
     * @else
     *
     * @brief Add an RT-component
     *
     * The operation causes the given RTC to begin participating in
     * the execution context.  The newly added RTC will receive a call
     * to LightweightRTComponent::attach_context and then enter the
     * Inactive state.  BAD_PARAMETER will be invoked, if the given
     * RT-Component is null or if the given RT-Component is other than
     * DataFlowComponent.
     *
     * @param comp The target RT-Component for add
     *
     * @return The return code of ReturnCode_t type
     *
     * @endif
     */
    virtual RTC::ReturnCode_t add_component(RTC::LightweightRTObject_ptr comp)
      throw (CORBA::SystemException);

    /*!
     * @if jp
     * @brief RT����ݡ��ͥ�Ȥ򻲲üԥꥹ�Ȥ���������
     *
     * ���ꤷ��RT����ݡ��ͥ�Ȥ򻲲üԥꥹ�Ȥ��������롣������줿
     * RT����ݡ��ͥ�Ȥ� detach_context ���ƤФ�롣���ꤵ�줿RT����ݡ�
     * �ͥ�Ȥ����üԥꥹ�Ȥ���Ͽ����Ƥ��ʤ����ϡ�BAD_PARAMETER ����
     * ����롣
     *
     * @param comp ����о�RT����ݡ��ͥ��
     *
     * @return ReturnCode_t ���Υ꥿���󥳡���
     *
     * @else
     *
     * @brief Remove the RT-Component from participant list
     *
     * This operation causes a participant RTC to stop participating in the
     * execution context.
     * The removed RTC will receive a call to
     * LightweightRTComponent::detach_context.
     * BAD_PARAMETER will be returned, if the given RT-Component is not
     * participating in the participant list.
     *
     * @param comp The target RT-Component for delete
     *
     * @return The return code of ReturnCode_t type
     *
     * @endif
     */
    virtual RTC::ReturnCode_t
    remove_component(RTC::LightweightRTObject_ptr comp)
      throw (CORBA::SystemException);

    /*!
     * @if jp
     * @brief ExecutionContextProfile ���������
     *
     * �� ExecutionContext �Υץ��ե������������롣
     *
     * @return ExecutionContextProfile
     *
     * @else
     *
     * @brief Get the ExecutionContextProfile
     *
     * This operation provides a profile ��descriptor�� for the execution
     * context.
     *
     * @return ExecutionContextProfile
     *
     * @endif
     */
    virtual RTC::ExecutionContextProfile* get_profile(void)
      throw (CORBA::SystemException);

  protected:
    template <class T>
    void getProperty(coil::Properties& prop, const char* key, T& value)
    {
      if (prop.findNode(key) != 0)
        {
          T tmp;
          if (coil::stringTo(tmp, prop[key].c_str()))
            {
              value = tmp;
            }
        }
    }
    /*!
     * @brief onStarted() template function
     */
    virtual RTC::ReturnCode_t onStarted();
    /*!
     * @brief onStopping() template function
     */
    virtual RTC::ReturnCode_t onStopping();
    // template virtual functions adding/removing component
    /*!
     * @brief onAddedComponent() template function
     */
     virtual RTC::ReturnCode_t
     onAddedComponent(RTC::LightweightRTObject_ptr rtobj);
    /*!
     * @brief onRemovedComponent() template function
     */
    virtual RTC::ReturnCode_t
    onRemovedComponent(RTC::LightweightRTObject_ptr rtobj);

    /*!
     * @brief onWaitingActivated() template function
     */
    virtual RTC::ReturnCode_t
    onWaitingActivated(RTC_impl::RTObjectStateMachine* comp, long int count);
    /*!
     * @brief onActivated() template function
     */
    virtual RTC::ReturnCode_t
    onActivated(RTC_impl::RTObjectStateMachine* comp, long int count);
    /*!
     * @brief onWaitingDeactivated() template function
     */
    virtual RTC::ReturnCode_t
    onWaitingDeactivated(RTC_impl::RTObjectStateMachine* comp, long int count);
    /*!
     * @brief onDeactivated() template function
     */
    virtual RTC::ReturnCode_t 
    onDeactivated(RTC_impl::RTObjectStateMachine* comp, long int count);
    /*!
     * @brief onWaitingReset() template function
     */
    virtual RTC::ReturnCode_t
    onWaitingReset(RTC_impl::RTObjectStateMachine* comp, long int count);
    /*!
     * @brief onReset() template function
     */
    virtual RTC::ReturnCode_t 
    onReset(RTC_impl::RTObjectStateMachine* comp, long int count);

    /*!
     * @brief setting CPU affinity from given properties
     */
    virtual void setCpuAffinity(coil::Properties& props);

    bool threadRunning()
    {
      Guard guard(m_svcmutex);
      return m_svc;
    }
  protected:
    /*!
     * @if jp
     * @brief ���������ȥ꡼��
     * @else
     * @brief Logger stream
     * @endif
     */
    RTC::Logger rtclog;

    /*!
     * @if jp
     * @brief ExecutionContext �Υ���åɼ¹ԥե饰
     * @else
     * @brief The thread running flag of ExecutionContext
     * @endif
     */
    bool m_svc;
    coil::Mutex m_svcmutex;
    
    /*!
     * @if jp
     * @brief worker �Ѿ����ѿ����饹
     * @else
     * @brief Condition variable class for worker
     * @endif
     */
    struct WorkerThreadCtrl
    {
      WorkerThreadCtrl() : cond_(mutex_), running_(false) {};
      coil::Mutex mutex_;
      coil::Condition<coil::Mutex> cond_;
      bool running_;
    };

    /*!
     * @if jp
     * @brief svn�Ѥξ����ѿ� 
     * @else
     * @brief A condition variable for external triggered worker
     * @endif
     */
    WorkerThreadCtrl m_workerthread;

    /*!
     * @if jp
     * @brief ExecutionContext ¨���¹�(wait̵���¹�)�ե饰
     * @else
     * @brief Flag of ExecutionContext to run immediately
     *        (to run without waiting)
     * @endif
     */
    bool m_nowait;

    /*!
     * @brief CPU affinity mask list
     */
    std::vector<int> m_cpu;

  }; // class PeriodicExecutionContext
}; // namespace RTC

#ifdef WIN32
#pragma warning( default : 4290 )
#endif


extern "C"
{
  /*!
   * @if jp
   * @brief ECFactory�ؤ���Ͽ�Τ���ν�����ؿ�
   * @else
   * @brief Initialization function to register to ECFactory
   * @endif
   */
  void PeriodicExecutionContextInit(RTC::Manager* manager);
};

#endif // RTC_PERIODICEXECUTIONCONTEXT_H