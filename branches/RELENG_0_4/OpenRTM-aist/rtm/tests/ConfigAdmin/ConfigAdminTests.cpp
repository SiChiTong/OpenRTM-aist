// -*- C++ -*-
/*!
 * @file   ConfigAdminTests.cpp
 * @brief  ConfigAdmin test class
 * @date   $Date: 2008/05/12 01:47:32 $
 *
 * $Id: ConfigAdminTests.cpp,v 1.1 2008/05/12 01:47:32 arafune Exp $
 *
 */

/*
 * $Log: ConfigAdminTests.cpp,v $
 * Revision 1.1  2008/05/12 01:47:32  arafune
 * The first commitment.
 *
 *
 */

#ifndef ConfigAdmin_cpp
#define ConfigAdmin_cpp

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestAssert.h>
#include <strstream>
#include <rtm/ConfigAdmin.h>

/*!
 * @class ConfigTests class
 * @brief Config test
 */
namespace Config
{
  class ConfigTests
    : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(ConfigTests);
    CPPUNIT_TEST(test_constructor);
    CPPUNIT_TEST(test_update);
    CPPUNIT_TEST(test_update_with_illegal_value);
    CPPUNIT_TEST(test_update_with_illegal_default_value);
    CPPUNIT_TEST_SUITE_END();
		
  private:
	
	
  public:
    /*!
     * @brief Constructor
     */
    ConfigTests()
    {
    }
		
    /*!
     * @brief Destructor
     */
    ~ConfigTests()
    {
    }
		
    /*!
     * @brief Test initialization
     */
    virtual void setUp()
    {
    }
		
    /*!
     * @brief Test finalization
     */
    virtual void tearDown()
    { 
    }
		
    /*!
     * @brief ���󥹥ȥ饯���Υƥ���
     */
    void test_constructor()
    {
      std::string name = "name of parameter";
      double value = 0.0;
      std::string default_value = "3.14159";
			
      RTC::Config<double> config(name.c_str(), value, default_value.c_str());
			
      // ���ꤷ��̾�Τ����������ꤵ��Ƥ��뤫��
      CPPUNIT_ASSERT_EQUAL(name, std::string(config.name));
			
      // ���ꤷ���ǥե�����͡ʤ�ʸ����ɽ���ˤ����������ꤵ��Ƥ��뤫��
      CPPUNIT_ASSERT_EQUAL(default_value, std::string(config.default_value));
    }
		
    /*!
     * @brief update()�᥽�åɤΥƥ���
     * 
     * - �Х���ɤ����ѿ�����update()�ǻ��ꤵ�줿�ͤ���������������뤫��
     */
    void test_update()
    {
      std::string name = "name of parameter";
      double value = 0.0;
      std::string default_value = "3.14159";
			
      RTC::Config<double> config(name.c_str(), value, default_value.c_str());
			
      // update()���ϡ��ѿ��ͤ�������֤ΤޤޤΤϤ�
      CPPUNIT_ASSERT_EQUAL(0.0, value);
			
      // update()����
      CPPUNIT_ASSERT_EQUAL(true, config.update("1.41421356"));
			
      // �Х���ɤ����ѿ�����update()�ǻ��ꤵ�줿�ͤ���������������뤫��
      CPPUNIT_ASSERT_EQUAL(1.41421356, value);
    }
		
    /*!
     * @brief update()�᥽�åɤΥƥ���
     * 
     * - ����η����Ѵ��Ǥ��ʤ����Ƥ�update()��ƽФ����տޤɤ��꼺�Ԥ��뤫��
     * - �Х���ɤ����ѿ����ͤ������󥹥ȥ饯���ǻ��ꤷ���ǥե�����ͤ���������������뤫��
     */
    void test_update_with_illegal_value()
    {
      std::string name = "name of parameter";
      double value = 0.0;
      std::string default_value = "3.14159";
			
      RTC::Config<double> config(name.c_str(), value, default_value.c_str());
			
      // update()���ϡ��ѿ��ͤ�������֤ΤޤޤΤϤ�
      CPPUNIT_ASSERT_EQUAL(0.0, value);
			
      // ��ư�������ͤ��Ѵ��Ǥ��ʤ����Ƥ�update()��ƽФ����տޤɤ��꼺�Ԥ��뤫��
      CPPUNIT_ASSERT_EQUAL(false, config.update("Not float value"));
			
      // �Х���ɤ����ѿ����ͤ������󥹥ȥ饯���ǻ��ꤷ���ǥե�����ͤ���������������뤫��
      CPPUNIT_ASSERT_EQUAL(3.14159, value);
    }
		
