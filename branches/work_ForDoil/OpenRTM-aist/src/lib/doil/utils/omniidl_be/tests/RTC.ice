// RTC.idl

#ifndef RTC_IDL
#define RTC_IDL

#include "SDOPackage.ice"

//#pragma prefix "omg.org"



module RTC
{
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief ExecutionContextHandle_t
   *
   * @section Description
   *
   * This data type represents the association between an RTC and an
   * ExecutionContext in which it participates.
   * 
   * @section Semantics
   *
   * This is an opaque DataType. It has no attributes or operations.
   *
   * @endif
   */
  

  
  //SDOPackage::NVList NVList;
  
  /*!
   * @if jp
   * @brief ReturnCode_t
   *
   * OMG RTC 1.0 �������¿���Υ��ڥ졼�����ϡ����饤��Ȥ��Ф��ƹͤ�
   * ���륨�顼���֤���𤹤�ɬ�פ����롣����ϡ�ReturnCode_t���ν��
   * �꥿���󥳡��ɤˤ��Ԥ��롣
   *
   * OMG RTC 1.0 �� PIM ��ˤ����ơ�ReturnCode_t�����ͤ��֤��ʤ����ڥ졼������
   * ����������ͤη��˰�¸���ơ�������ˡ�ˤ�ꥨ�顼����𤹤��ΤȤ��롣
   * -���ڥ졼����������ͤ��֤���� (OMG RTC 1.0 Section 5.2.2.6.4 ��
   *  get_rate�Τ褦��)������ͤ��֤����Ȥˤ�ꥨ�顼�򼨤���ΤȤ��롣
   * - ���ڥ졼����󤬥��֥������ȥ�ե����(RTObject::get_component_profile
   *   OMG RTC 1.0 5.4.2.2.1�򻲾�) ���̾��֤���硢nil���Ȥ��֤����Ȥˤ��
   *   ���顼�򼨤���ΤȤ��롣
   *
   * @else
   * @brief ReturnCode_t
   *
   * A number of operations in this specification will need to report
   * potential error conditions to their clients. This task shall be
   * accomplished by means of operation "return codes" of type
   * ReturnCode_t
   *
   * Operations in the PIM that do not return a value of type
   * ReturnCode_t shall report errors in the following ways, depending
   * on their return type:
   * - If an operation normally returns a positive numerical value (such as
   *   get_rate, see [OMG RTC 1.0 Section 5.2.2.6.4]), it shall indicate
   *   failure by returning a negative value.
   * - If an operation normally returns an object reference (such as
   *   RTObject::get_component_profile, see [OMG RTC 1.0 Section 5.4.2.2.1]),
   *   it shall indicate failure by returning a nil reference.
   *
   * @param RTC_OK The operation completed successfully.
   * @param RTC_ERROR The operation failed with a generic, unspecified error.
   * @param BAD_PARAMETER The operation failed because an illegal argument was
   *        passed to it.
   * @param UNSUPPORTED The operation is unsupported by the implementation
   *        (e.g., it belongs to a compliance point that is not implemented).
   * @param OUT_OF_RESOURCES The target of the operation ran out of the
   *        resources needed to complete the operation.
   * @param PRECONDITION_NOT_MET A pre-condition for the operation was not met.
   *
   * @endif
   */
  enum ReturnCode_t
  {
    RTC_OK,
    RTC_ERROR,
    BAD_PARAMETER,
    UNSUPPORTED,
    OUT_OF_RESOURCES,
    PRECONDITION_NOT_MET
  };

  /*!
   * @if jp
   * @brief 
   * @else
   * @brief LifeCycleState
   *
   * @section Description
   * LifeCycleState is an enumeration of the states in the lifecycle above.
   *
   * @endif
   */
  enum LifeCycleState
  {
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief CREATED
     *
     * @section Description
     *
     * The RTC object has been instantiated but not yet fully initialized.
     * @endif
     */
    CREATED_STATE,

    /*!
     * @if jp
     * @brief 
     * @else
     * @brief INACTIVE
     *
     * @section Description
     *
     * The RTC is Alive but is not being invoked in any execution
     * context (see Section 5.2.2.5), regardless of whether the context
     * is Running or not.
     *
     * @section Semantics
     *
     * An instance of this state exists for each execution context in
     * which the RTC participates. If the RTC does not participate in
     * any execution context, a single instance of this state exists.
     *
     * @endif
     */
    INACTIVE_STATE,

    /*!
     * @if jp
     * @brief 
     * @else
     * @brief ACTIVE
     *
     * @section Description
     *
     * The RTC is Alive and will be invoked in the execution context
     * if the context is Running.
     *
     * @section Semantics
     * 
     * An instance of this state exists for each execution context in
     * which the RTC participates. If the RTC does not participate in
     * any execution context, this state shall never be observed.
     *
     * @endif
     */
    ACTIVE_STATE,

    /*!
     * @if jp
     * @brief 
     * @else
     * @brief ERROR
     *
     * @section Description
     *
     * The RTC has encountered a problem in a given execution context
     * and cannot continue functioning in that context without being
     * reset.
     *
     * @endif
     */
    ERROR_STATE
  };
  
  interface ExecutionContext;
    sequence<ExecutionContext> ExecutionContextList;

  /*!
   * @if jp
   * @brief
   * @else
   * @brief ComponentAction
   *
   * @section Description
   *
   * The ComponentAction interface provides callbacks corresponding to
   * the execution of the lifecycle operations of LightweightRTObject
   * (see Section 5.2.2.2) and ExecutionContext (see Section
   * 5.2.2.5). An RTC developer may implement these callback
   * operations in order to execute application-specific logic
   * pointing response to those transitions.
   *
   * @section Semantics
   *
   * Clients of an RTC are not expected to invoke these operations
   * directly; they are provided for the benefit of the RTC middleware
   * implementation.
   *
   * @endif
   */
  interface ComponentAction
  {
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief on_initialize
     *
     * @section Description
     *
     * The RTC has been initialized and entered the Alive state.
     *
     * @section Semantics
     *
     * Any RTC-specific initialization logic should be performed here.
     *
     * @endif
     */
    ReturnCode_t on_initialize();

    /*!
     * @if jp
     * @brief 
     * @else
     * @brief on_finalize
     *
     * @section Description
     *
     * The RTC is being destroyed.
     *
     * @section Semantics
     *
     * Any final RTC-specific tear-down logic should be performed here.
     *
     * @endif
     */
    ReturnCode_t on_finalize();

    /*!
     * @if jp
     * @brief 
     * @else
     * @brief on_startup
     *
     * @section Description
     *
     * The given execution context, in which the RTC is participating,
     * has transitioned from Stopped to Running.
     *
     * @endif
     */
    ReturnCode_t on_startup( long exec_handle);

    /*!
     * @if jp
     * @brief 
     * @else
     * @brief on_shutdown
     *
     * @section Description
     *
     * The given execution context, in which the RTC is participating,
     * has transitioned from Running to Stopped.
     *
     * @endif
     */
    ReturnCode_t on_shutdown( long exec_handle);

    /*!
     * @if jp
     * @brief 
     * @else
     * @brief on_activated
     *
     * @section Description
     *
     * The RTC has been activated in the given execution context.
     *
     * @endif
     */
    ReturnCode_t on_activated( long exec_handle);

