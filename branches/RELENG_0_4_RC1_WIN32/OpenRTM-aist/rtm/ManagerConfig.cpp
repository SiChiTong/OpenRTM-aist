// -*- C++ -*-
/*!
 * @file ManagerConfig.cpp
 * @brief RTC manager configuration
 * @date $Date: 2007-01-14 19:43:01 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2003-2005
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id: ManagerConfig.cpp,v 1.5 2007-01-14 19:43:01 n-ando Exp $
 *
 */

/*
 * $Log: not supported by cvs2svn $
 * Revision 1.4  2006/11/06 01:26:21  n-ando
 * Some trivial fixes.
 *
 * Revision 1.3  2006/10/23 08:38:17  n-ando
 * To get Property after "init()" calling, getConfig() was added.
 *
 * Revision 1.2  2006/10/17 19:29:53  n-ando
 * Trivial fix.
 *
 * Revision 1.1  2006/10/17 10:21:31  n-ando
 * The first commitment.
 *
 *
 */

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
   * @brief ManagerConfig ���󥹥ȥ饯��
   * @else
   * @brief ManagerConfig constructor
   * @endif
   */
  ManagerConfig::ManagerConfig()
    : m_properties(default_config)
  {
  }


  /*!
   * @if jp
   * @brief ManagerConfig ���󥹥ȥ饯��
   * @else
   * @brief ManagerConfig constructor
   * @endif
   */
  ManagerConfig::ManagerConfig(int argc, char** argv)
    : m_properties(default_config)
  {
    init(argc, argv);
  }


  /*!
   * @if jp
   * @brief ManagerConfig �ǥ��ȥ饯��
   * @else
   * @brief ManagerConfig destructor
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
  Properties ManagerConfig::init()
  {
    if (findConfigFile())
      {
	std::ifstream f(m_configFile.c_str());
	m_properties.load(f);
	f.close();
	setSystemInformation(m_properties);
	return m_properties;
      }
    else
      {
	return m_properties;
      }
  }


  /*!
   * @if jp
   * @brief �����
   * @else
   * @brief Initialization
   * @endif
   */
  Properties ManagerConfig::init(int argc, char** argv)
  {
    parseArgs(argc, argv);
    return init();
  }


  /*!
   * @if jp
   * @brief ����ե�����졼�������������
   * @else
   * @brief Get configuration value.
   * @endif
   */
  Properties ManagerConfig::getConfig() const
  {
    return m_properties;
  }


  /*!
   * @if jp
   * @brief ���ޥ�ɰ�����ѡ�������
   * @else
   * @brief Parse command arguments
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
   * @brief Configuration file ��õ��
   * @else
   * @brief Find configuration file
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
   *
   * manager.os.name: 
   * manager.os.release:
   * maanger.os.version:
   * manager.os.arch:
   * manager.os.hostname:
   * manager.pid:
   *
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
   * @brief �ե����뤬¸�ߤ��뤫�ɤ����Τ����
   * @else
   * @brief Check file existance
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