    /*!
     * @brief update()�᥽�åɤΥƥ���
     * 
     * - ��ư�������ͤ��Ѵ��Ǥ��ʤ��ǥե�����ͤ����ꤵ�졢������ư�������ͤ��Ѵ��Ǥ��ʤ����Ƥ�update()��ƽФ�����硢
     * �Х���ɤ����ѿ����ͤϡ���������뤳�Ȥʤ������ͤ�α�ޤ뤫��
     */
    void test_update_with_illegal_default_value()
    {
      std::string name = "name of parameter";
      double value = 0.0;
      std::string illegal_default_value = "Not float value";
			
      RTC::Config<double> config(name.c_str(), value, illegal_default_value.c_str());

      // update()���ϡ��ѿ��ͤ�������֤ΤޤޤΤϤ�
      CPPUNIT_ASSERT_EQUAL(0.0, value);
			
      // ��ư�������ͤ��Ѵ��Ǥ��ʤ��ǥե�����ͤ����ꤵ�졢
      // ������ư�������ͤ��Ѵ��Ǥ��ʤ����Ƥ�update()��ƽФ�����硢
      // �Х���ɤ����ѿ����ͤϡ���������뤳�Ȥʤ������ͤ�α�ޤ뤫��
      CPPUNIT_ASSERT_EQUAL(false, config.update("Not float value too"));
      CPPUNIT_ASSERT_EQUAL(0.0, value);
    }
		
  };
}; // namespace ConfigAdmin

/*!
 * @class ConfigAdminTests class
 * @brief ConfigAdmin test
 */
namespace ConfigAdmin
{
  class ConfigAdminTests
    : public CppUnit::TestFixture
  {
    CPPUNIT_TEST_SUITE(ConfigAdminTests);
    CPPUNIT_TEST(test_constructor);
    CPPUNIT_TEST(test_bindParameter);
    CPPUNIT_TEST(test_bindParameter_already_exist);
    CPPUNIT_TEST(test_bindParameter_illegal_default_value);
    CPPUNIT_TEST(test_update);
    CPPUNIT_TEST(test_update_by_inexist_configuration_set);
    CPPUNIT_TEST(test_update_with_specified_parameter_name);
    CPPUNIT_TEST(test_update_by_active_configuration_set);
    CPPUNIT_TEST(test_isExist);
    CPPUNIT_TEST(test_isChanged_on_addConfigurationSet);
    CPPUNIT_TEST(test_isChanged_on_removeConfigurationSet);
    CPPUNIT_TEST(test_isChanged_on_activateConfigurationSet);
    CPPUNIT_TEST(test_getActiveId);
    CPPUNIT_TEST(test_haveConfig);
    CPPUNIT_TEST(test_isActive_on_addConfigurationSet);
    CPPUNIT_TEST(test_isActive_on_removeConfigurationSet);
    CPPUNIT_TEST(test_getConfigurationSets);
    CPPUNIT_TEST(test_addConfigurationSet_and_getConfigurationSet);
    CPPUNIT_TEST(test_setConfigurationSetValues);
    CPPUNIT_TEST(test_setConfigurationSetValues_with_inexist_configuration_set);
    CPPUNIT_TEST(test_getActiveConfigurationSet);
    CPPUNIT_TEST(test_removeConfigurationSet);
    CPPUNIT_TEST(test_removeConfigurationSet_with_inexist_configuration_id);
    CPPUNIT_TEST_SUITE_END();
		
  private:
    // helper
    const RTC::Properties* getPropertiesBy(
					   const std::string& name,
					   const std::vector<RTC::Properties*>& propertiesSet) const
    {
      for (int i = 0; i < (int) propertiesSet.size(); ++i)
	{
	  if (std::string(propertiesSet[i]->getName()) == name)
	    {
	      return propertiesSet[i];
	    }
	}
			
      return 0; // not found
    }
	
  public:
    /*!
     * @brief Constructor
     */
    ConfigAdminTests()
    {
    }
		
    /*!
     * @brief Destructor
     */
    ~ConfigAdminTests()
    {
    }
		
    /*!
     * @brief Test initialization
     */
    virtual void setUp()
    {
    }
		
    /*!
     * @brief Test finalization
     */
    virtual void tearDown()
    { 
    }
		
    /*!
     * @brief ���󥹥ȥ饯���Υƥ���
     * 
     * - �����ǻ��ꤷ������ե����졼����󥻥åȤ������������ꤵ��뤫��
     */
    void test_constructor()
    {
      RTC::Properties configSet("config_id");
      configSet.setProperty("config_id.key", "value");
			
      RTC::ConfigAdmin configAdmin(configSet);
			
      // �����ǻ��ꤷ������ե����졼����󥻥åȤ������������ꤵ��뤫��
      const RTC::Properties& activeConfigSet = configAdmin.getConfigurationSet("config_id");
      CPPUNIT_ASSERT_EQUAL(std::string("value"), activeConfigSet.getProperty("key"));
    }
		
    /*!
     * @brief bindParameter()�᥽�åɤΥƥ���
     * 
     * - bindParameter()�ǡ�������ѿ���Х���ɤǤ��뤫��
     * - �Х���ɤ����ѿ��ϡ����ꤷ���ǥե�����ͤ���������������Ƥ��뤫��
     */		
    void test_bindParameter()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);
			