    /*!
     * @if jp
     * @brief 
     * @else
     * @brief on_deactivated
     *
     * @section Description
     *
     * The RTC has been deactivated in the given execution context.
     *
     * @endif
     */
    ReturnCode_t on_deactivated( long exec_handle);

    /*!
     * @if jp
     * @brief 
     * @else
     * @brief on_aborting
     *
     * @section Description
     *
     * The RTC is transitioning from the Active state to the Error
     * state in some execution context.
     *
     * @section Semantics
     *
     * This callback is invoked only a single time for time that the
     * RTC transitions into the Error state from another state. This
     * behavior is in contrast to that of on_error.  
     *
     * @endif
     */
    ReturnCode_t on_aborting( long exec_handle);

    /*!
     * @if jp
     * @brief 
     * @else
     * @brief on_error
     *
     * @section Description
     *
     * The RTC remains in the Error state.
     *
     * @section Semantics
     *
     * If the RTC is in the Error state relative to some execution
     * context when it would otherwise be invoked from that context
     * (according to the context��s ExecutionKind), this callback
     * shall be invoked instead. For example,
     *
     * - If the ExecutionKind is PERIODIC, this operation shall be
     *   invoked in sorted order at the rate of the context instead of
     *   DataFlowComponentAction::on_execute and on_state_update.
     *
     * - If the ExecutionKind is EVENT_DRIVEN, this operation shall be
     *   invoked whenever FsmParticipantAction::on_action would
     *   otherwise have been invoked.
     *
     * @endif
     */
    ReturnCode_t on_error( long exec_handle);

