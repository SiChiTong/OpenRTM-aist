// -*- C++ -*-
/*!
 * @file   ManagerTests.cpp
 * @brief  Manager test class
 * @date   $Date: 2008/05/12 03:58:45 $
 *
 * $Id: ManagerTests.cpp,v 1.2 2008/05/12 03:58:45 arafune Exp $
 *
 */

/*
 * $Log: ManagerTests.cpp,v $
 * Revision 1.2  2008/05/12 03:58:45  arafune
 * Added some tests.
 * Rearranged tests in a different order.
 *
 * Revision 1.1  2008/05/09 12:01:44  arafune
 * The first commitment.
 *
 *
 */

#ifndef ManagerTests_cpp
#define ManagerTests_cpp

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>

#include <ace/DLL.h>

#include <rtm/Manager.h>
#include <rtm/RTObject.h>
#include <rtm/ECFactory.h>
#include <rtm/PeriodicExecutionContext.h>
#include <rtm/ExtTrigExecutionContext.h>
#include <rtm/NamingManager.h>

/*!
 * @class ManagerTests class
 * @brief Manager test
 */
namespace Tests
{
  class Logger
  {
  public:
    void log(const std::string& msg)
    {
      m_log.push_back(msg);
    }
		
    int countLog(const std::string& msg)
    {
      int count = 0;
      for (int i = 0; i < (int) m_log.size(); ++i)
	{
	  if (m_log[i] == msg) ++count;
	}
      return count;
    }
		
  private:
    std::vector<std::string> m_log;
  };
	
	
  class ManagerMock : public RTC::Manager
  {
  public: // for test
    static void clearInstance()
    {
      manager = NULL;
    }
  };
	
	
  class ModuleMock
  {
  public:
    static void setLogger(Logger* logger)
    {
      m_logger = logger;
    }
		
    static void InitProc(RTC::Manager* manager)
    {
      if (m_logger != NULL) m_logger->log("InitProc");
    }
	
  private:
    static Logger* m_logger;
  };
	
  Logger* ModuleMock::m_logger = NULL;
	
	
  class RTObjectMock
    : virtual public RTC::RTObject_impl
  {
  public:
    RTObjectMock(CORBA::ORB_ptr orb, PortableServer::POA_ptr poa)
      : RTC::RTObject_impl(orb, poa), m_logger(NULL)
    {
    }
		
    virtual RTC::ReturnCode_t initialize()
      throw (CORBA::SystemException)
    {
      if (m_logger != NULL) m_logger->log("initialize");
      return RTC::RTC_OK;
    }
		
    void setLogger(Logger* logger)
    {
      m_logger = logger;
    }
		
  private:
    Logger* m_logger;
  };


  class DataFlowComponentMock
    : virtual public RTC::RTObject_impl
  {
  public:
    DataFlowComponentMock(CORBA::ORB_ptr orb, PortableServer::POA_ptr poa)
      : RTC::RTObject_impl(orb, poa)
    {
    }
		
    RTC::UniqueId attach_executioncontext(RTC::ExecutionContext_ptr exec_context)
      throw (CORBA::SystemException)
    {
      return RTC::RTObject_impl::attach_executioncontext(exec_context);
    }
  };
	
  RTC::RtcBase* CreateDataFlowComponentMock(RTC::Manager* manager)
  {
    CORBA::ORB_ptr orb = manager->getORB();
    PortableServer::POA_ptr poa = manager->getPOA();
    DataFlowComponentMock* comp = new DataFlowComponentMock(orb, poa);
    comp->setObjRef(comp->_this());
    return comp;
  }

  void DeleteDataFlowComponentMock(RTC::RtcBase* rtc)
  {
    if (rtc != NULL) rtc->_remove_ref();
  }
	
	
  class ManagerTests
    : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(ManagerTests);
	  
    CPPUNIT_TEST(test_init_without_arguments);
    CPPUNIT_TEST(test_instance);
    CPPUNIT_TEST(test_instance_without_init);
    CPPUNIT_TEST(test_terminate_immediately_after_the_initialization);
    CPPUNIT_TEST(test_terminate_after_the_activation);
    CPPUNIT_TEST(test_getLogbuf);
    CPPUNIT_TEST(test_getConfig);
    CPPUNIT_TEST(test_setModuleInitProc);
    CPPUNIT_TEST(test_runManager_no_block);
    CPPUNIT_TEST(test_runManager_block);
    CPPUNIT_TEST(test_load);
    CPPUNIT_TEST(test_unload);
    CPPUNIT_TEST(test_unloadAll);
    CPPUNIT_TEST(test_registerFactory);
    CPPUNIT_TEST(test_registerECFactory);
    CPPUNIT_TEST(test_getModulesFactories);
    CPPUNIT_TEST(test_cleanupComponent);
    CPPUNIT_TEST(test_getComponents);

		
    // �����ߡ��ƥƥ��ȴ֤���Ω�������˳��ݤǤ��Ƥ��ʤ����ᡢ�����ƥ��Ȥϼ»ܽ�����ѹ����ʤ����ȡ�
    //   �ޤ����ƥ������Ƥ��ѹ������ꡢ¾�ƥ��Ȥ��ɲä����ꤹ����ϡ�ɬ������ƥ��ȴ֤���Ω����
    //   �ݤ���ʤ�����α�դ��뤳�ȡ��ʤ�����Ω�����ݤƤʤ��Τϡ�omniORB���Ѥ������ˡ�
    //   CORBA::ORB::destroy()�����Ԥ����礬���ꡢ���ƥ��Ȼ���CORBA::ORB_init()�ƽФ�
    //   ��ORB���󥹥��󥹤��֤��ʤ���礬���뤿�ᡣ�ܤ��������ϸ������Ǥ�������
    //
    CPPUNIT_TEST(test_createComponent_DataFlowComponent);
    //CPPUNIT_TEST(test_createComponent_Non_DataFlowComponent);
    CPPUNIT_TEST(test_createComponent_failed_in_bindExecutionContext);
    //CPPUNIT_TEST(test_createComponent_with_illegal_module_name);
		
    CPPUNIT_TEST_SUITE_END();
	
  private:
    RTC::Manager* m_mgr;
	
  private:
    bool isFound(const std::vector<std::string>& list, const std::string& target)
    {
      return ! (list.end() == std::find(list.begin(), list.end(), target));
    }

