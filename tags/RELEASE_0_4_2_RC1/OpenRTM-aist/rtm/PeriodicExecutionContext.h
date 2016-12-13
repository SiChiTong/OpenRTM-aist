// -*- C++ -*-
/*!
 * @file PeriodicExecutionContext.h
 * @brief PeriodicExecutionContext class
 * @date $Date: 2007-09-20 11:22:08 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2006
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id: PeriodicExecutionContext.h,v 1.4.2.3 2007-09-20 11:22:08 n-ando Exp $
 *
 */

/*
 * $Log: not supported by cvs2svn $
 * Revision 1.4.2.2  2007/09/19 07:44:35  n-ando
 * The usleep() in the execution loop will be skipped, if wait-time is 0.
 *
 * Revision 1.4.2.1  2007/07/20 15:59:18  n-ando
 * ACE header include was added.
 *
 * Revision 1.4  2007/04/26 15:30:46  n-ando
 * The header include order was modified to define _REENTRANT before
 * including ace/config-lite.h in Linux systems.
 * In ace 5.4.7 or later, _REENTRANT flag should be defined explicitly.
 *
 * Revision 1.3  2007/04/13 15:52:34  n-ando
 * RTC::OK was changed to RTC::RTC_OK.
 * Initialization function was added to make it an external module.
 *
 * Revision 1.2  2007/01/21 10:27:00  n-ando
 * Object reference count related bugs were fixed.
 *
 * Revision 1.1  2007/01/09 15:29:35  n-ando
 * PeriodicExecutionContext class
 *
 */

#ifndef PeriodicExecutionContext_h
#define PeriodicExecutionContext_h

#include <rtm/RTC.h>
#include <rtm/idl/RTCSkel.h>
#include <rtm/idl/OpenRTMSkel.h>
#include <rtm/Manager.h>
#include <rtm/StateMachine.h>
#include <rtm/ExecutionContextBase.h>

// ACE
#include <ace/Task.h>
#include <ace/OS_NS_unistd.h>
#include <vector>
#include <iostream>

namespace RTC
{