    /*!
     * @if jp
     * @brief 
     * @else
     * @brief on_reset
     *
     * @section Description
     *
     * The RTC is in the Error state. An attempt is being made to
     * recover it such that it can return to the Inactive state.
     *
     * @section Semantics
     *
     * If the RTC was successfully recovered and can safely return to
     * the Inactive state, this method shall complete with
     * ReturnCode_t::OK. Any other result shall indicate that the RTC
     * should remain in the Error state.
     *
     * @endif
     */
    ReturnCode_t on_reset( long exec_handle);
  };
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief LightweightRTObject
   * 
   * @section Description
   *
   * This interface is realized by all lightweight RTCs (as required
   * by the lightweightRTComponent stereotype). It defines the states
   * and transitions through which all RTCs will pass from the time
   * they are created until the time they are destroyed.
   *
   * @section Semantics
   * @subsection Initialization
   *
   * An RTC begins in the Created state; at this point, it has been
   * instantiated but not yet fully initialized. Note that this state
   * is highly implementation-dependent. For example, it may
   * correspond to the invocation of a constructor in languages that
   * support that concept, but not all languages do. Furthermore, how
   * soon this state is entered before initialize is invoked is
   * implementation-dependent. Therefore, it should be relied on by
   * RTC implementers only to the minimum extent possible.  An RTC
   * that has completed its initialization and has not been finalized
   * is said to be Alive.
   *
   * @subsection Execution Context
   *
   * An RTC in the Alive state may participate in any number of
   * execution contexts (see Section 5.2.2.5 ). These contexts shall
   * be represented to an RTC as distinct instances of the
   * ExecutionContext class. The ExecutionContext manages the behavior
   * of each RTC that participates in it. This relationship is defined
   * by the following state machine, which is embedded within the
   * ExecutionContext's own lifecycle (see Figure 5.5 ). Each
   * participating RTC is represented as a separate parallel region.
   *
   * Relative to a given execution context, an RTC may either be
   * Active, Inactive, or in Error. When the RTC is Active in a
   * Running execution context, the ComponentAction callbacks (see
   * Section 5.2.2.4) shall be invoked as appropriate for the context��
   * s ExecutionKind. The callbacks shall not be invoked relative to
   * that context when either the RTC is Inactive in that context or
   * the context is Stopped. (Note that starting and stopping an
   * execution context shall not impact whether its participating RTCs
   * are Active or Inactive.)  It may be that a given RTC does not
   * directly participate in any execution contexts. Such an RTC is
   * referred to as passive.  A passive RTC may provide services to
   * other components upon request. At any other time, it shall not be
   * required to perform any ongoing activity of its own; therefore,
   * instances of such an RTC typically exist only as parts (directly
   * or indirectly) of a containing active RTC.
   *
   * @subsection Error Handling
   *
   * If an operation fails while the RTC is Active in a given
   * execution context, the RTC will transition to the Error state
   * corresponding to that context. While the RTC is in Error, the
   * ComponentAction::on_error callback will be invoked in place of
   * those callbacks that would otherwise have been invoked according
   * to the context��s ExecutionKind. For example, if the kind is
   * PERIODIC, on_error shall be invoked instead of the pair of
   * on_execute, and on_state_update.  When an RTC is in Error, it may
   * be reset. If resetting is successful, the RTC shall return to the
   * Inactive state. If resetting is unsuccessful, it shall remain in
   * the Error state.
   * 
   * @endif
   */
  interface LightweightRTObject
    extends  ComponentAction
  {
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief initialize
     *
     * @section Description
     * Initialize the RTC that realizes this interface.
     *
     * @section Semantics
     * The invocation of this operation shall result in the invocation
     * of the callback ComponentAction::on_initialize.
     *
     * @section Constraints
     *
     * - An RTC may be initialized only while it is in the Created
     *   state. Any attempt to invoke this operation while in another
     *   state shall fail with ReturnCode_t::PRECONDITION_NOT_MET.
     * - Application developers are not expected to call this operation
     *   directly; it exists for use by the RTC infrastructure.
     *
     * @endif
     */
    ReturnCode_t initialize();

    /*!
     * @if jp
     * @brief 
     * @else
     * @brief finalize
     *
     * @section Description
     *
     * Finalize the RTC that realizes this interface, preparing it for
     * destruction.
     *
     * @section Semantics
     *
     * This invocation of this operation shall result in the
     * invocation of the callback ComponentAction::on_finalize
     *
     * @section Constraints
     *
     * - An RTC may not be finalized while it is participating in any
     *   execution context. It must first be removed with
     *   ExecutionContextOperations::remove_component. Otherwise, this
     *   operation shall fail with
     *   ReturnCode_t::PRECONDITION_NOT_MET. See Figure 5.9.
     *
     * - An RTC may not be finalized while it is in the Created state. Any
     *   attempt to invoke this operation while in that state shall fail with
     *   ReturnCode_t::PRECONDITION_NOT_MET.
     *
     * - Application developers are not expected to call this operation
     *   directly; it exists for use by the RTC infrastructure.
     *
     * @endif
     */
    ReturnCode_t finalize();

    /*!
     * @if jp
     * @brief 
     * @else
     * @brief is_alive
     * 
     * @section Description
     *
     * A component is alive or not regardless of the execution context
     * from which it is observed. However, whether or not it is
     * Active, Inactive, or in Error is dependent on the execution
     * context(s) (see Figure 5.7) in which it is running. That is, it
     * may be Active in one context but Inactive in
     * another. Therefore, this operation shall report whether this
     * RTC is either Active, Inactive, or in Error; which of those
     * states a component is in with respect to a particular context
     * may be queried from the context itself.
     *
     * @endif
     *
     */
    bool is_alive( ExecutionContext exec_context);

    /*!
     * @if jp
     * @brief 
     * @else
     * @brief exit
     *
     * @section Description
     *
     * Stop the RTC��s execution context(s) and finalize it along with
     * its contents.
     *
     * @section Semantics
     *
     * Any execution contexts for which the RTC is the owner shall be
     * stopped.  If the RTC participates in any execution contexts
     * belonging to another RTC that contains it, directly or
     * indirectly (i.e., the containing RTC is the owner of the
     * ExecutionContext), it shall be deactivated in those contexts.
     * After the RTC is no longer Active in any Running execution
     * context, it and any RTCs contained transitively within it shall
     * be finalized.
     *
     * @section Constraints
     *
     * An RTC cannot be exited if it has not yet been initialized. Any
     * attempt to exit an RTC that is in the Created state shall fail
     * with ReturnCode_t::PRECONDITION_NOT_MET.  
     *
     * @endif
     */
    ReturnCode_t exit();

    /*!
     * @if jp
     * @brief 
     * @else
     * @brief attach_context
     *
     * @section Description
     *
     * Inform this RTC that it is participating in the given execution
     * context. Return a handle that represents the association of
     * this RTC with the context.
     *
     * @section Semantics
     *
     * This operation is intended to be invoked by
     * ExecutionContextOperations::add_component (see Section
     * 5.2.2.6.6). It is not intended for use by other clients.
     *
     * @endif
     */
    long attach_context( ExecutionContext exec_context);

    /*!
     * @if jp
     * @brief 
     * @else
     * @brief detach_context
     * @section Description
     * 
     * Inform this RTC that it is no longer participating in the given
     * execution context.
     *
     * @section Semantics
     * 
     * This operation is intended to be invoked by
     * ExecutionContextOperations::remove_component (see Section
     * 5.2.2.6.7). It is not intended for use by other clients.
     *
     * @section Constraints
     *
     * - This operation may not be invoked if this RTC is not already
     *   participating in the execution context. Such a call shall fail
     *   with ReturnCode_t::PRECONDITION_NOT_MET.
     *
     * - This operation may not be invoked if this RTC is Active in
     *   the indicated execution context. Otherwise, it shall fail with
     *   ReturnCode_t::PRECONDITION_NOT_MET.
     *
     * @endif
     */
    ReturnCode_t detach_context( long exec_handle);

    /*!
     * @if jp
     * @brief 
     * @else
     * @brief get_context
     *
     * @section Description
     *
     * Obtain a reference to the execution context represented by the
     * given handle.
     *
     * @section Semantics
     *
     * The mapping from handle to context is specific to a particular
     * RTC instance. The given handle must have been obtained by a
     * previous call to attach_context on this RTC.
     *
     * @endif
     */
    ExecutionContext get_context( long exec_handle);

    /*!
     * @if jp
     * @brief get_owned_contexts
     *
     * @section Description 
     *
     * ���� RTC ����ͭ���� ExecutionContext �Υꥹ�Ȥ�������롣
     * 
     * @else
     * @brief get_owned_contexts
     *
     * @section Description 
     *
     * This operation returns a list of all execution contexts owned
     * by this RTC.
     *
     * @endif
     */
    ExecutionContextList get_owned_contexts();

    /*!
     * @if jp
     * @brief get_participating_contexts
     *  
     * @section Description
     *
     * ���� RTC �����ä��Ƥ��뤹�٤Ƥ� ExecutionContext �Υꥹ�Ȥ�������롣
     *
     * @section Semantics
     *
     * ���Υꥹ�Ȥ˴ޤޤ��¹ԥ���ƥ����Ȥϡ�attach_context ���Ƥӽ�
     * ����뤴�Ȥˡ��ꥹ�Ȥ��ɲä��졢detach_context ���ƤӽФ���뤴
     * �Ȥˡ��ꥹ�Ȥ���������롣
     *
     * @else
     * @brief* get_participating_contexts
     *
     * @section Description
     *
     * This operation returns a list of all execution contexts in
     * which this RTC participates.
     *
     * @section Semantics
     *
     * Each call to attach_context causes the provided context to be
     * added to this list. Each call to detach_context causes the
     * provided context to be removed from this list.
     *
     * @endif
     */
    ExecutionContextList get_participating_contexts();
    
    /*!
     * @if jp
     * ### [����] RTC.idl �ˤϴޤޤ�Ƥ��ʤ���PIM�ˤϴޤޤ�Ƥ��롣
     * ### PIM����������
     *
     * @brief 
     * @else
     * @brief get_context_handle
     *
     * @section Description
     * 
     * This operation returns a handle that is associated with the given
     * execution context.
     *
     * @endif
     */
    long get_context_handle( ExecutionContext cxt);
  };
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief ExecutionKind
   * 
   * @sectioni Description
   *
   * The ExecutionKind enumeration defines the execution semantics
   * (see Section 5.3) of the RTCs that participate in an execution
   * context.
   *
   * @endif
   */
  enum ExecutionKind
  {
    /*!
     * @if jp
     * @brief 
     *
     * @else
     * @brief PERIODIC
     *
     * @section Description
     *
     * The participant RTCs are executing according to periodic
     * sampled data semantics (see Section 5.3.1).
     * 
     * @endif
     */
    PERIODIC,
    /*!
     * @if jp
     * @brief 
     *
     * @else
     * @brief EVENT_DRIVEN
     *
     * @section Description
     *
     * The participant RTCs are executing according to stimulus
     * response semantics (see Section 5.3.2).
     * 
     * @endif
     */
    EVENT_DRIVEN,
    /*!
     * @if jp
     * @brief 
     *
     * @else
     * @brief OTHER
     *
     * @section Description
     *
     * The participant RTCs are executing according to some semantics
     * not defined by this specification.
     * 
     * @endif
     */
    OTHER
  };
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief ExecutionContext
   *
   * @section Description
   *
   * An ExecutionContext allows the business logic of an RTC to be
   * decoupled from the thread of control in which it is executed. The
   * context represents a logical thread of control and is provided to
   * RTCs at runtime as an argument to various operations, allowing
   * them to query and modify their own state, and that of other RTCs
   * executing within the same context, in the lifecycle.  This
   * separation of concerns is important for two primary reasons:
   *
   * - Large number of components may collaborate tightly within a
   * single node or process. If each component were to run within its
   * own thread of control, the infrastructure may not be able to
   * satisfy the timeliness and determinism requirements of real-time
   * applications due to the large number of threads and the required
   * synchronization between them.
   *
   * - A single application may carry out a number of independent
   * tasks that require different execution rates. For example, it may
   * need to sample a sensor periodically at a very high rate and update a
   * user interface at a much lower rate.
   *
   * @section Semantics
   *
   * The state machine of an ExecutionContext has two parts. The
   * behavior of the ExecutionContext itself is defined by the upper
   * region in the above figure. The behavior of the RTCs that
   * participate in the context is defined by the lower region. The
   * contents of that region are displayed in more detail in Figure
   * 5.5 in Section 5.2.2.2.  Ownership and Participation Each
   * execution context is owned by a single RTC and may be used to
   * execute that RTC and the RTCs contained within it, directly or
   * indirectly. An RTC that owns one or more execution contexts is
   * known as an autonomous RTC.  An autonomous RTC and some subset of
   * the RTCs within it (to be defined by the application developer)
   * shall be executed by the infrastructure according to the context��
   * s execution kind, which defines when each RTC��s operations will
   * be invoked when and in which order. These RTCs are said to
   * participate in the context. The available execution kinds are
   * described in Section 5.2.2.7.  The relationship between RTCs and
   * execution contexts may be many-to-many in the general case:
   * multiple RTCs may be invoked from the same execution context, and
   * a single RTC may be invoked from multiple contexts. In the case
   * where multiple RTCs are invoked from the same context, starting
   * or stopping the context shall result in the corresponding
   * lifecycle transitions for all of those components.
   *
   * @section Logical and Physical Threads
   *
   * Although an execution context represents a logical thread of
   * control, the choice of how it maps to a physical thread shall be
   * left to the application��s deployment
   * environment. Implementations may elect to associate contexts with
   * threads with a one-to-one mapping, to serve multiple contexts
   * from a single thread, or by any other means. In the case where a
   * given RTC may be invoked from multiple contexts, concurrency
   * management is implementation-dependent.
   *
   * @endif
   */
  interface ExecutionContext
  {
    
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief is_running
     * @section Description
     *
     * This operation shall return true if the context is in the
     * Running state.
     *
     * @section Semantics
     *
     * While the context is Running, all Active RTCs participating in
     * the context shall be executed according to the context��s
     * execution kind.  
     *
     * @endif
     */
    bool is_running();
    