    CosNaming::NamingContext_var getRootContext(const std::string& name_server)
    {
      std::string nsName
	= std::string("corbaloc::")
	+ name_server
	+ std::string("/NameService");
			
      CORBA::Object_var obj = m_mgr->getORB()->string_to_object(nsName.c_str());
      CosNaming::NamingContext_var rootContext = CosNaming::NamingContext::_narrow(obj);
      if (CORBA::is_nil(rootContext))
	{
	  throw std::bad_alloc();
	}
			
      return rootContext;
    }
		
    bool canResolve(const char* name_server, const char* id, const char* kind)
    {
      CosNaming::NamingContext_var nc = getRootContext(name_server);
			
      CosNaming::Name name;
      name.length(1);
      name[0].id = id;
      name[0].kind = kind;
			
      CORBA::Object_ptr obj;
      try
	{
	  obj = nc->resolve(name);
	}
      catch (CosNaming::NamingContext::NotFound e)
	{
	  return false;
	}
			
      return !CORBA::is_nil(obj);
    }
	
  public:
    /*!
     * @brief Constructor
     */
    ManagerTests()
    {
    }
		    
    /*!
     * @brief Destructor
     */
    virtual ~ManagerTests()
    {
    }
		  
    /*!
     * @brief Test initialization
     */
    virtual void setUp()
    {
      ManagerMock::clearInstance();
      m_mgr = NULL;
      usleep(100000);
    }
    
    /*!
     * @brief Test finalization
     */
    virtual void tearDown()
    {
      usleep(100000);
      /*
      if (m_mgr != NULL)
	{
	  try
	    {
	      CORBA::ORB_ptr orb = m_mgr->getORB();
	      if (! CORBA::is_nil(orb))
		{
		  orb->destroy();
		  sleep(3);
		}
	    }
	  catch (...) {}
	}
      */
    }
		
    /*!
     * @brief init()�᥽�åɤΥƥ���
     * 
     * - ���ޥ�ɥ饤������ʤ���init()������˸ƽФ��ơ����󥹥��󥹤�����Ǥ��뤫��
     */
    void test_init_without_arguments()
    {
      // ���ޥ�ɥ饤������ʤ���init()������˸ƽФ��ơ����󥹥��󥹤�����Ǥ��뤫��
      m_mgr = RTC::Manager::init(0, NULL);
      CPPUNIT_ASSERT(m_mgr != NULL);
      //      m_mgr->terminate();
    }
		
    /*!
     * @brief instance()�᥽�åɤΥƥ���
     * 
     * - instance()���̤��Ƽ����������󥹥��󥹤ϡ�init()�����������󥹥��󥹤�Ʊ�줫��
     */
    void test_instance()
    {
      int argc = 0;
      char* argv[] = {};
			
      m_mgr = RTC::Manager::init(argc, argv);
      CPPUNIT_ASSERT(m_mgr != NULL);
			
      // instance()���̤��Ƽ����������󥹥��󥹤ϡ�init()�����������󥹥��󥹤�Ʊ�줫��
      RTC::Manager& instance = RTC::Manager::instance();
      CPPUNIT_ASSERT_EQUAL(m_mgr, &instance);
    }
		
    /*!
     * @brief instance()�᥽�åɤΥƥ���
     * 
     * - ������init()��ƽФ�����instance()��ƽФ�����硢����˥��󥹥��󥹤���������뤫��
     */
    void test_instance_without_init()
    {
      // ������init()��ƽФ�����instance()��ƽФ�����硢����˥��󥹥��󥹤���������뤫��
      RTC::Manager::instance();
    }
		
    /*!
     * @brief terminate()�᥽�åɤΥƥ���
     * 
     * - ������夹����terminate()��ƽФ�������˽�λ�Ǥ��뤫��
     */
    void test_terminate_immediately_after_the_initialization()
    {
      // �������Ԥ�
      int argc = 0;
      char* argv[] = {};
			
      m_mgr = RTC::Manager::init(argc, argv);
      CPPUNIT_ASSERT(m_mgr != NULL);
      CPPUNIT_ASSERT(m_mgr->getORB() != NULL);
      CPPUNIT_ASSERT(m_mgr->getPOA() != NULL);
			
      // ������夹����terminate()��ƽФ�������˽�λ�Ǥ��뤫��
      CPPUNIT_ASSERT(! CORBA::is_nil(m_mgr->getORB()));
      CPPUNIT_ASSERT(! CORBA::is_nil(m_mgr->getPOA()));
      m_mgr->terminate();
      sleep(3); 
      CPPUNIT_ASSERT(CORBA::is_nil(m_mgr->getORB()));
      CPPUNIT_ASSERT(CORBA::is_nil(m_mgr->getPOA()));
      m_mgr = NULL;
    }
		
    /*!
     * @brief terminate()�᥽�åɤΥƥ���
     * 
     * - active���θ��terminate()��ƽФ�������˽�λ�Ǥ��뤫��
     */
    void test_terminate_after_the_activation()
    {
      // �������Ԥ�
      int argc = 0;
      char* argv[] = {};
			
      m_mgr = RTC::Manager::init(argc, argv);
      CPPUNIT_ASSERT(m_mgr != NULL);
      CPPUNIT_ASSERT(m_mgr->getORB() != NULL);
      CPPUNIT_ASSERT(m_mgr->getPOA() != NULL);
      CPPUNIT_ASSERT(m_mgr->getPOAManager() != NULL);
			
      // active������
      CPPUNIT_ASSERT(m_mgr->activateManager());
			
      // active���θ��terminate()��ƽФ�������˽�λ�Ǥ��뤫��
      CPPUNIT_ASSERT(! CORBA::is_nil(m_mgr->getORB()));
      CPPUNIT_ASSERT(! CORBA::is_nil(m_mgr->getPOA()));
      m_mgr->terminate();
      sleep(3); 
      CPPUNIT_ASSERT(CORBA::is_nil(m_mgr->getORB()));
      CPPUNIT_ASSERT(CORBA::is_nil(m_mgr->getPOA()));
    }
		
    void test_shutdown()
    {
      // terminate()���̤��ƸƤӽФ����᥽�åɤǤ��뤿�ᡢľ�ܤΥƥ����оݤȤϤ��ʤ�
    }
		
