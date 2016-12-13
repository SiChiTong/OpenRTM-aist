// -*- C++ -*-
/*!
 * @file ManagerConfig.cpp
 * @brief RTC manager configuration
 * @date $Date: 2007-12-31 03:08:04 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2003-2008
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

#include <rtm/RTC.h>
#include <rtm/ManagerConfig.h>
#include <ace/Get_Opt.h>
#include <fstream>
#include <iostream>
#include <ace/OS.h>
#include <rtm/DefaultConfiguration.h>

namespace RTC
{
  
  // The list of default configuration file path.
  const char* ManagerConfig::config_file_path[] = 
    {
      "./rtc.conf",
      "/etc/rtc.conf",
      "/etc/rtc/rtc.conf",
      "/usr/local/etc/rtc.conf",
      "/usr/local/etc/rtc/rtc.conf",
      NULL
    };
  
  // Environment value to specify configuration file
  const char* ManagerConfig::config_file_env = "RTC_MANAGER_CONFIG";
  
  /*!
   * @if jp
   * @brief ���󥹥ȥ饯��
   * @else
   * @brief Constructor
   * @endif
   */
  ManagerConfig::ManagerConfig()
  {
  }
  
  /*!
   * @if jp
   * @brief ���󥹥ȥ饯��
   * @else
   * @brief Constructor
   * @endif
   */
  ManagerConfig::ManagerConfig(int argc, char** argv)
  {
    init(argc, argv);
  }
  
  /*!
   * @if jp
   * @brief �ǥ��ȥ饯��
   * @else
   * @brief Destructor
   * @endif
   */
  ManagerConfig::~ManagerConfig()
  {
  }
  
  /*!
   * @if jp
   * @brief �����
   * @else
   * @brief Initialization
   * @endif
   */
  void ManagerConfig::init(int argc, char** argv)
  {
    parseArgs(argc, argv);
  }
  
  /*!
   * @if jp
   * @brief Configuration ����� Property �����ꤹ��
   * @else
   * @brief Specify the configuration information to the Property
   * @endif
   */
  void ManagerConfig::configure(Properties& prop)
  {
    prop.setDefaults(default_config);
    if (findConfigFile())
      {
	std::ifstream f(m_configFile.c_str());
	if (f.is_open())
	  {
	    prop.load(f);
	    f.close();
	  }
      }
    setSystemInformation(prop);
  }
  
  /*!
   * @if jp
   * @brief ���ޥ�ɰ�����ѡ�������
   * @else
   * @brief Parse the command arguments
   * @endif
   */
  void ManagerConfig::parseArgs(int argc, char** argv)
  {
    ACE_Get_Opt get_opts(argc, argv, "f:l:o:d", 0);
    int opt;
    
    //  if (argc == 0) return true;
    
    while ((opt = get_opts()) > 0)
      {
	switch (opt)
	  {
	    // Specify configuration file not default
	  case 'f':
	    m_configFile = get_opts.optarg;
	    break;
	  case 'l':
	    //	    m_configFile = get_opts.optarg;
	    break;
	  case 'o':
	    //	    m_configFile = get_opts.optarg;
	    break;
	    // Use default configuration settings
	  case 'd': 
	    //	    m_DebugMode = true;
	    break;
	  default:
	    ;
	  }
      }
    return;
  }
  
  /*!
   * @if jp
   * @brief Configuration file �θ���
   * @else
   * @brief Find the configuration file
   * @endif
   */
  bool ManagerConfig::findConfigFile()
  {
    // Check existance of configuration file given command arg
    if (m_configFile != "") 
      {
	if (fileExist(m_configFile))
	  {
	    return true;
	  }
      }
    
    // Search rtc configuration file from environment variable
    char* env = getenv(config_file_env);
    if (env != NULL)
      {
	if (fileExist(env))
	  {
	    m_configFile = env;
	    return true;
	  }
      }
    // Search rtc configuration file from default search path
    int i = 0;
    while (config_file_path[i] != NULL)
      {
	if (fileExist(config_file_path[i]))
	  {
	    m_configFile = config_file_path[i];
	    return true;
	  }
	++i;
      }
    return false;
  }
  
  /*!
   * @if jp
   * @brief �����ƥ��������ꤹ��
   * @else
   * @brief Set system information
   * @endif
   */
  void ManagerConfig::setSystemInformation(Properties& prop)
  { 
    //
    // Get system information by using ACE_OS::uname (UNIX/Windows)
    // 
    ACE_utsname  sysinfo;
    if (ACE_OS::uname(&sysinfo) != 0)
      {
	return;
      }
    
    //
    // Getting current proccess pid by using ACE_OS::getpid() (UNIX/Windows)
    //
    pid_t pid = ACE_OS::getpid();
    char pidc[8];
    sprintf(pidc, "%d", pid);
    
    prop.setProperty("manager.os.name",     sysinfo.sysname);
    prop.setProperty("manager.os.release",  sysinfo.release);
    prop.setProperty("manager.os.version",  sysinfo.version);
    prop.setProperty("manager.os.arch",     sysinfo.machine);
    prop.setProperty("manager.os.hostname", sysinfo.nodename);
    prop.setProperty("manager.pid",         pidc);
    
    return;
  }
  
  /*!
   * @if jp
   * @brief �ե������¸�߳�ǧ
   * @else
   * @brief Check the file existence
   * @endif
   */
  bool ManagerConfig::fileExist(const std::string& filename)
  {
    std::ifstream infile;
    infile.open(filename.c_str(), std::ios::in);
    // fial() 0: ok, !0: fail
    if (infile.fail() != 0) 
      {
	infile.close();
	return false;
      }
    else
      {
	infile.close();
	return true;
      }
    return false;
  }
}