      // bindParameter()�ǡ�������ѿ���Х���ɤǤ��뤫��
      const char* varName = "name";
      double var = 0.0;
      const char* default_value = "3.14159";
      CPPUNIT_ASSERT_EQUAL(
			   true, configAdmin.bindParameter(varName, var, default_value));
			
      // �Х���ɤ����ѿ��ϡ����ꤷ���ǥե�����ͤ���������������Ƥ��뤫��
      CPPUNIT_ASSERT_EQUAL(3.14159, var);
    }
		
    /*!
     * @brief bindParameter()�᥽�åɤΥƥ���
     * 
     * - Ʊ��̾�Τ��ѿ��Х���ɤ��ߤơ��տޤɤ��꼺�Ԥ��뤫��
     */
    void test_bindParameter_already_exist()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);
			
      // �ޤ��ϡ����ΥХ���ɤ�Ԥ�
      const char* varName1 = "name";
      double var1 = 0.0;
      const char* default_value1 = "3.14159";
      CPPUNIT_ASSERT_EQUAL(
			   true, configAdmin.bindParameter(varName1, var1, default_value1));
      CPPUNIT_ASSERT_EQUAL(3.14159, var1);
			
      // Ʊ��̾�Τ��ѿ��Х���ɤ��ߤơ��տޤɤ��꼺�Ԥ��뤫��
      const char* varName2 = varName1;
      double var2 = 1.0;
      const char* default_value2 = "1.41421356";
      CPPUNIT_ASSERT_EQUAL(
			   false, configAdmin.bindParameter(varName2, var2, default_value2));
				
      // �Х�����ѿ����ͤϹ�������뤳�Ȥʤ��ݻ�����Ƥ��뤫��
      CPPUNIT_ASSERT_EQUAL(3.14159, var1);
    }
		
    /*!
     * @brief bindParameter()�᥽�åɤΥƥ���
     * 
     * - ����Υǡ��������Ѵ��Ǥ��ʤ��ǥե�����ͤ���ꤷ�ơ��տޤɤ��꼺�Ԥ��뤫��
     */
    void test_bindParameter_illegal_default_value()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);
			
      // ����Υǡ��������Ѵ��Ǥ��ʤ��ǥե�����ͤ���ꤷ�ơ��տޤɤ��꼺�Ԥ��뤫��
      const char* varName = "name";
      double var = 0.0;
      const char* default_value = "Illegal default value";
      CPPUNIT_ASSERT_EQUAL(
			   false, configAdmin.bindParameter(varName, var, default_value));
    }
		
    /*!
     * @brief update()�᥽�åɤΥƥ���
     * 
     * - ����ե����졼����󥻥åȤ���ꤷ��update()�����������ƤǥХ�����ѿ��ͤ���������������뤫��
     */
    void test_update()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);
			
      // ����ե����졼����󥻥åȣ����������
      RTC::Properties configSet1("set 1");
      configSet1.setProperty("name", "1.41421356");
			
      // ����ե����졼����󥻥åȣ����������
      RTC::Properties configSet2("set 2");
      configSet2.setProperty("name", "1.7320508");
			
      // �����������ĤΥ���ե����졼����󥻥åȤ��ɲä��Ƥ���
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet1));
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet2));
			
      // �ѿ���Х���ɤ���
      const char* varName = "name";
      double var = 0.0;
      const char* default_value = "3.14159";
      CPPUNIT_ASSERT_EQUAL(
			   true, configAdmin.bindParameter(varName, var, default_value));
			
      // update()���ϡ��ޤ��ѿ����ǥե�����ͤΤޤޤǤ��뤳�Ȥ��ǧ����
      CPPUNIT_ASSERT_EQUAL(3.14159, var);
			
      // ����ե����졼����󥻥åȣ�����ꤷ��update()�����������ƤǥХ�����ѿ��ͤ���������������뤫��
      configAdmin.update("set 1");
      CPPUNIT_ASSERT_EQUAL(1.41421356, var);

      // ����ե����졼����󥻥åȣ�����ꤷ��update()�����������ƤǥХ�����ѿ��ͤ���������������뤫��
      configAdmin.update("set 2");
      CPPUNIT_ASSERT_EQUAL(1.7320508, var);
    }
		
    /*!
     * @brief update()�᥽�åɤΥƥ���
     * 
     * - ¸�ߤ��ʤ�����ե����졼�����ID����ꤷ��update()��ƽФ������ˡ�
     * �Х�����ѿ����������줺�ˡ����Τޤ��ݻ�����뤫��
     */
    void test_update_by_inexist_configuration_set()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);
			
      // ����ե����졼����󥻥åȤ���������ɲä��Ƥ���
      RTC::Properties configSet("set");
      configSet.setProperty("name", "1.41421356");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet));
			
      // �ѿ���Х���ɤ���
      const char* varName = "name";
      double var = 0.0;
      const char* default_value = "3.14159";
      CPPUNIT_ASSERT_EQUAL(
			   true, configAdmin.bindParameter(varName, var, default_value));
      CPPUNIT_ASSERT_EQUAL(3.14159, var);
			
      // ¸�ߤ��ʤ�����ե����졼�����ID����ꤷ��update()��ƽФ������ˡ�
      // �Х�����ѿ����������줺�ˡ����Τޤ��ݻ�����뤫��
      configAdmin.update("inexist set");
      CPPUNIT_ASSERT_EQUAL(3.14159, var);
    }
		
    /*!
     * @brief update()�᥽�åɡ�̾�λ���ˤΥƥ���
     * 
     * - ���ꤷ������ե����졼����󥻥åȤλ��ꤷ��̾�Τ��ѿ�����������������������뤫��
     */
    void test_update_with_specified_parameter_name()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);
			
      // ���ĤΥѥ�᡼���ͤ�ޤॳ��ե����졼����󥻥åȤ���������ɲä��Ƥ���
      RTC::Properties configSet1("set 1");
      configSet1.setProperty("name 1", "1.41421356");
      configSet1.setProperty("name 2", "1.7320508");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet1));

      RTC::Properties configSet2("set 2");
      configSet2.setProperty("name 1", "3.14159");
      configSet2.setProperty("name 2", "2.71828");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet2));
			
      // ���Ĥ��ѿ���Х���ɤ���
      const char* varName1 = "name 1";
      double var1 = 0.0;
      const char* default_value1 = "2.23620679";
      CPPUNIT_ASSERT_EQUAL(
			   true, configAdmin.bindParameter(varName1, var1, default_value1));
      CPPUNIT_ASSERT_EQUAL(2.23620679, var1);
			
      const char* varName2 = "name 2";
      double var2 = 0.0;
      const char* default_value2 = "2.4494897";
      CPPUNIT_ASSERT_EQUAL(
			   true, configAdmin.bindParameter(varName2, var2, default_value2));
      CPPUNIT_ASSERT_EQUAL(2.4494897, var2);
			
      // ���ĤΤ������������ѿ��Τߤ�̾�λ��ꤷ��update()��Ԥ������ꤷ���ѿ���������������������뤫��
      configAdmin.update("set 2", "name 1");
      CPPUNIT_ASSERT_EQUAL(3.14159, var1);
      CPPUNIT_ASSERT_EQUAL(2.4494897, var2);
    }
		
    /*!
     * @brief update()�᥽�åɡʥ����ƥ��֥���ե����졼����󥻥åȡˤΥƥ���
     * 
     * - update()�ƽФ��ˤ�ꡢ�Х�����ѿ��������ƥ��֤ʥ���ե����졼����󥻥åȤ��ͤǹ�������뤫��
     */
    void test_update_by_active_configuration_set()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);
			
      // ����ե����졼����󥻥åȤ���������ɲä��Ƥ���
      RTC::Properties configSet1("set 1");
      configSet1.setProperty("name", "1.41421356");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet1));

      RTC::Properties configSet2("set 2");
      configSet2.setProperty("name", "1.7320508");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet2));
			
      // �ѿ���Х���ɤ���
      const char* varName = "name";
      double var = 0.0;
      const char* default_value = "3.14159";
      CPPUNIT_ASSERT_EQUAL(
			   true, configAdmin.bindParameter(varName, var, default_value));
      CPPUNIT_ASSERT_EQUAL(3.14159, var);
			
      // "set 1"�Τۤ��򥢥��ƥ��֤ˤ���
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.activateConfigurationSet("set 1"));
			
      // �����ƥ��֤ˤ��������Ǥϡ��ޤ��Х�����ѿ��Ϲ�������Ƥ��ʤ��Ϥ�
      CPPUNIT_ASSERT_EQUAL(3.14159, var);
			
      // update()�ƽФ��ˤ�ꡢ�Х�����ѿ��������ƥ��֤ʥ���ե����졼����󥻥åȤ��ͤǹ�������뤫��
      configAdmin.update();
      CPPUNIT_ASSERT_EQUAL(1.41421356, var);

      // "set 2"�Τۤ��򥢥��ƥ��֤ˤ���
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.activateConfigurationSet("set 2"));
			
      // �����ƥ��֤ˤ��������Ǥϡ��ޤ��Х�����ѿ��Ϲ�������Ƥ��ʤ��Ϥ�
      CPPUNIT_ASSERT_EQUAL(1.41421356, var);

      // update()�ƽФ��ˤ�ꡢ�Х�����ѿ��������ƥ��֤ʥ���ե����졼����󥻥åȤ��ͤǹ�������뤫��
      configAdmin.update();
      CPPUNIT_ASSERT_EQUAL(1.7320508, var);
    }
		
    /*!
     * @brief isExist()�᥽�åɤΥƥ���
     * 
     * - �Х���ɤ����ѿ���̾�Τ�isExist()��ƽФ������ͤ������뤫��
     * - �Х���ɤ��Ƥ��ʤ�̾�Τ�isExist()��ƽФ������ͤ������뤫��
     */
    void test_isExist()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);
			
      // �ѿ���Х���ɤ���
      const char* varName = "name";
      double var = 0.0;
      const char* default_value = "3.14159";
      CPPUNIT_ASSERT_EQUAL(
			   true, configAdmin.bindParameter(varName, var, default_value));
      CPPUNIT_ASSERT_EQUAL(3.14159, var);
			
      // �Х���ɤ����ѿ���̾�Τ�isExist()��ƽФ������ͤ������뤫��
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.isExist("name"));
			
      // �Х���ɤ��Ƥ��ʤ�̾�Τ�isExist()��ƽФ������ͤ������뤫��
      CPPUNIT_ASSERT_EQUAL(false, configAdmin.isExist("inexist name"));
    }
		
    /*!
     * @brief isChanged()�᥽�åɤΥƥ���
     * 
     * - addConfigurationSet()�ƽи�ϡ�isChanged()�Ͽ��ͤȤʤ뤫��
     */
    void test_isChanged_on_addConfigurationSet()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);
			
      // update()��ƽФ��ƥХ�����ѿ���Ʊ����Ԥ���isChanged()�����Ȥʤ���֤ˤ���
      configAdmin.update();
      CPPUNIT_ASSERT_EQUAL(false, configAdmin.isChanged());
			
      // addConfigurationSet()��ƽФ�
      RTC::Properties configSet("id");
      configSet.setProperty("key", "value");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet));

      // addConfigurationSet()�ƽи�ϡ�isChanged()�Ͽ��ͤȤʤ뤫��
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.isChanged());
    }
		
    /*!
     * @brief isChanged()�᥽�åɤΥƥ���
     * 
     * - removeConfigurationSet()�ƽи�ϡ�isChanged()�Ͽ��ͤȤʤ뤫��
     */
    void test_isChanged_on_removeConfigurationSet()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);
			
      // addConfigurationSet()��ƽФ�
      RTC::Properties configSet("id");
      configSet.setProperty("key", "value");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet));

      // update()��ƽФ��ƥХ�����ѿ���Ʊ����Ԥ���isChanged()�����Ȥʤ���֤ˤ���
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.activateConfigurationSet("id"));
      configAdmin.update();
      CPPUNIT_ASSERT_EQUAL(false, configAdmin.isChanged());
			
      // removeConfigurationSet()�θƽи�ϡ�isChanged()�Ͽ��ͤȤʤ뤫��
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.removeConfigurationSet("id"));
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.isChanged());
    }
		
    /*!
     * @brief isChanged()�᥽�åɤΥƥ���
     * 
     * - activateConfigurationSet()�ƽи�ϡ�isChanged()�Ͽ��ͤȤʤ뤫��
     */
    void test_isChanged_on_activateConfigurationSet()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);
			
      // ����ե����졼����󥻥åȤ���������ɲä��Ƥ���
      RTC::Properties configSet1("set 1");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet1));

      RTC::Properties configSet2("set 2");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet2));
			
      // "set 1"�Τۤ��򥢥��ƥ��֤ˤ���
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.activateConfigurationSet("set 1"));
			
      // update()��Ԥ���isChanged()�����ͤξ��֤ˤ��Ƥ���
      configAdmin.update();
      CPPUNIT_ASSERT_EQUAL(false, configAdmin.isChanged());

      // "set 2"�Τۤ��򥢥��ƥ��֤ˤ���ȡ�isChanged()�����ͤȤʤ뤫��
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.activateConfigurationSet("set 2"));
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.isChanged());
    }
		
    void test_isChanged_on_setConfigurationSetValues()
    {
      // TODO �������뤳��
    }
		
    /*!
     * @brief getActiveId()�᥽�åɤΥƥ���
     * 
     * - �����ƥ��ֲ���������ե����졼����󥻥åȤ�ID�������������Ǥ��뤫��
     */
    void test_getActiveId()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);
			
      // ����ե����졼����󥻥åȤ���������ɲä��Ƥ���
      RTC::Properties configSet1("set 1");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet1));

      RTC::Properties configSet2("set 2");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet2));
			
      // ������֤Ǥϡ������ƥ���ID��"default"�ΤϤ�
      CPPUNIT_ASSERT_EQUAL(std::string("default"), std::string(configAdmin.getActiveId()));
			
      // "set 1"�򥢥��ƥ��֤ˤ����塢�տޤɤ���Υ����ƥ���ID������Ǥ��뤫��
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.activateConfigurationSet("set 1"));
      CPPUNIT_ASSERT_EQUAL(std::string("set 1"), std::string(configAdmin.getActiveId()));
			
      // "set 2"�򥢥��ƥ��֤ˤ����塢�տޤɤ���Υ����ƥ���ID������Ǥ��뤫��
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.activateConfigurationSet("set 2"));
      CPPUNIT_ASSERT_EQUAL(std::string("set 2"), std::string(configAdmin.getActiveId()));
    }
		
    /*!
     * @brief haveConfig()�᥽�åɤΥƥ���
     * 
     * - ¸�ߤ��륳��ե����졼����󥻥å�ID����ꤷ�����ˡ�haveConfig()�����������ͤ����뤫��
     * - ¸�ߤ��ʤ�����ե����졼����󥻥å�ID����ꤷ�����ˡ�haveConfig()�����������ͤ����뤫��
     */
    void test_haveConfig()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);
			
      // ����ե����졼����󥻥åȤ���������ɲä��Ƥ���
      RTC::Properties configSet1("id");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet1));
			
      // ¸�ߤ��륳��ե����졼����󥻥å�ID����ꤷ�����ˡ�haveConfig()�����������ͤ����뤫��
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.haveConfig("id"));
			
      // ¸�ߤ��ʤ�����ե����졼����󥻥å�ID����ꤷ�����ˡ�haveConfig()�����������ͤ����뤫��
      CPPUNIT_ASSERT_EQUAL(false, configAdmin.haveConfig("inexist id"));
    }
		
    /*!
     * @brief isActive()�᥽�åɤΥƥ���
     * 
     * - addConfigurationSet()�ƽи�ϡ�isActive()�����������ͤ����뤫��
     * - activateConfigurationSet()�ǥ����ƥ��ֲ�������ϡ�isActive()�����������ͤ����뤫��
     */
    void test_isActive_on_addConfigurationSet()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);
			
      // addConfigurationSet()�ƽи�ϡ�isActive()�����������ͤ����뤫��
      RTC::Properties configSet("id");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet));
      CPPUNIT_ASSERT_EQUAL(false, configAdmin.isActive());
			
      // activateConfigurationSet()�ǥ����ƥ��ֲ�������ϡ�isActive()�����������ͤ����뤫��
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.activateConfigurationSet("id"));
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.isActive());
    }
		
    /*!
     * @brief isActive()�᥽�åɤΥƥ���
     * 
     * - removeConfigurationSet()�ƽи�ϡ�isActive()�����������ͤ����뤫��
     */
    void test_isActive_on_removeConfigurationSet()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);

      // ����ե����졼����󥻥åȤ��ɲä��ƥ����ƥ��ֲ����Ƥ���
      RTC::Properties configSet("id");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet));
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.activateConfigurationSet("id"));
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.isActive());
			
      // ��Ͽ����Ƥ��륳��ե����졼����󥻥åȤ�removeConfigurationSet()�ǲ��������硢
      // isActive()�����������ͤ����뤫��
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.removeConfigurationSet("id"));
      CPPUNIT_ASSERT_EQUAL(false, configAdmin.isActive());
    }
		
    /*!
     * @brief isActive()�᥽�åɤΥƥ���
     * 
     * - activateConfigurationSet()�ǥ����ƥ��ֲ�������ϡ�isActive()�����������ͤ����뤫��
     */
    void test_isActive_on_activateConfigurationSet()
    {
      // test_isActive_on_addConfigurationSet()�Ƿ���
    }
		
    void test_isActive_on_setConfigurationSetValues()
    {
      // TODO �������뤳��
    }
		
    /*!
     * @brief getConfigurationSets()�᥽�åɤΥƥ���
     * 
     * - ��Ͽ����Ƥ������ƤΥ���ե����졼����󥻥åȤ������������Ǥ��뤫��
     */
    void test_getConfigurationSets()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);
			
      // ���ĤΥѥ�᡼���ͤ�ޤॳ��ե����졼����󥻥åȤ���������ɲä��Ƥ���
      RTC::Properties configSet1("set 1");
      configSet1.setProperty("name 1", "1.41421356");
      configSet1.setProperty("name 2", "1.7320508");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet1));
			
      RTC::Properties configSet2("set 2");
      configSet2.setProperty("name 1", "3.14159");
      configSet2.setProperty("name 2", "2.71828");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet2));
			
      // getConfigurationSets()����Ͽ����Ƥ������ƤΥ���ե����졼����󥻥åȤ��������
      // ��Ͽ������ΤȰ��פ��Ƥ��뤳�Ȥ��ǧ����
      std::vector<RTC::Properties*> expectedConfigSets;
      expectedConfigSets.push_back(&configSet1);
      expectedConfigSets.push_back(&configSet2);
			
      const std::vector<RTC::Properties*>& configSets
	= configAdmin.getConfigurationSets();
			
      CPPUNIT_ASSERT_EQUAL(
			   std::string("1.41421356"),
			   getPropertiesBy("set 1", configSets)->getProperty("name 1"));
      CPPUNIT_ASSERT_EQUAL(
			   std::string("1.7320508"),
			   getPropertiesBy("set 1", configSets)->getProperty("name 2"));

      CPPUNIT_ASSERT_EQUAL(
			   std::string("3.14159"),
			   getPropertiesBy("set 2", configSets)->getProperty("name 1"));
      CPPUNIT_ASSERT_EQUAL(
			   std::string("2.71828"),
			   getPropertiesBy("set 2", configSets)->getProperty("name 2"));
    }
		
    /*!
     * addConfigurationSet()�᥽�åɤ�getConfigurationSet()�᥽�åɤΥƥ���
     * 
     * - addConfigurationSet()�ǡ�����ե����졼����󥻥åȤ��ɲäǤ��뤫��
     * - getConfigurationSet()�ǡ��ɲä�������ե����졼����󥻥åȤ������������Ǥ��뤫��
     */
    void test_addConfigurationSet_and_getConfigurationSet()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);
			
      // addConfigurationSet()�ǡ�����ե����졼����󥻥åȤ��ɲäǤ��뤫��
      RTC::Properties configSet("id");
      configSet.setProperty("key", "value");
      configAdmin.addConfigurationSet(configSet);
			
      // getConfigurationSet()�ǡ��ɲä�������ե����졼����󥻥åȤ������������Ǥ��뤫��
      const RTC::Properties& configSetRet = configAdmin.getConfigurationSet("id");
      CPPUNIT_ASSERT_EQUAL(std::string("value"), configSetRet.getProperty("key"));
    }
		
    /*!
     * setConfigurationSetValues()�᥽�åɤΥƥ���
     * 
     * - ���ꤷ���ץ��ѥƥ��������������ꤷ������ե����졼����󥻥åȤ��ɲä���뤫��
     */
    void test_setConfigurationSetValues()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);
			
      RTC::Properties configSet1("id");
      configSet1.setProperty("name 1", "1.41421356");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet1));
			
      // ��Ͽ�ѤߤΥ���ե����졼����󥻥åȤ��Ф��ơ��ץ��ѥƥ����ɲä���
      RTC::Properties configSet2("id");
      configSet2.setProperty("name 2", "1.7320508");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.setConfigurationSetValues("id", configSet2));
			
      // ��������ե����졼����󥻥åȤ�������ơ��ץ��ѥƥ����տޤɤ����ɲä���Ƥ��뤳�Ȥ��ǧ����
      const RTC::Properties& configSetRet = configAdmin.getConfigurationSet("id");
      CPPUNIT_ASSERT_EQUAL(std::string("1.41421356"), configSetRet.getProperty("name 1"));
      CPPUNIT_ASSERT_EQUAL(std::string("1.7320508"), configSetRet.getProperty("name 2"));
    }
		
    /*!
     * @brief setConfigurationSetValues()�᥽�åɤΥƥ���
     * 
     * - ¸�ߤ��ʤ�����ե����졼����󥻥åȤ��Ф��ƥץ��ѥƥ��ɲä��ߤơ��տޤɤ��꼺�Ԥ��뤫��
     * - ���Ը�ˡ���Ͽ�ѤߤΥ���ե����졼����󥻥åȤ��ƶ�������Ƥ��ʤ�����
     */
    void test_setConfigurationSetValues_with_inexist_configuration_set()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);
			
      RTC::Properties configSet1("id");
      configSet1.setProperty("name 1", "1.41421356");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet1));
			
      // ¸�ߤ��ʤ�����ե����졼����󥻥åȤ��Ф��ƥץ��ѥƥ��ɲä��ߤơ��տޤɤ��꼺�Ԥ��뤫��
      RTC::Properties configSet2("inexist id");
      configSet2.setProperty("name 2", "1.7320508");
      CPPUNIT_ASSERT_EQUAL(false, configAdmin.setConfigurationSetValues("inexist id", configSet2));
			
      // ���Ը�ˡ���Ͽ�ѤߤΥ���ե����졼����󥻥åȤ��ƶ�������Ƥ��ʤ�����
      const RTC::Properties& configSetRet = configAdmin.getConfigurationSet("id");
      CPPUNIT_ASSERT_EQUAL(std::string("1.41421356"), configSetRet.getProperty("name 1"));
      CPPUNIT_ASSERT_EQUAL(std::string(""), configSetRet.getProperty("name 2"));
    }
		
    /*!
     * @brief getActiveConfigurationSet()�᥽�åɤΥƥ���
     * 
     * - �����ƥ��֥���ե����졼����󥻥åȤ������������Ǥ��뤫��
     */
    void test_getActiveConfigurationSet()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);
			
      // ����ե����졼����󥻥åȤ���������ɲä��Ƥ���
      RTC::Properties configSet1("set 1");
      configSet1.setProperty("name 1", "1.41421356");
      configSet1.setProperty("name 2", "1.7320508");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet1));
			
      RTC::Properties configSet2("set 2");
      configSet2.setProperty("name 1", "3.14159");
      configSet2.setProperty("name 2", "2.71828");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet2));
			
      // "set 1"�򥢥��ƥ��ֲ������塢�����ƥ��֥���ե����졼����󥻥åȤȤ��������������Ǥ��뤫��
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.activateConfigurationSet("set 1"));
      const RTC::Properties& activeConfigSet1 = configAdmin.getActiveConfigurationSet();
      CPPUNIT_ASSERT_EQUAL(std::string("1.41421356"), activeConfigSet1.getProperty("name 1"));
      CPPUNIT_ASSERT_EQUAL(std::string("1.7320508"), activeConfigSet1.getProperty("name 2"));

      // "set 2"�򥢥��ƥ��ֲ������塢�����ƥ��֥���ե����졼����󥻥åȤȤ��������������Ǥ��뤫��
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.activateConfigurationSet("set 2"));
      const RTC::Properties& activeConfigSet2 = configAdmin.getActiveConfigurationSet();
      CPPUNIT_ASSERT_EQUAL(std::string("3.14159"), activeConfigSet2.getProperty("name 1"));
      CPPUNIT_ASSERT_EQUAL(std::string("2.71828"), activeConfigSet2.getProperty("name 2"));
    }
		
    /*!
     * @brief removeConfigurationSet()�Υƥ���
     * 
     * - ��Ͽ����Ƥ��륳��ե����졼����󥻥åȤ���������Ͽ����Ǥ��뤫��
     */
    void test_removeConfigurationSet()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);
			
      // ����ե����졼����󥻥åȤ��ɲä��Ƥ���
      RTC::Properties configSet("id");
      configSet.setProperty("key", "value");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet));
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.haveConfig("id"));
			
      // ���ä�����Ͽ��������ե����졼����󥻥åȤ���Ͽ�������
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.removeConfigurationSet("id"));
			
      // ��������ե����졼����󥻥åȤ�¸�ߤ��ʤ����Ȥ��ǧ����
      CPPUNIT_ASSERT_EQUAL(false, configAdmin.haveConfig("id"));
    }
		
    /*!
     * @brief removeConfigurationSet()�Υƥ���
     * 
     * - ¸�ߤ��ʤ�����ե����졼����󥻥å�ID����ꤷ�����ˡ��տޤɤ���˼��Ԥ��뤫��
     */
    void test_removeConfigurationSet_with_inexist_configuration_id()
    {
      RTC::Properties nullProp;
      RTC::ConfigAdmin configAdmin(nullProp);
			
      // ����ե����졼����󥻥åȤ��ɲä��Ƥ���
      RTC::Properties configSet("id");
      configSet.setProperty("key", "value");
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.addConfigurationSet(configSet));
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.haveConfig("id"));
			
      // ¸�ߤ��ʤ�����ե����졼����󥻥å�ID����ꤷ�����ˡ��տޤɤ���˼��Ԥ��뤫��
      CPPUNIT_ASSERT_EQUAL(false, configAdmin.removeConfigurationSet("inexist id"));
			
      // ��Ͽ����Ƥ��륳��ե����졼����󥻥åȤϡ����ɤ���¸�ߤ��Ƥ��뤫��
      CPPUNIT_ASSERT_EQUAL(true, configAdmin.haveConfig("id"));
    }
		
    void test_activateConfigurationSet()
    {
      // test_getActiveConfigurationSet()�Ƿ�ͤ�
    }
		
  };
}; // namespace ConfigAdmin

/*
 * Register test suite
 */
CPPUNIT_TEST_SUITE_REGISTRATION(Config::ConfigTests);
CPPUNIT_TEST_SUITE_REGISTRATION(ConfigAdmin::ConfigAdminTests);

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
#endif // ConfigAdmin_cpp