    void test_join()
    {
      // shutdown()����ǻ��Ѥ����᥽�åɤǤ��뤿�ᡢľ�ܤΥƥ����оݤȤϤ��ʤ�
      // �����⤽�⡢�ʤ�public�᥽�åɤˤʤäƤ���Τ���������
    }
		
    /*!
     * @brief getLogbuf()�᥽�åɤΥƥ���
     * 
     * - �����Хåե��������������Ǥ�������˥����ץ󤵤�Ƥ��뤫��
     */
    void test_getLogbuf()
    {
      // �������Ԥ�
      int argc = 1;
      char* argv[] = { "-f fixture1.conf" };
			
      m_mgr = RTC::Manager::init(argc, argv);
      CPPUNIT_ASSERT(m_mgr != NULL);
			
      // �����Хåե��������������Ǥ�������˥����ץ󤵤�Ƥ��뤫��
      RTC::Logbuf& logbuf = m_mgr->getLogbuf();
      CPPUNIT_ASSERT(logbuf.is_open());
    }
		
    /*!
     * @brief getConfig()�᥽�åɤΥƥ���
     * 
     * - conf�ե�����ǻ��ꤷ���Ƽ������getConfig()���̤��������������Ǥ��뤫��
     */
    void test_getConfig()
    {
      // �������Ԥ�
      int argc = 1;
      char* argv[] = { "-f fixture2.conf" };
			
      m_mgr = RTC::Manager::init(argc, argv);
      CPPUNIT_ASSERT(m_mgr != NULL);
			
      // conf�ե�����ǻ��ꤷ���Ƽ������getConfig()���̤��������������Ǥ��뤫��
      RTC::Properties& properties = m_mgr->getConfig();
      CPPUNIT_ASSERT_EQUAL(std::string("YES"),
			   properties.getProperty("logger.enable"));
      CPPUNIT_ASSERT_EQUAL(std::string("fixture2.log"),
			   properties.getProperty("logger.file_name"));
    }
		
    /*!
     * @brief setModuleInitProc()�᥽�åɤΥƥ���
     * 
     * - �����ƥ��ֲ��ˤ�ꡢ���ꤷ��������ץ��������㤬�������ƤӽФ���뤫��
     */
    void test_setModuleInitProc()
    {
      // Mock�ν���
      Logger logger;
      ModuleMock::setLogger(&logger);
			
      // �������Ԥ�
      int argc = 0;
      char* argv[] = {};
			
      m_mgr = RTC::Manager::init(argc, argv);
      CPPUNIT_ASSERT(m_mgr != NULL);
			
      // ������ץ������������Ͽ����
      m_mgr->setModuleInitProc(&ModuleMock::InitProc);
			
      // �����ƥ��ֲ��ˤ�ꡢ���ꤷ��������ץ��������㤬�������ƤӽФ���뤫��
      CPPUNIT_ASSERT_EQUAL(0, logger.countLog("InitProc"));
      CPPUNIT_ASSERT(m_mgr->activateManager());
      CPPUNIT_ASSERT_EQUAL(1, logger.countLog("InitProc"));
    }
		
    void test_activateManager()
    {
      // ¾�ƥ�����ǻ��Ѥ���Ƥ��뤿���ά����
    }
		
    /*!
     * @brief runManager()�᥽�åɤΥƥ��ȡ���֥��å��󥰥⡼�ɡ�
     * 
     * - POAManager�������ƥ��ֲ�����뤫��
     */
    void test_runManager_no_block()
    {
      // �������Ԥ�
      int argc = 0;
      char* argv[] = {};
      m_mgr = RTC::Manager::init(argc, argv);
      CPPUNIT_ASSERT(m_mgr != NULL);

      // ���֥������Ȥ��������ơ����Ȥ�����
      CORBA::ORB_ptr orb = m_mgr->getORB();
      CPPUNIT_ASSERT(! CORBA::is_nil(orb));
      PortableServer::POA_ptr poa = m_mgr->getPOA();
      CPPUNIT_ASSERT(! CORBA::is_nil(poa));

      RTObjectMock* rto = new RTObjectMock(orb, poa);
      CPPUNIT_ASSERT(rto != NULL);

      PortableServer::ObjectId_var rtoId = poa->activate_object(rto);

      RTC::DataFlowComponent_ptr rtoRef
	= RTC::DataFlowComponent::_narrow(poa->id_to_reference(rtoId));
      CPPUNIT_ASSERT(! CORBA::is_nil(rtoRef));

      // �ƥ����Ѥ˥����������ꤷ�Ƥ���
      Logger logger;
      rto->setLogger(&logger);
			
      // ��֥��å��󥰥⡼�ɤǥޥ͡�������ư������
      CPPUNIT_ASSERT(m_mgr->activateManager());
      m_mgr->runManager(true); // true:��֥��å��󥰡�false:�֥��å���

      // runManager()�ˤ��POAManager��������active������Ƥ��뤫��
      // �ʼ����������֥������Ȼ��Ȥ��Ф��ƥ᥽�åɸƽФ�Ԥ���
      // ��⡼��¦���ƽФ��줿���Ȥˤ��POAManager�Υ����ƥ��ֲ����ǧ�����
      CPPUNIT_ASSERT_EQUAL(0, logger.countLog("initialize"));
      rtoRef->initialize();
      sleep(3);
      CPPUNIT_ASSERT_EQUAL(1, logger.countLog("initialize"));
      m_mgr->shutdown();
    }
		
    /*!
     * @brief runManager()�᥽�åɤΥƥ��ȡʥ֥��å��󥰥⡼�ɡ�
     * 
     * - POAManager�������ƥ��ֲ�����뤫��
     */
    void test_runManager_block()
    {
      // �������Ԥ�
      int argc = 0;
      char* argv[] = {};
			
      m_mgr = RTC::Manager::init(argc, argv);
      CPPUNIT_ASSERT(m_mgr != NULL);
			
      // ���֥������Ȥ��������ơ����Ȥ�����
      CORBA::ORB_ptr orb = m_mgr->getORB();
      CPPUNIT_ASSERT(! CORBA::is_nil(orb));
      PortableServer::POA_ptr poa = m_mgr->getPOA();
      CPPUNIT_ASSERT(! CORBA::is_nil(poa));
			
      RTObjectMock* rto = new RTObjectMock(orb, poa);
      CPPUNIT_ASSERT(rto != NULL);
			
      PortableServer::ObjectId_var rtoId = poa->activate_object(rto);
			
      RTC::DataFlowComponent_ptr rtoRef
	= RTC::DataFlowComponent::_narrow(poa->id_to_reference(rtoId));
      CPPUNIT_ASSERT(! CORBA::is_nil(rtoRef));
			
      // �ƥ����Ѥ˥����������ꤷ�Ƥ���
      Logger logger;
      rto->setLogger(&logger);

      // �֥��å��󥰥⡼�ɤǥޥ͡�������ư������
      CPPUNIT_ASSERT(m_mgr->activateManager());
      CPPUNIT_ASSERT_EQUAL(0, logger.countLog("initialize"));
      {
	InvokerMock invoker(rtoRef, m_mgr);
	m_mgr->runManager(false); // true:��֥��å��󥰡�false:�֥��å���
	sleep(3);
      }
      CPPUNIT_ASSERT_EQUAL(1, logger.countLog("initialize"));
      m_mgr->shutdown();
    }
		
