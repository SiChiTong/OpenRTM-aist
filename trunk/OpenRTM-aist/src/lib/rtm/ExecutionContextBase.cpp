// -*- C++ -*-
/*!
 * @file ExecutionContextBase.cpp
 * @brief ExecutionContext base class
 * @date $Date$
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2011-2012
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id$
 *
 */

#include <rtm/NVUtil.h>
#include <rtm/RTObjectStateMachine.h>
#include <rtm/ExecutionContextBase.h>

namespace RTC
{
  ExecutionContextBase::ExecutionContextBase(const char* name)
    : rtclog("ec_base"),
      m_activationTimeout(0.5),
      m_deactivationTimeout(0.5),
      m_resetTimeout(0.5),
      m_syncActivation(true), m_syncDeactivation(true), m_syncReset(true)
  {
  }
  /*!
   * @if jp
   * @brief ���ۥǥ��ȥ饯��
   * @else
   * @brief Virtual Destructor
   * @endif
   */
  ExecutionContextBase::~ExecutionContextBase(void)
  {
  }
  
  /*!
   * @if jp
   * @brief ExecutionContext�ν�����ʤ��
   * @else
   * @brief Proceed with tick of ExecutionContext
   * @endif
   */
  void ExecutionContextBase::init(coil::Properties& props)
  {
    RTC_TRACE(("init()"));
    RTC_DEBUG_STR((props));
    
    // getting rate
    setExecutionRate(props);
    
    // getting sync/async mode flag
    bool transitionMode;
    if (setTransitionMode(props, "sync_transition", transitionMode))
      {
        m_syncActivation   = transitionMode;
        m_syncDeactivation = transitionMode;
        m_syncReset        = transitionMode;
      }
    setTransitionMode(props, "sync_activation", m_syncActivation);
    setTransitionMode(props, "sync_deactivation", m_syncDeactivation);
    setTransitionMode(props, "sync_reset", m_syncReset);
    
    // getting transition timeout
    coil::TimeValue timeout;
    if (setTimeout(props, "transition_timeout", timeout))
      {
        m_activationTimeout   = timeout;
        m_deactivationTimeout = timeout;
        m_resetTimeout        = timeout;
      }
    setTimeout(props, "activation_timeout",   m_activationTimeout);
    setTimeout(props, "deactivation_timeout", m_deactivationTimeout);
    setTimeout(props, "reset_timeout",        m_resetTimeout);

    RTC_DEBUG(("ExecutionContext's configurations:"));
    RTC_DEBUG(("Exec rate   : %f [Hz]", getRate()));
    RTC_DEBUG(("Activation  : Sync = %s, Timeout = %f",
               m_syncActivation ? "YES" : "NO",
               (double)m_activationTimeout));
    RTC_DEBUG(("Deactivation: Sync = %s, Timeout = %f",
               m_syncDeactivation ? "YES" : "NO",
               (double)m_deactivationTimeout));
    RTC_DEBUG(("Reset       : Sync = %s, Timeout = %f",
               m_syncReset ? "YES" : "NO",
               (double)m_resetTimeout));
    // Setting given Properties to EC's profile::properties
    setProperties(props);
  }
  
  /*!
   * @if jp
   * @brief ����ݡ��ͥ�Ȥ�Х���ɤ��롣
   *
   * ����ݡ��ͥ�Ȥ�Х���ɤ��롣
   *
   * @else
   * @brief Bind the component.
   *
   * Bind the component.
   *
   * @endif
   */
  RTC::ReturnCode_t ExecutionContextBase::
  bindComponent(RTC::RTObject_impl* rtc)
  {
    return m_worker.bindComponent(rtc);
  }
  
  //============================================================
  // Functions to be delegated by EC's CORBA operations
  /*!
   * @if jp
   * @brief ExecutionContext �¹Ծ��ֳ�ǧ�ؿ�
   * @else
   * @brief Check for ExecutionContext running state
   * @endif
   */
  CORBA::Boolean ExecutionContextBase::isRunning()
  {
    RTC_TRACE(("isRunning()"));
    return m_worker.isRunning();
  }
  