    /*!
     * @if jp
     * @brief 
     * @else
     *
     * @brief start
     *
     * @section Description
     *
     * Request that the context enter the Running state. Once the
     * state transition occurs, the ComponentAction::on_startup
     * operation (see Section 5.2.2.4.3) will be invoked.  @section *
     *
     * @section Semantics
     *
     * An execution context may not be started
     * until the RT components that participate in it have been
     * initialized. An execution context may be started and stopped
     * multiple times.
     *
     * @section Constraints
     *
     * - This operation shall fail with
     * ReturnCode_t::PRECONDITION_NOT_MET if the context is not in the
     * Stopped state.
     *
     * - This operation shall fail with
     * ReturnCode_t::PRECONDITION_NOT_MET if any of the participating
     * components are not in their Alive state.
     *
     * @endif
     */
    ReturnCode_t start();
    
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief stop
     *
     * @section Description
     *
     * Request that the context enter the Stopped state. Once the
     * transition occurs, the ComponentAction::on_shutdown operation
     * (see Section 5.2.2.4.4) will be invoked.
     *
     * @section Semantics
     *
     * An execution context must be stopped before the RT components
     * that participate in it are finalized.
     *
     * An execution context may be started and stopped multiple times.
     *
     * @section Constraints
     *
     * - This operation shall fail with
     * ReturnCode_t::PRECONDITION_NOT_MET if the context is not in the
     * Running state.
     *
     * @endif
     */
    ReturnCode_t stop();
    
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief get_rate
     *
     * @section Description
     *
     * This operation shall return the rate (in hertz) at which its
     * Active participating RTCs are being invoked.
     *
     * @section Semantics
     *
     * An implementation is permitted to perform some periodic or
     * quasi-periodic processing within an execution context with an
     * ExecutionKind other than PERIODIC. In such a case, the result
     * of this operation is implementation-defined. If no periodic
     * processing of any kind is taking place within the context, this
     * operation shall fail as described in Section 5.2.1.
     *
     * @section Constraints
     *
     * - If the context has an ExecutionKind of PERIODIC, this
     *   operation shall return a rate greater than zero.
     *
     * @endif
     */
    double get_rate();
    
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief set_rate
     *
     * @section Description
     *
     * This operation shall set the rate (in hertz) at which this
     * context��s Active participating RTCs are being called.
     *
     * @section Semantics
     *
     * If the execution kind of the context is PERIODIC, a rate change
     * shall result in the invocation of on_rate_changed on any RTCs
     * realizing DataFlowComponentAction that are registered with any
     * RTCs participating in the context.  An implementation is
     * permitted to perform some periodic or quasi-periodic processing
     * within an execution context with an ExecutionKind other than
     * PERIODIC. If such is the case, and the implementation reports a
     * rate from get_rate, this operation shall set that rate
     * successfully provided that the given rate is valid. If no
     * periodic processing of any kind is taking place within the
     * context, this operation shall fail with
     * ReturnCode_t::UNSUPPORTED.
     *
     * @section Constraints
     *
     * - The given rate must be greater than zero. Otherwise, this
     * operation shall fail with ReturnCode_t::BAD_PARAMETER.
     *
     * @endif
     */
    ReturnCode_t set_rate( double rate);
    
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief add_component
     *
     * @section Description
     *
     * The operation causes the given RTC to begin participating in
     * the execution context.
     *
     * @section Semantics
     *
     * The newly added RTC will receive a call to
     * LightweightRTComponent::attach_context (see Section 5.2.2.2.5)
     * and then enter the Inactive state. 
     * 
     * @section Constraints
     * 
     * - If the ExecutionKind of this context is PERIODIC, the RTC
     * must be a data flow component (see Section 5.3.1.1).
     * Otherwise, this operation shall fail with
     * ReturnCode_t::PRECONDITION_NOT_MET.
     * 
     * - If the ExecutionKind of this context is EVENT_DRIVEN, the RTC
     * must be an FSM participant (see Section 5.3.2.3).  Otherwise,
     * this operation shall fail with
     * ReturnCode_t::PRECONDITION_NOT_MET.
     *
     * @endif
     */
    ReturnCode_t add_component( LightweightRTObject comp);
    
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief remove_component
     *
     * @section Description
     *
     * This operation causes a participant RTC to stop participating
     * in the execution context.
     *
     * @section Semantics
     *
     * The removed RTC will receive a call to
     * LightweightRTComponent::detach_context (see Section 5.2.2.2.6).
     *
     * @section Constraints
     *
     * - If the given RTC is not currently participating in the
     * execution context, this operation shall fail with
     * ReturnCode_t::BAD_PARAMETER.
     * 
     * - An RTC must be deactivated before it can be removed from an
     * execution context. If the given RTC is participating in the
     * execution context but is still in the Active state, this
     * operation shall fail with ReturnCode_t::PRECONDITION_NOT_MET.
     *
     * @endif
     */
    ReturnCode_t remove_component( LightweightRTObject comp);
    
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief activate_component
     *
     * @section Description
     *
     * The given participant RTC is Inactive and is therefore not
     * being invoked according to the execution context��s execution
     * kind. This operation shall cause the RTC to transition to the
     * Active state such that it may subsequently be invoked in this
     * execution context.
     *
     * @section Semantics
     *
     * The callback on_activate shall be called as a result of calling
     * this operation. This operation shall not return until the
     * callback has returned, and shall result in an error if the
     * callback does.  The following figure is a non-normative example
     * sequence diagram for activate_component.
     *
     * @section Constraints
     *
     * - An execution context can only activate its participant
     *   components. If the given RTC is not participating in the
     *   execution context, this operation shall fail with
     *   ReturnCode_t::BAD_PARAMETER.
     *
     * - An RTC that is in the Error state cannot be activated until
     *   after it has been reset. If the given RTC is in the Error
     *   state, this operation shall fail with
     *   ReturnCode_t::PRECONDITION_NOT_MET.
     *
     * - This operation shall fail with ReturnCode_t::BAD_PARAMETER if
     *   the given component is not in its Alive state.
     *
     * @endif
     */
    ReturnCode_t activate_component( LightweightRTObject comp);
    
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief deactivate_component
     *
     * @section Description
     *
     * The given RTC is Active in the execution context. Cause it to
     * transition to the Inactive state such that it will not be
     * subsequently invoked from the context unless and until it is
     * activated again.
     *
     * @section Semantics
     *
     * The callback on_deactivate shall be called as a result of
     * calling this operation. This operation shall not return until
     * the callback has returned, and shall result in an error if the
     * callback does.  The following figure is a non-normative example
     * sequence diagram for deactivate_component.
     *
     * @section Constraints
     *
     * - An execution context can only deactivate its participant
     * components. If the given RTC is not participating in the
     * execution context, this operation shall fail with
     * ReturnCode_t::BAD_PARAMETER.
     *
     * - This operation shall fail with ReturnCode_t::BAD_PARAMETER if
     * the given component is not in its Alive state.
     *
     * @endif
     */
    ReturnCode_t deactivate_component( LightweightRTObject comp);
    
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief reset_component
     *
     * @section Description
     *
     * Attempt to recover the RTC when it is in Error.
     *
     * @section Semantics
     *
     * The ComponentAction::on_reset callback shall be invoked. This
     * operation shall not return until the callback has returned, and
     * shall result in an error if the callback does. If possible, the
     * RTC developer should implement that callback such that the RTC
     * may be returned to a valid state.  * If this operation fails,
     * the RTC will remain in Error.
     *
     * @section Constraints
     *
     * - An RTC may only be reset in an execution context in which it
     *   is in error. If the RTC is not in Error in the identified
     *   context, this operation shall fail with
     *   ReturnCode_t::PRECONDITION_NOT_MET. However, that failure shall
     *   not cause the RTC to enter the Error state.
     *
     * - An RTC may not be reset while in the Created state. Any
     *   attempt to invoke this operation while the RTC is in that state
     *   shall fail with ReturnCode_t::PRECONDITION_NOT_MET. However,
     *   that failure shall not cause the RTC to enter the Error state.
     *
     * @endif
     */
    ReturnCode_t reset_component( LightweightRTObject comp);
    
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief get_component_state
     *
     * @section Description
     *
     * This operation shall report the LifeCycleState of the given
     * participant RTC.
     *
     * @section Constraints
     *
     * - The given RTC must be Alive.
     *
     * - The given RTC must be a participant in the target execution context.
     *
     * - The LifeCycleState returned by this operation shall be one of
     *   LifeCycleState::INACTIVE, ACTIVE, or ERROR.
     *
     * @endif
     */
    LifeCycleState get_component_state( LightweightRTObject comp);
    
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief get_kind
     *
     * @section Description
     *
     * This operation shall report the execution kind of the execution
     * context.
     *
     * @endif
     */
    ExecutionKind get_kind();
  };
  
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief DataFlowComponentAction
   *
   * @section Description
   *
   * DataFlowComponentAction is a companion to ComponentAction (see
   * Section 5.2.2.4) that provides additional callbacks for
   * intercepting the two execution passes defined in Section
   * 5.3.1.1.2.
   *
   * @endif
   */
  interface DataFlowComponentAction
  {
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief on_execute
     * @section Description
     *
     * This operation will be invoked periodically at the rate of the
     * given execution context as long as the following conditions
     * hold:
     *
     * - The RTC is Active.
     *
     * - The given execution context is Running.
     *
     * @section Semantics
     *
     This callback occurs during the first execution pass.
     *
     * @section Constraints
     *
     * - The execution context of the given context shall be PERIODIC.
     *
     * @endif
     */
    ReturnCode_t on_execute( long exec_handle);
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief on_state_update
     *
     * @section Description
     *
     * This operation will be invoked periodically at the rate of the
     * given execution context as long as the following conditions hold:
     *
     * - The RTC is Active.
     *
     * - The given execution context is Running.
     *
     * @section Semantics
     *
     * This callback occurs during the second execution pass.
     *
     * @section Constraints
     *
     * - The execution context of the given context shall be PERIODIC.
     *
     *
     * @endif
     */
    ReturnCode_t on_state_update( long exec_handle);
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief on_rate_changed
     *
     * @section Description
     *
     * This operation is a notification that the rate of the indicated
     * execution context (see Section 5.2.2.6.4) has changed.
     *
     * @section Constraints
     *
     * - The execution context of the given context shall be PERIODIC.
     *
     *
     * @endif
     */
    ReturnCode_t on_rate_changed( long exec_handle);
  };
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief dataFlowComponent
   *
   * @section Description
   *
   * The dataFlowComponent stereotype may be applied to a component
   * type to indicate that its instances should be executed in sorted
   * order by a periodic execution context.
   *
   * @section Constraints
   *
   * - An instance of a component extended by the dataFlowComponent
   *   stereotype must participate in at least one * execution context
   *   of kind PERIODIC, which shall also be used for the execution of
   *   any contained data flow components.
   *
   * - A component extended by dataFlowComponent must realize the
   *   interface DataFlowComponentAction.
   *
   *
   * @endif
   */
  interface DataFlowComponent
    extends LightweightRTObject, DataFlowComponentAction
  {
  };
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief fsm
   *
   * @section Description
   *
   * Applying the fsm stereotype to a component implies the ability to
   * define component-specific states and transitions.
   *
   * @section Semantics
   *
   * In creating a state machine such as is depicted in Figure 5.22,
   * the RTC developer is implicitly defining the Active state to be a
   * submachine state.  * The BehaviorStateMachines package described
   * in [UML] is considered the normative definition of a state
   * machine.
   *
   * @endif
   */
  interface Fsm
    extends LightweightRTObject
  {
  };
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief FsmParticipantAction
   *
   * @section Description
   *
   * FsmParticipantAction is companion to ComponentAction (see Section
   * 5.2.2.4) that is intended for use with FSM participant RTCs. It
   * adds a callback for the interception of state transitions, state
   * entries, and state exits.
   *
   * @endif
   */
  interface FsmParticipantAction
  {
    
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief on_action
     *
     * @section Description
     *
     * The indicated FSM participant RTC has been invoked as a result
     * of a transition, state entry, or state exit in its containing
     * FSM.
     *
     * @section Constraints
     *
     * - The given execution context shall be of kind EVENT_DRIVEN.
        *
     * @endif
     */
    ReturnCode_t on_action( long exec_handle);
  };
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief 
   * @endif
   */
  interface FsmParticipant
    extends LightweightRTObject, FsmParticipantAction
  {
  };
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief Mode
   *
   * @section Description
   *
   * Each mode defined by a given RTC shall be represented by an
   * instance of Mode.
   *
   * @endif
   */
  interface Mode
  {
  };
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief ModeCapable
   *
   * @section Description
   *
   * The ModeCapable interface provides access to an object��s modes
   * and a means to set the current mode.
   *
   * @section Semantics
   *
   * A given RTC may support multiple modes as well as multiple
   * execution contexts. In such a case, a request for a mode change
   * (e.g., from "cruise control on" to "cruise control off") may
   * come asynchronously with respect to one or more of those
   * execution contexts. The mode of an RTC may therefore be observed
   * to be different from one execution context to another.  - A mode
   * is pending in a given execution context when a mode change has
   * been requested but the new mode has not yet been observed by that
   * context.
   *
   * - The new mode has been committed in a given execution context
   *   when the context finally observes the new mode.
   *
   * - The new mode has stabilized once it has been committed in all
   *   execution contexts in which the RTC participates.
   *
   * Figure 5.26 depicts a state machine that describes mode
   * changes. Each parallel region in the composite state Mode Pending
   * represents an execution context. The trigger "sample" within
   * that state is considered to have occurred: - ��just before the
   * next call to on_execute (see Section 5.3.1.2.1) in the case where
   * immediate is false and the execution kind is PERIODIC, ��
   *
   * - ��just before the processing of the next stimulus in the case
   *   where immediate is false and the execution kind is
   *   EVENT_DRIVEN, or ��- ��immediately in all other cases.
   *
   * @endif
   */
  interface ModeCapable
  {
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief get_default_mode
     *
     * @section Description
     *
     * This operation shall return the mode in which the RTC shall be
     * when no other mode has been set.
     *
     * @section Constraints
     *
     * - This operation shall not return nil.
     *
     * @endif
     */
    Mode get_default_mode();
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief get_current_mode
     *
     * @section Description
     *
     * This operation shall return the last mode to have
     * stabilized. If no mode has been explicitly set, the current
     * mode shall be the default mode.
     *
     * @section Constraints
     *
     * - This operation shall never return nil.
     *
     * @endif
     */
    Mode get_current_mode();
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief get_current_mode_in_context
     *
     * @section Description
     *
     * This operation returns the current mode of the component as
     * seen by the indicated execution context.
     *
     * @section Semantics
     *
     * The manner in which this property changes is described in Figure 5.26.
     *
     * @endif
     */
    Mode get_current_mode_in_context( ExecutionContext exec_context);
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief get_pending_mode
     *
     * @section Description
     *
     * This operation shall return the last mode to have been passed
     * to set_mode that has not yet stabilized. Once the RTC��s mode
     * has stabilized, this operation shall return nil.
     *
     * @endif
     */
    Mode get_pending_mode();
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief get_pending_mode_in_context
     *
     * @section Description
     *
     * If the last mode to be requested by a call to set_mode is
     * different than the current mode as seen by the indicated
     * execution context (see get_current_mode_in_context), this
     * operation returns the former. If the requested mode has already
     * been seen in that context, it returns nil.
     *
     * @section Semantics
     *
     * See Figure 5.26 for a description of how the pending mode
     * relates to the current mode within a given execution context.
     *
     * @endif
     */
    Mode get_pending_mode_in_context( ExecutionContext exec_context);
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief set_mode
     *
     * @section Description
     *
     * This operation shall request that the RTC change to the indicated mode.
     *
     * @section Semantics
     *
     * Usually, the new mode will be pending in each execution context
     * in which the component executes until the next sample period
     * (if the execution kind is PERIODIC); at that point it will
     * become the current mode in that context and there will no
     * longer be a pending mode. However, in some cases it is
     * important for a mode change to take place immediately; for
     * example, a serious fault has occurred and the component must
     * enter an emergency mode to ensure fail-safe behavior in a
     * safety-critical system. In such a case, immediate should be
     * true and the mode change will take place in all contexts
     * without waiting for the next sample period.
     *
     * @endif
     */
    ReturnCode_t set_mode( Mode new_mode,
			   bool immediate);
  };
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief MultiModeComponentAction
   *
   * MultiModeComponentAction is a companion to ComponentAction that
   is realized by RTCs that support multiple modes.
   *
   * @endif
   */
  interface MultiModeComponentAction
  {
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief on_mode_changed
     *
     * @section Description
     *
     * This callback is invoked each time the observed mode of a
     * component has changed with respect to a particular execution
     * context.
     *
     * @section Semantics
     *
     * If the context is PERIODIC, this callback shall come before the
     * next call to on_execute (see Section 5.3.1.2.1) within that
     * context.  The new mode can be retrieved with
     * get_current_mode_in_context. If the result is the same as the
     * result of get_current_mode, the mode has stabilized.
     *
     * @endif
     */
    ReturnCode_t on_mode_changed( long exec_handle);
  };
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief 
   * @endif
   */
  interface MultiModeObject
    extends LightweightRTObject, ModeCapable, MultiModeComponentAction
  {
  };
  