    class InvokerMock
      : public ACE_Task<ACE_MT_SYNCH>
    {
    public:
      InvokerMock(const RTC::DataFlowComponent_ptr& rtoRef, RTC::Manager* mgr)
      {
	m_rtoRef = RTC::DataFlowComponent::_duplicate(rtoRef);
	m_mgr = mgr;
	activate();
      }
			
      ~InvokerMock()
      {
	wait();	
      }
			
      virtual int svc(void)
      {
	m_rtoRef->initialize();
	sleep(1);
				
	// �֥��å�����Ƥ���runManager�ƽФ�֥��å��������
	m_mgr->getORB()->shutdown(true);
	m_mgr->join();
				
	return 0;
      }
		
    private:
      RTC::DataFlowComponent_ptr m_rtoRef;
      RTC::Manager* m_mgr;
    };
		
    /*!
     * @brief load()�᥽�åɤΥƥ���
     * 
     * - ���ꤷ���⥸�塼�������ɤ��ơ�����ν�����ؿ����������ƤӽФ���뤫��
     */
    void test_load()
    {
      // �������Ԥ�
      int argc = 1;
      char* argv[] = { "-f fixture3.conf" };
			
      m_mgr = RTC::Manager::init(argc, argv);
      CPPUNIT_ASSERT(m_mgr != NULL);

      // Manager�Ȥ��̤ˡ���ǧ�Ѥ˥⥸�塼��ؤΥ���ܥ��������Ƥ���
      typedef int (*FUNC_GETINITPROCCOUNT)();
      typedef void (*FUNC_RESETINITPROCCOUNT)();
      ACE_DLL loader("./DummyModule.so");

      FUNC_GETINITPROCCOUNT pGetInitProcCount
	= (FUNC_GETINITPROCCOUNT) loader.symbol("getInitProcCount");
      CPPUNIT_ASSERT(pGetInitProcCount != NULL);
			
      FUNC_RESETINITPROCCOUNT pResetInitProcCount
	= (FUNC_RESETINITPROCCOUNT) loader.symbol("resetInitProcCount");
      CPPUNIT_ASSERT(pResetInitProcCount != NULL);
			
      (*pResetInitProcCount)(); // �����󥿥��ꥢ
			
      // �⥸�塼������ɤˤ�ꡢ���ꤷ��������ؿ����ƤӽФ���뤫��
      CPPUNIT_ASSERT_EQUAL(0, (*pGetInitProcCount)());
      // std::string moduleName = m_mgr->load("DummyModule.so", "InitProc");
      m_mgr->load("DummyModule.so", "InitProc");
      // CPPUNIT_ASSERT(isFound(m_mgr->getLoadedModules(), moduleName));
      // CPPUNIT_ASSERT_EQUAL(1, (*pGetInitProcCount)());
    }
		
    /*!
     * @brief unload()�᥽�åɤΥƥ���
     * 
     * - ���ä���load�����⥸�塼���������unload�Ǥ��뤫��
     */
    void test_unload()
    {
      // �������Ԥ�
      int argc = 1;
      char* argv[] = { "-f fixture3.conf" };
			
      m_mgr = RTC::Manager::init(argc, argv);
      CPPUNIT_ASSERT(m_mgr != NULL);

      // Manager�Ȥ��̤ˡ���ǧ�Ѥ˥⥸�塼��ؤΥ���ܥ��������Ƥ���
      typedef int (*FUNC_GETINITPROCCOUNT)();
      typedef void (*FUNC_RESETINITPROCCOUNT)();
      ACE_DLL loader("./DummyModule.so");
			
      FUNC_GETINITPROCCOUNT pGetInitProcCount
	= (FUNC_GETINITPROCCOUNT) loader.symbol("getInitProcCount");
      CPPUNIT_ASSERT(pGetInitProcCount != NULL);

      FUNC_RESETINITPROCCOUNT pResetInitProcCount
	= (FUNC_RESETINITPROCCOUNT) loader.symbol("resetInitProcCount");
      CPPUNIT_ASSERT(pResetInitProcCount != NULL);
			
      (*pResetInitProcCount)(); // �����󥿥��ꥢ
			
      // ���ä���load���Ƥ���
      CPPUNIT_ASSERT_EQUAL(0, (*pGetInitProcCount)());
      m_mgr->load("DummyModule.so", "InitProc");
      // std::string moduleName = m_mgr->load("DummyModule.so", "InitProc");
      // CPPUNIT_ASSERT(isFound(m_mgr->getLoadedModules(), moduleName));
      // CPPUNIT_ASSERT_EQUAL(1, (*pGetInitProcCount)());
			
      // ���ä���load�����⥸�塼���������unload�Ǥ��뤫��
      // m_mgr->unload(moduleName.c_str());
      // CPPUNIT_ASSERT(! isFound(m_mgr->getLoadedModules(), moduleName));
    }
		