  class PeriodicExecutionContext
    : public virtual ExecutionContextBase,
      public ACE_Task<ACE_MT_SYNCH>
  {
  public:
    PeriodicExecutionContext();
    PeriodicExecutionContext(DataFlowComponent_ptr owner,
			     double rate = 1000.0);
    virtual ~PeriodicExecutionContext();
    
    ExecutionContextService_ptr getRef() {return m_ref;}

    /*!
     * @if jp
     * @brief ����ݡ��ͥ�ȤΥ����ƥ��ӥƥ�����åɤ���������
     *
     * ����ݡ��ͥ�Ȥ����������ƥ��ӥƥ�����åɤ���������ư���롣
     * ����� ACE_Task �����ӥ����饹�᥽�åɤΥ����С��饤�ɡ�
     *
     * @param args �̾��0
     *
     * @else
     *
     * @brief Create internal activity thread
     *
     * Create internal activity thread. This is ACE_Task class method over ride.
     *
     * @param args usually give 0
     *
     * @endif
     */     
    virtual int open(void *args);


    /*!
     * @if jp
     * @brief ����ݡ��ͥ�ȤΥ����ƥ��ӥƥ�����åɴؿ�
     *
     * ����ݡ��ͥ�Ȥ����������ƥ��ӥƥ�����åɤμ¹Դؿ���
     * ����� ACE_Task �����ӥ����饹�᥽�åɤΥ����С��饤�ɡ�
     *
     * @else
     *
     * @brief Create internal activity thread
     *
     * Run by a daemon thread to handle deferred processing.
     * ACE_Task class method override.
     *
     * @endif
     */     
    virtual int svc(void);


    /*!
     * @if jp
     * @brief ����ݡ��ͥ�ȤΥ����ƥ��ӥƥ�����åɽ�λ�ؿ�
     *
     * ����ݡ��ͥ�Ȥ����������ƥ��ӥƥ�����åɽ�λ���˸ƤФ�롣
     * ����ݡ��ͥ�ȥ��֥������Ȥ��󥢥��ƥ��ֲ����ޥ͡�����ؤ����Τ�Ԥ���
     * ����� ACE_Task �����ӥ����饹�᥽�åɤΥ����С��饤�ɡ�
     *
     * @else
     *
     * @brief Close activity thread
     *
     * close() method is called when activity thread svc() is returned.
     * This method deactivate this object and notify it to manager.
     * ACE_Task class method override.
     *
     * @endif
     */     
    virtual int close(unsigned long flags);


    //============================================================
    // ExecutionContext
    //============================================================
    
    virtual CORBA::Boolean is_running();

    virtual ReturnCode_t start();
    virtual ReturnCode_t stop();

    virtual CORBA::Double get_rate();
    virtual ReturnCode_t  set_rate(CORBA::Double rate);

    virtual ReturnCode_t activate_component(LightweightRTObject_ptr comp);
    virtual ReturnCode_t deactivate_component(LightweightRTObject_ptr comp);
    virtual ReturnCode_t reset_component(LightweightRTObject_ptr comp);

    virtual LifeCycleState get_component_state(LightweightRTObject_ptr comp);
    virtual ExecutionKind get_kind();
    virtual ReturnCode_t add(LightweightRTObject_ptr comp);
    virtual ReturnCode_t remove(LightweightRTObject_ptr comp);

    virtual ExecutionContextProfile* get_profile();

  
  protected:
    //============================================================
    // DFPBase
    //============================================================
    typedef LifeCycleState ExecContextState;
    /*
    enum ExecContextState
      {
	INACTIVE_STATE,
	ACTIVE_STATE,
	ERROR_STATE,
      };
    */
    typedef StateHolder<ExecContextState> ECStates;
    
    class DFPBase
    {
    public:

      DFPBase(UniqueId id)
	: ec_id(id), m_sm(3)
      {
	m_sm.setListener(this);
	m_sm.setEntryAction (ACTIVE_STATE,
			     &DFPBase::on_activated);
	m_sm.setDoAction    (ACTIVE_STATE,
			     &DFPBase::on_execute);
	m_sm.setPostDoAction(ACTIVE_STATE,
			     &DFPBase::on_state_update);
	m_sm.setExitAction  (ACTIVE_STATE,
			     &DFPBase::on_deactivated);
	m_sm.setEntryAction (ERROR_STATE,
			     &DFPBase::on_aborting);
	m_sm.setDoAction    (ERROR_STATE,
			     &DFPBase::on_error);
	m_sm.setExitAction  (ERROR_STATE,
			     &DFPBase::on_reset);

	ECStates st;
	st.prev = INACTIVE_STATE;
	st.curr = INACTIVE_STATE;
	st.next = INACTIVE_STATE;
	m_sm.setStartState(st);
	m_sm.goTo(INACTIVE_STATE);
      }	
      virtual ~DFPBase(){}
      virtual void on_startup() = 0;
      virtual void on_shutdown() = 0;

      virtual void on_activated(const ECStates& st) = 0;
      virtual void on_deactivated(const ECStates& st) = 0;
      virtual void on_aborting(const ECStates& st) = 0;
      virtual void on_error(const ECStates& st) = 0;
      virtual void on_reset(const ECStates& st) = 0;
      virtual void on_execute(const ECStates& st) = 0;
      virtual void on_state_update(const ECStates& st) = 0;

      virtual void on_rate_changed() = 0;
      virtual void worker() {return m_sm.worker();}
      virtual ExecContextState get_state(){ return m_sm.getState();}
      UniqueId ec_id;
      StateMachine<ExecContextState, DFPBase> m_sm;
    };


    //============================================================
    // DFP
    //============================================================
    template <class Object>
    class DFP
      : public DFPBase
    {
    public:
      DFP(Object obj, UniqueId id)
	: DFPBase(id), m_obj(obj), m_active(true)
      {
	
      }
      void on_startup()
      {
	m_obj->on_startup(ec_id);
      }
      void on_shutdown()
      {
	m_obj->on_shutdown(ec_id);
      }

      void on_activated(const ECStates& st)
      {
	if (m_obj->on_activated(ec_id) != RTC::RTC_OK)
	  {
	    m_sm.goTo(ERROR_STATE);
	    return;
	  }
	return;
      }

      void on_deactivated(const ECStates& st)
      {
	m_obj->on_deactivated(ec_id);
      }

      void on_aborting(const ECStates& st)
      {
	m_obj->on_aborting(ec_id);
      }

      void on_error(const ECStates& st)
      {
	m_obj->on_error(ec_id);
      }

      void on_reset(const ECStates& st)
      {
	if (m_obj->on_reset(ec_id) != RTC::RTC_OK)
	  {
	    m_sm.goTo(ERROR_STATE);
	    return;
	  }
	return;
      }

      void on_execute(const ECStates& st)
      {
	if (m_obj->on_execute(ec_id) != RTC::RTC_OK)
	  {
	    m_sm.goTo(ERROR_STATE);
	    return;
	  }  
	return;
      }

      void on_state_update(const ECStates& st)
      {
	if (m_obj->on_state_update(ec_id) != RTC::RTC_OK)
	  {
	    m_sm.goTo(ERROR_STATE);
	    return;
	  }
	return;
      }

      void on_rate_changed()
      {
	m_obj->on_rate_changed(ec_id);
      }

      Object m_obj;
      bool m_active;
    };

    struct Comp
    {
      Comp(LightweightRTObject_ptr ref, DataFlowComponent_ptr dfp,
	   UniqueId id)
	: _ref(ref), _sm(dfp, id)
      {
      }
      Comp(const Comp& comp)
	: _ref(comp._ref), _sm(comp._sm.m_obj, comp._sm.ec_id)
      {
      }
      Comp& operator=(const Comp& comp)
      {
	_ref = comp._ref;
	_sm.m_obj = comp._sm.m_obj;
	_sm.ec_id = comp._sm.ec_id;
	return *this;
      }
      LightweightRTObject_var _ref;
      DFP<DataFlowComponent_var> _sm;
    };



    struct find_comp
    {
      LightweightRTObject_var m_comp;
      find_comp(LightweightRTObject_ptr comp) : m_comp(comp) {}
      bool operator()(Comp& comp)
      {
	return comp._ref->_is_equivalent(m_comp);
      }
    };


    struct invoke_on_startup
    {
      void operator()(Comp& comp)
      {
	comp._sm.on_startup();
      }
    };

    struct invoke_on_shutdown
    {
      void operator()(Comp& comp)
      {
	comp._sm.on_shutdown();
      }
    };

    struct invoke_on_rate_changed
    {
      void operator()(Comp& comp)
      {
	comp._sm.on_rate_changed();
      }
    };

    struct invoke_worker
    {
      void operator()(Comp& comp)
      {
	comp._sm.worker();
      }
    };

    std::vector<Comp> m_comps;
    typedef std::vector<Comp>::iterator CompItr;
    bool m_running;
    ExecutionContextProfile m_profile;
    long int m_usec;
    ExecutionContextService_var m_ref;
    bool m_nowait;
  
  }; // class PeriodicExecutionContext
}; // namespace RTC

extern "C"
{
  void PeriodicExecutionContextInit(RTC::Manager* manager);
};

#endif