  interface RTObject;
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief PortInterfacePolarity
   *
   * @section Description
   *
   * The PortInterfacePolarity enumeration identifies exposed
   * interface instances as provided or required.  @endif
   */
  enum PortInterfacePolarity
  {
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief PROVIDED
     *
     * @section Description
     *
     * The target interface is provided as an output by the target
     * port.
     *
     * @endif
     */
    PROVIDED,
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief REQUIRED
     *
     * @section Description
     *
     * The target interface is required as an input by the target port.
     *
     * @endif
     */
    REQUIRED
  };
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief PortInterfaceProfile
   *
   * @section Description
   *
   * PortInterfaceProfile describes an instance of a particular
   * interface as it is exposed by a particular port. These objects
   * are referred to below as the "target interface" and "target
   * port" respectively.
   *
   * @endif
   */
  struct PortInterfaceProfile
  {
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief instance_name
     *
     * @section Description
     *
     * This attribute stores the name of the target interface instance.
     *
     * @endif
     */
    string instance_name;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief type_name
     *
     * @section Description
     *
     * This attribute stores the name of the target interface type.
     *
     * @endif
     */
    string type_name;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief polarity
     *
     * @section Description
     *
     * This attribute indicates whether the target interface instance
     * is provided or required by the RTC.
     *
     * @endif
     */
    PortInterfacePolarity polarity;
  };
  