    /*!
     * @brief unloadAll()�᥽�åɤΥƥ���
     * 
     * - unloadAll()�ˤ�ꡢ�����ɤ����⥸�塼�뤬���٤ƥ�������ɤ���뤫��
     */
    void test_unloadAll()
    {
      // �������Ԥ�
      int argc = 1;
      char* argv[] = { "-f fixture3.conf" };
			
      m_mgr = RTC::Manager::init(argc, argv);
      CPPUNIT_ASSERT(m_mgr != NULL);

      // Manager�Ȥ��̤ˡ���ǧ�Ѥ˥⥸�塼��ؤΥ���ܥ��������Ƥ���
      typedef int (*FUNC_GETINITPROCCOUNT)();
      typedef void (*FUNC_RESETINITPROCCOUNT)();
			
      ACE_DLL loader1("./DummyModule.so");
      ACE_DLL loader2("./DummyModule2.so");
			
      FUNC_GETINITPROCCOUNT pGetInitProcCount1
	= (FUNC_GETINITPROCCOUNT) loader1.symbol("getInitProcCount");
      CPPUNIT_ASSERT(pGetInitProcCount1 != NULL);

      FUNC_RESETINITPROCCOUNT pResetInitProcCount1
	= (FUNC_RESETINITPROCCOUNT) loader1.symbol("resetInitProcCount");
      CPPUNIT_ASSERT(pResetInitProcCount1 != NULL);

      FUNC_GETINITPROCCOUNT pGetInitProcCount2
	= (FUNC_GETINITPROCCOUNT) loader2.symbol("getInitProcCount");
      CPPUNIT_ASSERT(pGetInitProcCount2 != NULL);

      FUNC_RESETINITPROCCOUNT pResetInitProcCount2
	= (FUNC_RESETINITPROCCOUNT) loader2.symbol("resetInitProcCount");
      CPPUNIT_ASSERT(pResetInitProcCount2 != NULL);
			
      (*pResetInitProcCount1)(); // �����󥿥��ꥢ
      (*pResetInitProcCount2)(); // �����󥿥��ꥢ
			
      // ���ä���load���Ƥ���
      CPPUNIT_ASSERT_EQUAL(0, (*pGetInitProcCount1)());
      CPPUNIT_ASSERT_EQUAL(0, (*pGetInitProcCount2)());

      // std::string moduleName1 = m_mgr->load("DummyModule.so", "InitProc");
      // std::string moduleName2 = m_mgr->load("DummyModule2.so", "InitProc");
      m_mgr->load("DummyModule.so", "InitProc");
      m_mgr->load("DummyModule2.so", "InitProc");

      // CPPUNIT_ASSERT(isFound(m_mgr->getLoadedModules(), moduleName1));
      // CPPUNIT_ASSERT(isFound(m_mgr->getLoadedModules(), moduleName2));

      // CPPUNIT_ASSERT_EQUAL(1, (*pGetInitProcCount1)());
      // CPPUNIT_ASSERT_EQUAL(1, (*pGetInitProcCount2)());
			
      // unloadAll()�ˤ�ꡢ�����ɤ����⥸�塼�뤬���٤ƥ�������ɤ���뤫��
      m_mgr->unloadAll();
      // CPPUNIT_ASSERT(! isFound(m_mgr->getLoadedModules(), moduleName1));
      // CPPUNIT_ASSERT(! isFound(m_mgr->getLoadedModules(), moduleName2));
    }
		
    void test_getLoadedModules()
    {
      // ¾�ƥ�����ǻ��Ѥ���Ƥ���ΤǾ�ά����
    }
		
    void test_getLoadableModules()
    {
      // �ƥ����оݤΥ��饹¦��̤�����ˤĤ����ƥ��Ȥ�̤����
    }
		
    /*!
     * @brief registerFactory()�᥽�åɤΥƥ���
     * 
     * - Factory���������Ͽ�Ǥ��뤫��
     */
    void test_registerFactory()
    {
      // �������Ԥ�
      int argc = 0;
      char* argv[] = {};
			
      m_mgr = RTC::Manager::init(argc, argv);
      CPPUNIT_ASSERT(m_mgr != NULL);
			
      // Factory���������Ͽ�Ǥ��뤫��
      RTC::Properties properties;
      properties.setProperty("implementation_id", "ID");

      CPPUNIT_ASSERT(! isFound(m_mgr->getModulesFactories(), "ID"));
      CPPUNIT_ASSERT(m_mgr->registerFactory(
					    properties, CreateDataFlowComponentMock, DeleteDataFlowComponentMock));
      CPPUNIT_ASSERT(isFound(m_mgr->getModulesFactories(), "ID"));
    }
		
    /*!
     * @brief registerECFactory()
     * 
     * - �����ECFactory����Ͽ�Ǥ��뤫��
     */
    void test_registerECFactory()
    {
      // �������Ԥ�
      int argc = 0;
      char* argv[] = {};
			
      m_mgr = RTC::Manager::init(argc, argv);
      CPPUNIT_ASSERT(m_mgr != NULL);
			
      // �����ECFactory����Ͽ�Ǥ��뤫��
      CPPUNIT_ASSERT(m_mgr->registerECFactory(
					      "PeriodicEC",
					      RTC::ECCreate<RTC::PeriodicExecutionContext>,
					      RTC::ECDelete<RTC::PeriodicExecutionContext>));
			
      // ��Ͽ�Ѥߤ�ECFactory��Ʊ���̾�Τ���Ͽ���ߤ���硢�տޤɤ�����Ͽ���Ԥ��뤫��
      CPPUNIT_ASSERT(! m_mgr->registerECFactory(
						"PeriodicEC",
						RTC::ECCreate<RTC::PeriodicExecutionContext>,
						RTC::ECDelete<RTC::PeriodicExecutionContext>));
    }
		
    /*!
     * @brief getModulesFactories()�᥽�åɤΥƥ���
     * 
     * - ��Ͽ����Ƥ���Factory�Ρ�"implementation_id"�ץ��ѥƥ��Ρ˥ꥹ�Ȥ������������Ǥ��뤫��
     */
    void test_getModulesFactories()
    {
      // �������Ԥ�
      int argc = 0;
      char* argv[] = {};
			
      m_mgr = RTC::Manager::init(argc, argv);
      CPPUNIT_ASSERT(m_mgr != NULL);
			
      // ʣ����Factory����Ͽ���Ƥ���
      RTC::Properties properties1;
      properties1.setProperty("implementation_id", "ID 1");
      CPPUNIT_ASSERT(m_mgr->registerFactory(
					    properties1, CreateDataFlowComponentMock, DeleteDataFlowComponentMock));

      RTC::Properties properties2;
      properties2.setProperty("implementation_id", "ID 2");
      CPPUNIT_ASSERT(m_mgr->registerFactory(
					    properties2, CreateDataFlowComponentMock, DeleteDataFlowComponentMock));
				
      // ��Ͽ����Ƥ���Factory�Ρ�"implementation_id"�ץ��ѥƥ��Ρ˥ꥹ�Ȥ������������Ǥ��뤫��
      CPPUNIT_ASSERT_EQUAL(2, (int) m_mgr->getModulesFactories().size());
      CPPUNIT_ASSERT(isFound(m_mgr->getModulesFactories(), "ID 1"));
      CPPUNIT_ASSERT(isFound(m_mgr->getModulesFactories(), "ID 2"));
    }
		