  /*!
   * @if jp
   * @brief ExecutionContext �μ¹Ԥ򳫻�
   * @else
   * @brief Start the ExecutionContext
   * @endif
   */
  RTC::ReturnCode_t ExecutionContextBase::start()
  {
    RTC_TRACE(("start()"));
    RTC::ReturnCode_t ret = onStarting(); // Template
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("onStarting() failed. Starting EC aborted."));
        return ret;
      }
    ret = m_worker.start(); // Actual start()
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("Invoking on_startup() for each RTC failed."));
        return ret;
      }
    ret = onStarted(); // Template
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("Invoking on_startup() for each RTC failed."));
        m_worker.stop();
        RTC_ERROR(("on_shutdown() was invoked, because of on_startup"));
        return ret;
      }
    return ret;
  }
  
  /*!
   * @if jp
   * @brief ExecutionContext �μ¹Ԥ����
   * @else
   * @brief Stopping the ExecutionContext
   * @endif
   */
  RTC::ReturnCode_t ExecutionContextBase::stop()
  {
    RTC_TRACE(("stop()"));
    RTC::ReturnCode_t ret = onStopping(); // Template
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("onStopping() failed. Stopping EC aborted."));
        return ret;
      }
    ret = m_worker.stop(); // Actual stop()
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("Invoking on_shutdown() for each RTC failed."));
        return ret;
      }
    ret = onStopped(); // Template
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("Invoking on_shutdown() for each RTC failed."));
        return ret;
      }
    return ret;
  }
  
  /*!
   * @if jp
   * @brief ExecutionContext �μ¹Լ���(Hz)���������
   * @else
   * @brief Get execution rate(Hz) of ExecutionContext
   * @return Execution cycle(Unit:Hz)
   * @endif
   */
  double ExecutionContextBase::getRate(void) const
  {
    double rate = m_profile.getRate(); // Actual getRate()
    return onGetRate(rate); // Template
  }
  
  coil::TimeValue ExecutionContextBase::getPeriod(void) const
  {
    coil::TimeValue period = m_profile.getPeriod();
    return period;
  }
  
  /*!
   * @if jp
   * @brief ExecutionContext �μ¹Լ���(Hz)�����ꤹ��
   * @else
   * @brief Set execution rate(Hz) of ExecutionContext
   * @endif
   */
  RTC::ReturnCode_t ExecutionContextBase::setRate(double rate)
  {
    RTC_TRACE(("setRate(%f)", rate));
    RTC::ReturnCode_t ret = m_profile.setRate(onSettingRate(rate));
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("Setting execution rate failed. %f", rate));
        return ret;
      }
    ret = m_worker.rateChanged();
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("Invoking on_rate_changed() for each RTC failed."));
        return ret;
      }
    ret = onSetRate(rate);
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("onSetRate(%f) failed.", rate));
        return ret;
      }
    RTC_INFO(("setRate(%f) done", rate));
    return ret;
  }
  
  /*!
   * @if jp
   * @brief RT����ݡ��ͥ�Ȥ��ɲä���
   * @else
   * @brief Add an RT-component
   * @endif
   */
  RTC::ReturnCode_t ExecutionContextBase::
  addComponent(RTC::LightweightRTObject_ptr comp)
  {
    RTC_TRACE(("addComponent()"));
    RTC::ReturnCode_t ret = onAddingComponent(comp); // Template
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("Error: onAddingComponent(). RTC is not attached."));
        return ret;
      }
    ret = m_worker.addComponent(comp); // Actual addComponent()
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("Error: ECWorker addComponent() faild."));
        return ret;
      }
    ret = m_profile.addComponent(comp); // Actual addComponent()
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("Error: ECProfile addComponent() faild."));
        return ret;
      }
    ret = onAddedComponent(comp); // Template
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("Error: onAddedComponent() faild."));
        RTC_INFO(("Removing attached RTC."));
        m_worker.removeComponent(comp);
        m_profile.removeComponent(comp);
        return ret;
      }
    RTC_INFO(("Component has been added to this EC."));
    return RTC::RTC_OK;
  }
  
  /*!
   * @if jp
   * @brief RT����ݡ��ͥ�Ȥ򻲲üԥꥹ�Ȥ���������
   * @else
   * @brief Remove the RT-Component from participant list
   * @endif
   */
  RTC::ReturnCode_t ExecutionContextBase::
  removeComponent(RTC::LightweightRTObject_ptr comp)
  {
    RTC_TRACE(("removeComponent()"));
    RTC::ReturnCode_t ret = onRemovingComponent(comp); // Template
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("Error: onRemovingComponent(). "
                   "RTC will not not attached."));
        return ret;
      }
    ret = m_worker.removeComponent(comp); // Actual removeComponent()
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("Error: ECWorker removeComponent() faild."));
        return ret;
      }
    ret = m_profile.removeComponent(comp); // Actual removeComponent()
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("Error: ECProfile removeComponent() faild."));
        return ret;
      }
    ret = onRemovedComponent(comp); // Template
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("Error: onRemovedComponent() faild."));
        RTC_INFO(("Removing attached RTC."));
        m_worker.removeComponent(comp);
        m_profile.removeComponent(comp);
        return ret;
      }
    RTC_INFO(("Component has been removeed to this EC."));
    return RTC::RTC_OK;
  }
  
  /*!
   * @if jp
   * @brief RT����ݡ��ͥ�Ȥ򥢥��ƥ��ֲ�����
   * @else
   * @brief Activate an RT-component
   * @endif
   */
  RTC::ReturnCode_t ExecutionContextBase::
  activateComponent(RTC::LightweightRTObject_ptr comp)
  {
    RTC_TRACE(("activateComponent()"));
    RTC::ReturnCode_t ret = onActivating(comp); // Template
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("onActivating() failed."));
        return ret;
      }
    RTC_impl::RTObjectStateMachine* rtobj;
    ret = m_worker.activateComponent(comp, rtobj); // Actual activateComponent()
    if (ret != RTC::RTC_OK) { return ret; }
    if (!m_syncActivation) // Asynchronous activation mode
      {
        ret = onActivated(rtobj, -1);
        if (ret != RTC::RTC_OK)
          {
            RTC_ERROR(("onActivated() failed."));
          }
        return ret;
      }
    //------------------------------------------------------------
    // Synchronized activation mode
    RTC_DEBUG(("Synchronous activation mode. "
               "Waiting for the RTC to be ACTIVE state. "));
    return waitForActivated(rtobj);
  }

  RTC::ReturnCode_t ExecutionContextBase::
  waitForActivated(RTC_impl::RTObjectStateMachine* rtobj)
  {
    long int count(0);
    RTC::ReturnCode_t ret = onWaitingActivated(rtobj, count);
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("onWaitingActivated failed."));
        return ret;
      }
    long int cycle =
      (long int)((double)m_activationTimeout / (double)getPeriod());
    RTC_DEBUG(("Timeout is %f [s] (%f [s] in %d times)",
               (double)m_activationTimeout, getRate(), cycle));
    // Wating INACTIVE -> ACTIVE
    coil::TimeValue starttime(coil::gettimeofday());
    while (rtobj->isCurrentState(RTC::INACTIVE_STATE))
      {
        ret = onWaitingActivated(rtobj, count); // Template method
        if (ret != RTC::RTC_OK)
          {
            RTC_ERROR(("onWaitingActivated failed."));
            return ret;
          }
        coil::sleep(getPeriod());
        coil::TimeValue delta(coil::gettimeofday() - starttime);
        RTC_DEBUG(("Waiting to be ACTIVE state. %f [s] slept (%d/%d)",
                   (double)delta, count, cycle));
        ++count;
        if (delta > m_activationTimeout || count > cycle)
          {
            RTC_WARN(("The component is not responding."));
            break;
          }
      }
    // Now State must be ACTIVE or ERROR
    if (rtobj->isCurrentState(RTC::INACTIVE_STATE))
      {
        RTC_ERROR(("Unknown error: Invalid state transition."));
        return RTC::RTC_ERROR;
      }
    RTC_DEBUG(("Current state is %s", getStateString(rtobj->getState())));
    ret = onActivated(rtobj, count); // Template method
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("onActivated() failed."));
      }
    RTC_DEBUG(("onActivated() done."))
    return ret;
  }

  /*!
   * @if jp
   * @brief RT����ݡ��ͥ�Ȥ��󥢥��ƥ��ֲ�����
   * @else
   * @brief Deactivate an RT-component
   * @endif
   */
  RTC::ReturnCode_t ExecutionContextBase::
  deactivateComponent(RTC::LightweightRTObject_ptr comp)
  {
    RTC_TRACE(("deactivateComponent()"));
    RTC::ReturnCode_t ret = onDeactivating(comp); // Template
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("onDeactivatingComponent() failed."));
        return ret;
      }
    // Deactivate all the RTCs
    RTC_impl::RTObjectStateMachine* rtobj;
    ret = m_worker.deactivateComponent(comp, rtobj);
    if (ret != RTC::RTC_OK) { return ret; }
    if (!m_syncDeactivation)
      {
        ret = onDeactivated(rtobj, -1);
        if (ret != RTC::RTC_OK)
          {
            RTC_ERROR(("onDeactivated() failed."));
          }
        return ret;
      }
    //------------------------------------------------------------
    // Waiting for synchronized deactivation
    RTC_DEBUG(("Synchronous deactivation mode. "
               "Waiting for the RTC to be INACTIVE state. "));
    return waitForDeactivated(rtobj);
  }

  RTC::ReturnCode_t ExecutionContextBase::
  waitForDeactivated(RTC_impl::RTObjectStateMachine* rtobj)
  {
    long int count(0);
    RTC::ReturnCode_t ret = onWaitingDeactivated(rtobj, count);
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("onWaitingDeactivated failed."));
        return ret;
      }
    long int cycle =
      (long int)((double)m_deactivationTimeout / (double)getPeriod());
    RTC_DEBUG(("Timeout is %f [s] (%f [s] in %d times)",
               (double)m_deactivationTimeout, getRate(), cycle));
    // Wating ACTIVE -> INACTIVE
    coil::TimeValue starttime(coil::gettimeofday());
    while (rtobj->isCurrentState(RTC::ACTIVE_STATE))
      {
        ret = onWaitingDeactivated(rtobj, count); // Template method
        if (ret != RTC::RTC_OK)
          {
            RTC_ERROR(("onWaitingDeactivated failed."));
            return ret;
          }
        coil::sleep(getPeriod());
        coil::TimeValue delta(coil::gettimeofday() - starttime);
        RTC_DEBUG(("Waiting to be INACTIVE state. Sleeping %f [s] (%d/%d)",
                   (double)delta, count, cycle));
        ++count;
        if (delta > m_deactivationTimeout || count > cycle)
          {
            RTC_ERROR(("The component is not responding."));
            break;
          }
      }
    // Now State must be INACTIVE or ERROR
    if (rtobj->isCurrentState(RTC::ACTIVE_STATE))
      {
        RTC_ERROR(("Unknown error: Invalid state transition."));
        return RTC::RTC_ERROR;
      }
    RTC_DEBUG(("Current state is %s", getStateString(rtobj->getState())));
    ret = onDeactivated(rtobj, count);
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("onDeactivated() failed."));
      }
    RTC_DEBUG(("onDeactivated() done."))
    return ret;
  }
  
  /*!
   * @if jp
   * @brief RT����ݡ��ͥ�Ȥ�ꥻ�åȤ���
   * @else
   * @brief Reset the RT-component
   * @endif
   */
  RTC::ReturnCode_t ExecutionContextBase::
  resetComponent(RTC::LightweightRTObject_ptr comp)
  {
    RTC_TRACE(("resetComponent()"));
    RTC::ReturnCode_t ret = onResetting(comp); // Template
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("onResetting() failed."));
        return ret;
      }
    RTC_impl::RTObjectStateMachine* rtobj;
    ret = m_worker.resetComponent(comp, rtobj); // Actual resetComponent()
    if (ret != RTC::RTC_OK) { return ret; }
    if (!m_syncReset)
      {
        ret = onReset(rtobj, -1);
        if (ret != RTC::RTC_OK)
          {
            RTC_ERROR(("onReset() failed."));
          }
        return ret;
      }
    //------------------------------------------------------------
    // Waiting for synchronized reset
    RTC_DEBUG(("Synchronous reset mode. "
               "Waiting for the RTC to be INACTIVE state. "));
    return waitForReset(rtobj);
  }
  
  RTC::ReturnCode_t ExecutionContextBase::
  waitForReset(RTC_impl::RTObjectStateMachine* rtobj)
  {
    long int count(0);
    RTC::ReturnCode_t ret = onWaitingReset(rtobj, count);
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("onWaitingReset() failed."));
        return ret;
      }
    long int cycle =
      (long int)((double)m_resetTimeout / (double)getPeriod());
    RTC_DEBUG(("Timeout is %f [s] (%f [s] in %d times)",
               (double)m_resetTimeout, getRate(), cycle));
    // Wating ERROR -> INACTIVE
    coil::TimeValue starttime(coil::gettimeofday());
    while (rtobj->isCurrentState(RTC::ERROR_STATE))
      {
        ret = onWaitingReset(rtobj, count); // Template
        if (ret != RTC::RTC_OK)
          {
            RTC_ERROR(("onWaitingReset failed."));
            return ret;
          }
        coil::sleep(getPeriod());
        coil::TimeValue delta(coil::gettimeofday() - starttime);
        RTC_DEBUG(("Waiting to be INACTIVE state. Sleeping %f [s] (%d/%d)",
                   (double)delta, count, cycle));
        ++count;
        if (delta > m_resetTimeout || count > cycle)
          {
            RTC_ERROR(("The component is not responding."));
            break;
          }
      }
    // Now State must be INACTIVE
    if (!rtobj->isCurrentState(RTC::INACTIVE_STATE))
      {
        RTC_ERROR(("Unknown error: Invalid state transition."));
        return RTC::RTC_ERROR;
      }
    RTC_DEBUG(("Current state is %s", getStateString(rtobj->getState())));
    ret = onReset(rtobj, count); // Template method
    if (ret != RTC::RTC_OK)
      {
        RTC_ERROR(("onResetd() failed."));
      }
    RTC_DEBUG(("onReset() done."))
    return ret;
  }

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
  RTC::LifeCycleState ExecutionContextBase::
  getComponentState(RTC::LightweightRTObject_ptr comp)
  {
    RTC::LifeCycleState state = m_worker.getComponentState(comp);
    RTC_TRACE(("getComponentState() = %s", getStateString(state)));
    if (state == RTC::CREATED_STATE)
      {
        RTC_ERROR(("CREATED state: not initialized "
                   "RTC or unknwon RTC specified."));
      }
    return onGetComponentState(state);
  }

  const char* ExecutionContextBase::getStateString(RTC::LifeCycleState state)
  {
    return m_worker.getStateString(state);
  }
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
  RTC::ExecutionKind ExecutionContextBase::getKind(void) const
  {
    RTC::ExecutionKind kind = m_profile.getKind();
    RTC_TRACE(("getKind() = %s", getKindString(kind)));
    kind = onGetKind(kind);
    RTC_DEBUG(("onGetKind() returns %s", getKindString(kind)));
    return kind;
  }
  
  /*!
   * @if jp
   * @brief Profile���������
   *
   * RTC::ExecutionContextProfile ��������롣��������
   * ExecutionContextProfile �ν�ͭ���ϸƤӽФ�¦�ˤ��롣�������줿��
   * �֥������Ȥ����פˤʤä���硢�ƤӽФ�¦������������Ǥ���餦��
   *
   * @return RTC::ExecutionContextProfile
   *
   * @else
   * @brief Getting Profile
   *
   * This function gets RTC::ExecutionContextProfile.  The ownership
   * of the obtained ExecutionContextProfile is given to caller. The
   * caller should release obtained object when it is unneccessary
   * anymore.
   *
   * @return RTC::ExecutionContextProfile
   *
   * @endif
   */
  RTC::ExecutionContextProfile* ExecutionContextBase::getProfile(void)
  {
    RTC_TRACE(("getProfile()"));
    RTC::ExecutionContextProfile* prof = m_profile.getProfile();
    RTC_DEBUG(("kind: %s", getKindString(prof->kind)));
    RTC_DEBUG(("rate: %f", prof->rate));
    RTC_DEBUG(("properties:"));
    coil::Properties props;
    NVUtil::copyToProperties(props, prof->properties);
    RTC_DEBUG_STR((props));
    return onGetProfile(prof);
  }
  
  //============================================================
  // Delegated functions to ExecutionContextProfile
  //============================================================
  /*!
   * @if jp
   * @brief CORBA ���֥������Ȼ��Ȥμ���
   * @else
   * @brief Get the reference to the CORBA object
   * @endif
   */
  void ExecutionContextBase::setObjRef(RTC::ExecutionContextService_ptr ec_ptr)
  {
    m_worker.setECRef(ec_ptr);
    m_profile.setObjRef(ec_ptr);
  }
  /*!
   * @if jp
   * @brief CORBA ���֥������Ȼ��Ȥμ���
   * @else
   * @brief Getting object reference
   * @endif
   */
  RTC::ExecutionContextService_ptr ExecutionContextBase::getObjRef(void) const
  {
    return m_profile.getObjRef();
  }
  
  /*!
   * @if jp
   * @brief ExecutionKind ��ʸ���󲽤���
   * @else
   * @brief Converting ExecutionKind enum to string 
   * @endif
   */
  const char* ExecutionContextBase::getKindString(RTC::ExecutionKind kind) const
  {
    return m_profile.getKindString(kind);
  }
  
  /*!
   * @if jp
   * @brief ExecutionKind �����ꤹ��
   * @else
   * @brief Set the ExecutionKind
   * @endif
   */
  RTC::ReturnCode_t ExecutionContextBase::setKind(RTC::ExecutionKind kind)
  {
    return m_profile.setKind(kind);
  }
  
  /*!
   * @if jp
   * @brief Owner����ݡ��ͥ�Ȥ򥻥åȤ��롣
   * @else
   * @brief Setting owner component of the execution context
   * @endif
   */
  RTC::ReturnCode_t ExecutionContextBase::
  setOwner(RTC::LightweightRTObject_ptr comp)
  {
    return m_profile.setOwner(comp);
  }
  
  /*!
   * @if jp
   * @brief Owner����ݡ��ͥ�Ȥλ��Ȥ��������
   * @else
   * @brief Getting a reference of the owner component
   * @endif
   */
  const RTC::RTObject_ptr ExecutionContextBase::getOwner() const
  {
    return m_profile.getOwner();
  }
  
  /*!
   * @if jp
   * @brief RT����ݡ��ͥ�Ȥλ��üԥꥹ�Ȥ��������
   * @else
   * @brief Getting participant RTC list
   * @endif
   */
  const RTC::RTCList& ExecutionContextBase::getComponentList() const
  {
    return m_profile.getComponentList();
  }
  
  /*!
   * @if jp
   * @brief Properties�򥻥åȤ���
   * @else
   * @brief Setting Properties
   * @endif
   */
  void ExecutionContextBase::setProperties(coil::Properties& props)
  {
    m_profile.setProperties(props);
  }
  
  /*!
   * @if jp
   * @brief Properties���������
   * @else
   * @brief Setting Properties
   * @endif
   */
  const coil::Properties ExecutionContextBase::getProperties() const
  {
    return m_profile.getProperties();
  }
  
  /*!
   * @if jp
   * @brief Profile���������
   * @else
   * @brief Getting Profile
   * @endif
   */
  const RTC::ExecutionContextProfile& ExecutionContextBase::
  getProfile(void) const
  {
    return m_profile.getProfile();
  }
  // end of delegated functions to ExecutionContextProfile
  //============================================================
  
  
  //============================================================
  // private functions
  /*!
   * @if jp
   * @brief Properties����¹ԥ���ƥ����Ȥ򥻥åȤ���
   * @else
   * @brief Setting execution rate from given properties.
   * @endif
   */
  bool ExecutionContextBase::setExecutionRate(coil::Properties& props)
  {
    if (props.findNode("rate") != NULL)
      {
        double rate;
        if (coil::stringTo(rate, props["rate"].c_str()))
          {
            setRate(rate);
            return true;
          }
      }
    return false;
  }
  
  /*!
   * @if jp
   * @brief Properties����������ܥ⡼�ɤ򥻥åȤ���
   * @else
   * @brief Setting state transition mode from given properties.
   * @endif
   */
  bool ExecutionContextBase::
  setTransitionMode(coil::Properties& props, const char* key, bool& flag)
  {
    RTC_TRACE(("setTransitionMode(%s)", key));
    if (props.findNode(key) != NULL)
      {
        flag = coil::toBool(props[key], "YES", "NO", "YES");
        RTC_DEBUG(("Transition Mode: %s = %s",
                   key, flag ? "YES" : "NO"));
        return true;
      }
    RTC_DEBUG(("Configuration %s not found.", key));
    return false;
  }
  
  /*!
   * @if jp
   * @brief Properties�����������Timeout�򥻥åȤ���
   * @else
   * @brief Setting state transition timeout from given properties.
   * @endif
   */
  bool ExecutionContextBase::
  setTimeout(coil::Properties& props, const char* key,
             coil::TimeValue& timevalue)
  {
    RTC_TRACE(("setTimeout(%s)", key));
    if (props.findNode(key) != NULL)
      {
        double timeout;
        if (coil::stringTo(timeout, props[key].c_str()))
          {
            timevalue = timeout;
            RTC_DEBUG(("Timeout (%s): %f [s]", key, timeout));
            return true;
          }
      }
    RTC_DEBUG(("Configuration %s not found.", key));
    return false;
  }
};  // namespace RTC