    sequence<PortInterfaceProfile> PortInterfaceProfileList;
  
  interface PortService;
    sequence<PortService> PortServiceList;
    sequence<RTObject> RTCList;
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief ConnectorProfile
   *
   * @section Description
   *
   * The ConnectorProfile contains information about a connection
   * between the ports of collaborating RTCs.
   *
   * @endif
   */
  struct ConnectorProfile
  {
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief name
     *
     * @section Description
     *
     * This attribute contains the name of this connection.
     *
     * @endif
     */
    string name;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief connector_id
     *
     * @section Description
     *
     * Each connector has a unique identifier that is assigned when
     * connection is established. This attribute stores that
     * identifier.
     *
     * @endif
     */
    string connector_id;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief ports
     *
     * @section Description
     *
     * This field stores references to all ports connected by the
     * target connector.
     *
     * @endif
     */
    PortServiceList ports;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief properties
     *
     * @section Description
     *
     * This attribute contains additional properties of the connection.
     *
     * @section Semantics
     *
     * This attribute provides implementations the opportunity to
     * describe additional characteristics of a particular connection
     * that are outside of the scope of this specification.
     *
     * @endif
     */
    SDOPackage::NVList properties;
  };
  
    sequence<ConnectorProfile> ConnectorProfileList;
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief PortProfile
   *
   * @section Description
   *
   * A PortProfile describes a port of an RTC (referred to as the
   * "target" RTC). This port is referred to as the "target" port.
   * From this profile, other components and tools can obtain Port��s
   * name, type, object reference, and so on.
   *
   * @endif
   */
  struct PortProfile
  {
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief name
     *
     * @section Description
     *
     * This attribute contains the name of the target port.
     *
     * @section Semantics
     *
     * Ports owned by an RTC are distinguished by their
     * names. Therefore, this name should be unique within the target
     * RTC.
     *
     * @endif
     */
    string name;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief interfaces
     *
     * @section Description
     *
     * This attribute contains the name and polarity of each interface
     * exposed by the target port.
     *
     * @endif
     */
    PortInterfaceProfileList interfaces;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief port_ref
     *
     * @section Description
     *
     * This attributes contains a reference to the target port.
     *
     * @endif
     */
    PortService port_ref;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief connector_profiles
     *
     * @section Description
     *
     * This attribute contains a collection of profiles describing the
     * connections to the target port.
     *
     * @endif
     */
    ConnectorProfileList connector_profiles;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief owner
     *
     * @section Description
     *
     * This attribute contains a reference to the target RTC.
     *
     * @endif
     */
    RTObject owner;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief properties
     *
     * @section Description
     *
     * This attribute contains additional properties of the port.
     *
     * @section Semantics
     *
     * This attribute provides implementations the opportunity to
     * describe additional characteristics of a particular port that
     * are otherwise outside of the scope of this specification.
     *
     * @endif
     */
    SDOPackage::NVList properties;
  };
  