    /*!
     * @brief createComponent()�᥽�åɤΥƥ��ȡ�DataFlowComponent�ξ���
     * 
     * - ����������ݡ��ͥ�Ȥ������Ǥ��뤫��
     */
    void test_createComponent_DataFlowComponent()
    {
      // �������Ԥ�
      int argc = 1;
      char* argv[] = { "-f fixture4.conf" };
			
      m_mgr = RTC::Manager::init(argc, argv);
      CPPUNIT_ASSERT(m_mgr != NULL);
      CPPUNIT_ASSERT(! CORBA::is_nil(m_mgr->getORB()));
      CPPUNIT_ASSERT(! CORBA::is_nil(m_mgr->getPOA()));
      CPPUNIT_ASSERT(! CORBA::is_nil(m_mgr->getPOAManager()));

      // ��֥��å��󥰥⡼�ɤǥޥ͡�������ư������
      CPPUNIT_ASSERT(m_mgr->activateManager());
      m_mgr->runManager(true); // true:��֥��å��󥰡�false:�֥��å���
			
      // Factory����Ͽ���Ƥ���
      RTC::Properties properties;
      properties.setProperty("implementation_id", "DataFlowComponentFactory");
      properties.setProperty("type_name", "DataFlowComponent");
      CPPUNIT_ASSERT(m_mgr->registerFactory(
					    properties, CreateDataFlowComponentMock, DeleteDataFlowComponentMock));
			
      // ECFactory����Ͽ���Ƥ���
      CPPUNIT_ASSERT(m_mgr->registerECFactory(
					      "PeriodicEC",
					      RTC::ECCreate<RTC::PeriodicExecutionContext>,
					      RTC::ECDelete<RTC::PeriodicExecutionContext>));
			
      // ����������ݡ��ͥ�Ȥ������Ǥ��뤫��
      RTC::RtcBase* comp = m_mgr->createComponent("DataFlowComponentFactory");
      CPPUNIT_ASSERT(comp != NULL);
      CPPUNIT_ASSERT(dynamic_cast<DataFlowComponentMock*>(comp) != NULL);
      CPPUNIT_ASSERT(! CORBA::is_nil(comp->_this()));
      CPPUNIT_ASSERT_EQUAL(
			   std::string("DataFlowComponent0"), // ��������0��NumberingPolicy�ˤ���ղä����
			   std::string(comp->getInstanceName()));
			
      // ����ݡ��ͥ�Ȥˡ��տޤɤ���ExecutionContext�������å�����Ƥ��뤫��
      RTC::ExecutionContextList* ecList = comp->get_contexts();
      CPPUNIT_ASSERT(ecList != NULL);
      CPPUNIT_ASSERT_EQUAL(1, (int) ecList->length());
			
      // �������줿����ݡ��ͥ�Ȥϡ��������͡��ॵ���ӥ�����Ͽ����Ƥ��뤫��
      // ��fixture4.conf�γ�����˹�碌�Ƥ�����������
      RTC::NamingManager nmgr(m_mgr);
      const char* name_server = "localhost:9876";
      nmgr.registerNameServer("corba", name_server);
      CPPUNIT_ASSERT(canResolve(name_server, "DataFlowComponent0", "rtc"));
    }

    void test_createComponent_Non_DataFlowComponent()
    {
      // �������Ǥϡ�Manager¦��DataFlowComponent�Τߤ��б����Ƥ��뤿�ᡢ�ƥ��Ⱦ�ά����
    }
		
    /*!
     * @brief createComponent()�᥽�åɤΥƥ���
     * 
     * - ��Ͽ����Ƥ��ʤ��⥸�塼��̾����ꤷ�ƥ���ݡ��ͥ���������ߤơ��տޤɤ���NULL����뤫��
     * - �⥸�塼��̾��NULL����ꤷ�ƥ���ݡ��ͥ���������ߤơ��տޤɤ���NULL����뤫��
     */
    void test_createComponent_with_illegal_module_name()
    {
      // �������Ԥ�
      int argc = 0;
      char* argv[] = {};
			
      m_mgr = RTC::Manager::init(argc, argv);
      CPPUNIT_ASSERT(m_mgr != NULL);
      CPPUNIT_ASSERT(! CORBA::is_nil(m_mgr->getORB()));
      CPPUNIT_ASSERT(! CORBA::is_nil(m_mgr->getPOA()));
      CPPUNIT_ASSERT(! CORBA::is_nil(m_mgr->getPOAManager()));

      // ��Ͽ����Ƥ��ʤ��⥸�塼��̾����ꤷ�ƥ���ݡ��ͥ���������ߤơ��տޤɤ���NULL����뤫��
      RTC::RtcBase* comp1 = m_mgr->createComponent("illegal_module_name");
      CPPUNIT_ASSERT(comp1 == NULL);
			
      // �⥸�塼��̾��NULL����ꤷ�ƥ���ݡ��ͥ���������ߤơ��տޤɤ���NULL����뤫��
      RTC::RtcBase* comp2 = m_mgr->createComponent(NULL);
      CPPUNIT_ASSERT(comp2 == NULL);
    }
		