    sequence<PortProfile> PortProfileList;
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief 
   * @endif
   */
  struct ExecutionContextProfile
  {
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief kind
     *
     * @section Description
     *
     * This attribute stores the context��s ExecutionKind.
     *
     * @endif
     */
    ExecutionKind kind;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief rate
     *
     * @section Description
     *
     * This attribute stores execution rate.
     *
     * @section Semantics
     *
     * If the execution kind is not PERIODIC, the value here may not
     * be valid (and should be negative in that case). See
     * ExecutionContext::get_rate (see Section 5.2.2.6.4) and set_rate
     * (see Section 5.2.2.6.5) for more information.
     *
     * @endif
     */
    double rate;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief owner
     *
     * @section Description
     *
     * This attribute stores a reference to the RTC that owns the context.
     *
     * @endif
     */
    RTObject owner;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief participants
     *
     * @section Description
     *
     * This attribute stores references to the context��s participant RTCs.
     *
     * @endif
     */
    RTCList participants;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief properties
     *
     * @section Description
     *
     * This attribute contains additional properties of the execution
     * context.
     *
     * @section Semantics
     *
     * This attribute provides implementations the opportunity to
     * describe additional characteristics of a particular execution
     * context that are outside the scope of this specification.
     *
     * @endif
     */
    SDOPackage::NVList properties;
  };
  
    sequence<ExecutionContextProfile>
  ExecutionContextProfileList;
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief FsmObject
   *
   * @section Description
   *
   * The FsmObject interface allows programs to send stimuli to a
   * finite state machine, possibly causing it to change states.
   *
   * @endif
   */
  interface FsmObject
  {
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief send_stimulus
     *
     * @section Description
     *
     * Send a stimulus to an FSM that realizes this interface.
     *
     * @section Semantics
     *
     * If the stimulus corresponds to any outgoing transition of the
     * current state, that transition shall be taken and the state
     * shall change. Any FSM participants associated with the exit of
     * the current state, the transition to the new state, or the
     * entry to the new state shall be invoked. If the stimulus does
     * not correspond to any such transition, this operation shall
     * succeed but have no effect.  
     *
     * If the given execution context is a non-nil reference to a
     * context in which this FSM participates, the transition shall be
     * executed in that context. If the argument is nil, the FSM shall
     * choose an EVENT_DRIVEN context in which to execute the
     * transition. If the argument is non-nil, but this FSM does not
     * participate in the given context, this operation shall fail
     * with * ReturnCode_t::BAD_PARAMETER.
     *
     * @section Constraints
     *
     * - The given execution context shall be of kind EVENT_DRIVEN.
     *
     * @endif
     */
    ReturnCode_t send_stimulus( string message,
			        long exec_handle);
  };
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief FsmBehaviorProfile
   *
   * @section Description
   *
   * FsmBehaviorProfile represents the association of an FSM
   * participant with a transition, state entry, or state exit in an
   * FSM.
   *
   * @section Semantics
   *
   * The assignment of identifiers to particular transitions, state
   * entries, or state exits is implementation-dependent.
   *
   * @endif
   */
  struct FsmBehaviorProfile
  {
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief action_component
     *
     * @section Description
     *
     * This attribute stores a reference to the FSM participant that
     * is invoked when the containing Fsm receives a message
     * distinguished by id.
     *
     * @endif
     */
    FsmParticipantAction action_component;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief id
     *
     * @section Description
     *
     * This attribute stores the message identifier.
     *
     * @endif
     */
    string id;
  };
  
    sequence<FsmBehaviorProfile> FsmBehaviorProfileList;
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief FsmProfile
   *
   * @section Description
   *
   * The FsmProfile describes the correspondence between an FSM and
   * its contained FSM participants. This Profile is necessary for
   * Stimulus Response Processing.
   *
   * @endif
   */
  struct FsmProfile
  {
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief behavior_profiles
     *
     * @section Description
     *
     * This attribute lists the correspondences between an FSM and its
     * contained FSM participants.
     *
     * @endif
     */
    FsmBehaviorProfileList behavior_profiles;
  };
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief FsmService
   *
   * @section Description
   *
   * The FsmService interface defines operations necessary for
   * Stimulus Response Processing as an SDO service.
   *
   * @endif
   */
  interface FsmService
    extends SDOPackage::SDOService
  {
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief get_fsm_profile
     *
     * @section Description
     *
     * Get the current state of the FSM.
     *
     * @section Semantics
     *
     * Modifications to the object returned by this operation will not
     * be reflected in the FSM until and unless set_fsm_profile is
     * called.
     *
     * @endif
     */
    FsmProfile get_fsm_profile();
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief set_fsm_profile
     *
     * @section Description
     *
     * This operation will be used to modify the behavior of an FSM as
     * described in Stimulus Response Processing.
     *
     * @endif
     */
    ReturnCode_t set_fsm_profile( FsmProfile fsm_profile);
  };
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief ComponentProfile
   *
   * @section Description
   *
   * ComponentProfile represents the static state of an RTC that is
   * referred to here as the "target" RTC.
   *
   * @endif
   */
  struct ComponentProfile
  {
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief instance_name
     *
     * @section Description
     *
     * This attribute shall contain the name of the target RTC instance.
     *
     * @section Semantics
     *
     * The instance_name should be unique among RTC instances
     * contained within the same containing component.
     *
     * @endif
     */
    string instance_name;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief type_name
     *
     * @section Description
     *
     * This attribute shall contain the name of the target RTC class.
     *
     * @section Semantics
     *
     * Each RTC class must have a name that is unique within an
     * application.
     *
     * @endif
     */
    string type_name;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief description
     *
     * @section Description
     *
     * This attribute shall briefly describe the target RTC for the
     * benefit of a human operator.
     *
     * @endif
     */
    string description;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief version
     *
     * @section Description
     *
     * This attribute shall contain the version number of the target
     * RTC class.
     *
     * @section Semantics
     *
     * The format of the version number is outside of the scope of
     * this specification.
     *
     * @endif
     */
    string version;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief vendor
     *
     * @section Description
     *
     * The name of the individual or organization that produced the
     * target RTC class.
     *
     * @endif
     */
    string vendor;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief category
     *
     * @section Description
     *
     * This attribute contains the name of a "category" or group to
     * which the target RTC belongs.
     *
     *
     * @endif
     */
    string category;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief port_profiles
     *
     * @section Description
     *
     * This attribute contains a list of PortProfiles that describe
     * the ports of the target RTC.
     *
     * @section Semantics
     *
     * There shall be a one-to-one correspondence between the members
     * of this list and the ports of the target RTC.
     *
     *
     * @endif
     */
    PortProfileList port_profiles;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief parent
     *
     * @section Description
     *
     * This attribute contains a reference to the RTC that contains
     * the target RTC instance. If the target RTC instance is not
     * owned by any other RTC, this field stores a nil reference.
     *
     * @endif
     */
    RTObject parent;
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief properties
     *
     * @section Description
     *
     * This attribute contains additional properties of the target RTC.
     *
     * @section Semantics
     *
     * This attribute provides implementations the opportunity to
     * describe additional characteristics of a particular RTC that
     * are otherwise outside of the scope of this specification.
     *
     * @endif
     */
    SDOPackage::NVList properties;
  };
  
    sequence<ComponentProfile> ComponentProfileList;
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief PortService
   *
   * @section Description
   *
   * An instance of the PortService interface represents a port (i.e.,
   * UML::Composite Structures::Ports::Port) of an RTC. It provides
   * operations that allow it to be connected to and disconnected from
   * other ports.
   *
   * @section Semantics
   *
   * A port service can support unidirectional or bidirectional
   * communication.  A port service may allow for a service-oriented
   * connection, in which other connected ports, invoke methods on
   * it. It may also allow for a data-centric connection, in which
   * data values are streamed in or out. In either case, the
   * connection is described by an instance of
   * ConnectorProfile. However, the behavioral contracts of such
   * connections are dependent on the interfaces exposed by the ports
   * and are not described normatively by this specification.
   *
   * @endif
   */
  interface PortService
    extends SDOPackage::SDOService
  {
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief get_port_profile
     *
     * @section Description
     *
     * This operation returns the PortProfile of the PortService.
     *
     * @endif
     */
    PortProfile get_port_profile();
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief get_connector_profiles
     *
     * @section Description
     *
     * This operation returns a list of the ConnectorProfiles of the
     * PortService.
     *
     * @endif
     */ 
    ConnectorProfileList get_connector_profiles();
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief get_connector_profiles
     *
     * @section Description
     *
     * This operation returns a list of the ConnectorProfiles of the
     * PortService.
     *
     * @endif
     */
    ConnectorProfile get_connector_profile( string connector_id);
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief connect
     *
     * @section Description
     *
     * This operation establishes connection between this port and the
     * peer ports according to given ConnectionProfile.
     *
     * @section Semantics
     *
     * A ConnectorProfile has a sequence of port references. This port
     * invokes the notify_connect operation of one of the ports
     * included in the sequence. It follows that the notification of
     * connection is propagated by the notify_connect operation with
     * ConnectorProfile. This operation returns ConnectorProfile
     * return value and returns ReturnCode_t as return codes.
     *
     * @endif
     */
    ReturnCode_t connect( ConnectorProfile connector_profile);
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief disconnect
     *
     * @section Description
     *
     * This operation destroys the connection between this port and
     * its peer ports using the ID that was given when the connection
     * was established.
     *
     * @section Semantics
     *
     * This port invokes the notify_disconnect operation of one of the
     * ports included in the sequence of the ConnectorProfile stored
     * when the connection was established. The notification of
     * disconnection is propagated by the notify_disconnect operation.
     *
     * @endif
     */
    ReturnCode_t disconnect( string connector_id);
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief disconnect_all
     *
     * @section Description
     *
     * This operation destroys all connection channels owned by the
     * PortService.
     *
     * @endif
     */
    ReturnCode_t disconnect_all();
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief notify_connect
     *
     * @section Description
     *
     * This operation notifies this PortService of the connection
     * between its corresponding port and the other ports and
     * propagates the given ConnectionProfile.
     *
     * @section Semantics
     *
     * A ConnectorProfile has a sequence of port references. This
     * PortService stores the ConnectorProfile and invokes the
     * notify_connect operation of the next PortService in the
     * sequence. As ports are added to the connector, PortService
     * references are added to the ConnectorProfile and provided to
     * the caller. In this way, notification of connection is
     * propagated with the ConnectorProfile.
     *
     * @endif
     */
    ReturnCode_t notify_connect( ConnectorProfile connector_profile);
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief notify_disconnect
     *
     * @section Description
     *
     * This operation notifies a PortService of a disconnection
     * between its corresponding port and the other ports. The
     * disconnected connector is identified by the given ID, which was
     * given when the connection was established.
     *
     * @section Semantics
     *
     * This port invokes the notify_disconnect operation of the next
     * PortService in the sequence of the ConnectorProfile that was
     * stored when the connection was established. As ports are
     * disconnected, PortService references are removed from the
     * ConnectorProfile. In this way, the notification of
     * disconnection is propagated by the notify_disconnect operation.
     *
     * @endif
     */
    ReturnCode_t notify_disconnect( string connector_id);
  };
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief ExecutionContextService
   *
   * @section Description
   *
   * An ExecutionContextService exposes an ExecutionContext as an SDO
   * service such that the context may be controlled remotely.
   *
   * @section Semantics
   *
   * Depending on the implementation, this interface may itself be an
   * execution context (that is, it may be passed to the operations of
   * ComponentAction) or it may represent a remote execution context
   * that is not of type ExecutionContextService.  @endif
   */
  interface ExecutionContextService
    extends ExecutionContext, SDOPackage::SDOService
  {
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief get_profile
     *
     * @section Description
     *
     * This operation provides a profile "descriptor" for the
     * execution context.
     *
     * @endif
     */
    ExecutionContextProfile get_profile();
  };
  
    sequence<ExecutionContextService>
  ExecutionContextServiceList;
  
  /*!
   * @if jp
   * @brief 
   * @else
   * @brief RTObject
   *
   * @section Description
   *
   * The RTObject interface defines the operations that all SDO-based
   * RTCs must provide. It is required by the rtComponent stereotype.
   *
   * @endif
   */
  interface RTObject
    extends  LightweightRTObject, SDOPackage::SDO
  {
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief get_component_profile
     *
     * @section Description
     *
     * This operation returns the ComponentProfile of the RTC.
     *
     * @endif
     */
    ComponentProfile get_component_profile();
    /*!
     * @if jp
     * @brief 
     * @else
     * @brief get_ports
     *
     * @section Description
     *
     * This operation returns a list of the RTCs ports.
     *
     * @endif
     */
    PortServiceList get_ports();
  };
};

#endif // RTC_IDL