    void test_createComponent_failed_in_bindExecutionContext()
    {
      // �������Ԥ�
      int argc = 1;
      char* argv[] = { "-f fixture4.conf" };
			
      m_mgr = RTC::Manager::init(argc, argv);
      CPPUNIT_ASSERT(m_mgr != NULL);
      CPPUNIT_ASSERT(! CORBA::is_nil(m_mgr->getORB()));
      CPPUNIT_ASSERT(! CORBA::is_nil(m_mgr->getPOA()));
      CPPUNIT_ASSERT(! CORBA::is_nil(m_mgr->getPOAManager()));

      // ��֥��å��󥰥⡼�ɤǥޥ͡�������ư������
      CPPUNIT_ASSERT(m_mgr->activateManager());
      m_mgr->runManager(true); // true:��֥��å��󥰡�false:�֥��å���
			
      // Factory����Ͽ���Ƥ���
      RTC::Properties properties;
      properties.setProperty("implementation_id", "DataFlowComponentFactory");
      properties.setProperty("type_name", "DataFlowComponent");
      CPPUNIT_ASSERT(m_mgr->registerFactory(
					    properties, CreateDataFlowComponentMock, DeleteDataFlowComponentMock));
			
      // bindExecutionContext()�Ǽ��Ԥ���褦�ˡ��տ�Ū��ECFactory����Ͽ�����ˤ���
			
      // ����ݡ��ͥ���������ߤơ��տޤɤ���NULL����뤫��
      RTC::RtcBase* comp = m_mgr->createComponent("DataFlowComponentFactory");
      CPPUNIT_ASSERT(comp == NULL);
    }
		
    /*!
     * @brief cleanupComponent()�᥽�åɤΥƥ���
     * 
     * - ��Ͽ��������ݡ��ͥ�Ȥ����͡��ॵ���ӥ�������������Ͽ�������뤫��
     * - ��Ͽ��������ݡ��ͥ�Ȥ���Manager������������Ͽ�������뤫��
     */
    void test_cleanupComponent()
    {
      // �������Ԥ�
      int argc = 1;
      char* argv[] = { "-f fixture4.conf" };
			
      m_mgr = RTC::Manager::init(argc, argv);
      CPPUNIT_ASSERT(m_mgr != NULL);
      CPPUNIT_ASSERT(! CORBA::is_nil(m_mgr->getORB()));
      CPPUNIT_ASSERT(! CORBA::is_nil(m_mgr->getPOA()));
      CPPUNIT_ASSERT(! CORBA::is_nil(m_mgr->getPOAManager()));

      // ��֥��å��󥰥⡼�ɤǥޥ͡�������ư������
      CPPUNIT_ASSERT(m_mgr->activateManager());
      m_mgr->runManager(true); // true:��֥��å��󥰡�false:�֥��å���
			
      // Factory����Ͽ���Ƥ���
      RTC::Properties properties;
      properties.setProperty("implementation_id", "DataFlowComponentFactory");
      properties.setProperty("type_name", "DataFlowComponent");
      CPPUNIT_ASSERT(m_mgr->registerFactory(
					    properties, CreateDataFlowComponentMock, DeleteDataFlowComponentMock));
			
      // ECFactory����Ͽ���Ƥ���
      CPPUNIT_ASSERT(m_mgr->registerECFactory(
					      "PeriodicEC",
					      RTC::ECCreate<RTC::PeriodicExecutionContext>,
					      RTC::ECDelete<RTC::PeriodicExecutionContext>));

      // ��ǧ�Ѥ˥͡��ॵ���ӥ��ؤΥ����������ʤȤ���NamingManager��������Ƥ���
      // ��fixture4.conf�γ�����˹�碌�Ƥ�����������
      RTC::NamingManager nmgr(m_mgr);
      const char* name_server = "localhost:9876";
      nmgr.registerNameServer("corba", name_server);
			
      // ����������ݡ��ͥ�Ȥ������Ǥ��뤫��
      RTC::RtcBase* comp = m_mgr->createComponent("DataFlowComponentFactory");
      CPPUNIT_ASSERT(comp != NULL);
      CPPUNIT_ASSERT(dynamic_cast<DataFlowComponentMock*>(comp) != NULL);
      CPPUNIT_ASSERT(! CORBA::is_nil(comp->_this()));
      CPPUNIT_ASSERT_EQUAL(
			   std::string("DataFlowComponent0"), // ��������0��NumberingPolicy�ˤ���ղä����
			   std::string(comp->getInstanceName()));
			
      // ����ݡ��ͥ�Ȥˡ��տޤɤ���ExecutionContext�������å�����Ƥ��뤫��
      RTC::ExecutionContextList* ecList = comp->get_contexts();
      CPPUNIT_ASSERT(ecList != NULL);
      CPPUNIT_ASSERT_EQUAL(1, (int) ecList->length());
			
      // cleanupComponent()�ˤ�ꡢ��������Ͽ�������뤫��
      // - ��Ͽ��������ݡ��ͥ�Ȥ����͡��ॵ���ӥ�������������Ͽ�������뤫��
      // - ��Ͽ��������ݡ��ͥ�Ȥ���Manager������������Ͽ�������뤫��
      CPPUNIT_ASSERT(canResolve(name_server, "DataFlowComponent0", "rtc"));
      CPPUNIT_ASSERT_EQUAL(comp, m_mgr->getComponent("DataFlowComponent0"));
      m_mgr->cleanupComponent(comp);
      CPPUNIT_ASSERT(! canResolve(name_server, "DataFlowComponent0", "rtc"));
      CPPUNIT_ASSERT(m_mgr->getComponent("DataFlowComponent0") == NULL);
    }
		
    void test_registerComponent()
    {
      // Manager::createComponent()��ǻ��Ѥ���Ƥ���Τǡ������Ǥϥƥ��Ⱦ�ά����
    }
		
    void test_unregisterComponent()
    {
      // Manager::cleanupComponent()��ǻ��Ѥ���Ƥ���Τǡ������Ǥϥƥ��Ⱦ�ά����
    }
		
    void test_bindExecutionContext()
    {
      // Manager::createComponent()��ǻ��Ѥ���Ƥ���Τǡ������Ǥϥƥ��Ⱦ�ά����
    }
		
    void test_deleteComponent()
    {
      // Manager¦��̤�����ˤĤ��ƥ��Ȥ�̤����
    }
		
    void test_getComponent()
    {
      // ¾�ƥ�����ǻ��Ѥ���Ƥ���Τǡ�������Ƿ�ͤ��ΤȤ��ơ������Ǥϥƥ��Ⱦ�ά����
    }
		
    /*!
     * @brief getComponents()�᥽�åɤΥƥ���
     * 
     * - getComponents()�ǡ������������٤ƤΥ���ݡ��ͥ�Ȥ�����Ǥ��뤫��
     * - ��Ͽ�����������ݡ��ͥ�Ȥ������������������������Ƥ��뤫��
     */
    void test_getComponents()
    {
      // �������Ԥ�
      int argc = 1;
      char* argv[] = { "-f fixture4.conf" };
			
      m_mgr = RTC::Manager::init(argc, argv);
      CPPUNIT_ASSERT(m_mgr != NULL);
      CPPUNIT_ASSERT(! CORBA::is_nil(m_mgr->getORB()));
      CPPUNIT_ASSERT(! CORBA::is_nil(m_mgr->getPOA()));
      CPPUNIT_ASSERT(! CORBA::is_nil(m_mgr->getPOAManager()));

      // ��֥��å��󥰥⡼�ɤǥޥ͡�������ư������
      CPPUNIT_ASSERT(m_mgr->activateManager());
      m_mgr->runManager(true); // true:��֥��å��󥰡�false:�֥��å���
			
      // Factory����Ͽ���Ƥ���
      RTC::Properties properties;
      properties.setProperty("implementation_id", "DataFlowComponentFactory");
      properties.setProperty("type_name", "DataFlowComponent");
      CPPUNIT_ASSERT(m_mgr->registerFactory(
					    properties, CreateDataFlowComponentMock, DeleteDataFlowComponentMock));
			
      // ECFactory����Ͽ���Ƥ���
      CPPUNIT_ASSERT(m_mgr->registerECFactory(
					      "PeriodicEC",
					      RTC::ECCreate<RTC::PeriodicExecutionContext>,
					      RTC::ECDelete<RTC::PeriodicExecutionContext>));
			
      // ʣ���Υ���ݡ��ͥ�Ȥ��������Ƥ���
      RTC::RtcBase* comp1 = m_mgr->createComponent("DataFlowComponentFactory");
      CPPUNIT_ASSERT(comp1 != NULL);
      CPPUNIT_ASSERT(dynamic_cast<DataFlowComponentMock*>(comp1) != NULL);
      CPPUNIT_ASSERT(! CORBA::is_nil(comp1->_this()));

      RTC::RtcBase* comp2 = m_mgr->createComponent("DataFlowComponentFactory");
      CPPUNIT_ASSERT(comp2 != NULL);
      CPPUNIT_ASSERT(dynamic_cast<DataFlowComponentMock*>(comp2) != NULL);
      CPPUNIT_ASSERT(! CORBA::is_nil(comp2->_this()));
			
      CPPUNIT_ASSERT(comp1 != comp2);
			
      // getComponents()�ǡ������������٤ƤΥ���ݡ��ͥ�Ȥ�����Ǥ��뤫��
      std::vector<RTC::RtcBase*> comps = m_mgr->getComponents();
      CPPUNIT_ASSERT_EQUAL(2, (int) comps.size());
      CPPUNIT_ASSERT(std::find(comps.begin(), comps.end(), comp1) != comps.end());
      CPPUNIT_ASSERT(std::find(comps.begin(), comps.end(), comp2) != comps.end());
			
      // ����ݡ��ͥ�Ȥ򣱤Ĥ�����Ͽ���������硢���������Τ����������������Ƥ��뤫��
      m_mgr->cleanupComponent(comp1);
      comps = m_mgr->getComponents();
      CPPUNIT_ASSERT(std::find(comps.begin(), comps.end(), comp1) == comps.end());
      CPPUNIT_ASSERT(std::find(comps.begin(), comps.end(), comp2) != comps.end());
    }
		
    void test_getORB()
    {
      // ¾�ƥ�����ǻ��Ѥ���Ƥ���Τǡ������Ǥϥƥ��Ⱦ�ά����
    }
		
    void test_getPOA()
    {
      // ¾�ƥ�����ǻ��Ѥ���Ƥ���Τǡ������Ǥϥƥ��Ⱦ�ά����
    }
		
    void test_getPOAManager()
    {
      // ¾�ƥ�����ǻ��Ѥ���Ƥ���Τǡ������Ǥϥƥ��Ⱦ�ά����
    }
		
  };
}; // namespace Tests

/*
 * Register test suite
 */
CPPUNIT_TEST_SUITE_REGISTRATION(Tests::ManagerTests);

#ifdef LOCAL_MAIN
int main(int argc, char* argv[])
{

  FORMAT format = TEXT_OUT;
  int target = 0;
  std::string xsl;
  std::string ns;
  std::string fname;
  std::ofstream ofs;

  int i(1);
  while (i < argc)
    {
      std::string arg(argv[i]);
      std::string next_arg;
      if (i + 1 < argc) next_arg = argv[i + 1];
      else              next_arg = "";

      if (arg == "--text") { format = TEXT_OUT; break; }
      if (arg == "--xml")
	{
	  if (next_arg == "")
	    {
	      fname = argv[0];
	      fname += ".xml";
	    }
	  else
	    {
	      fname = next_arg;
	    }
	  format = XML_OUT;
	  ofs.open(fname.c_str());
	}
      if ( arg == "--compiler"  ) { format = COMPILER_OUT; break; }
      if ( arg == "--cerr"      ) { target = 1; break; }
      if ( arg == "--xsl"       )
	{
	  if (next_arg == "") xsl = "default.xsl"; 
	  else                xsl = next_arg;
	}
      if ( arg == "--namespace" )
	{
	  if (next_arg == "")
	    {
	      std::cerr << "no namespace specified" << std::endl;
	      exit(1); 
	    }
	  else
	    {
	      xsl = next_arg;
	    }
	}
      ++i;
    }
  CppUnit::TextUi::TestRunner runner;
  if ( ns.empty() )
    runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
  else
    runner.addTest(CppUnit::TestFactoryRegistry::getRegistry(ns).makeTest());
  CppUnit::Outputter* outputter = 0;
  std::ostream* stream = target ? &std::cerr : &std::cout;
  switch ( format )
    {
    case TEXT_OUT :
      outputter = new CppUnit::TextOutputter(&runner.result(),*stream);
      break;
    case XML_OUT :
      std::cout << "XML_OUT" << std::endl;
      outputter = new CppUnit::XmlOutputter(&runner.result(),
					    ofs, "shift_jis");
      static_cast<CppUnit::XmlOutputter*>(outputter)->setStyleSheet(xsl);
      break;
    case COMPILER_OUT :
      outputter = new CppUnit::CompilerOutputter(&runner.result(),*stream);
      break;
    }
  runner.setOutputter(outputter);
  runner.run();
  return 0; // runner.run() ? 0 : 1;
}
#endif // MAIN
#endif // ManagerTests_cpp