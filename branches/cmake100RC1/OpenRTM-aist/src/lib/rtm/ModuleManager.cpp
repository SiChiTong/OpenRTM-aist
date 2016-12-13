// -*- C++ -*-
/*!
 * @file ModuleManager.cpp
 * @brief Loadable modules manager class
 * @date $Date: 2007-12-31 03:08:04 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2006-2008
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

// ACE includes
#include <coil/File.h>

// RTC includes
#include <rtm/Manager.h>
#include <rtm/ModuleManager.h>
#include <coil/stringutil.h>

namespace RTC
{
  
  /*!
   * @if jp
   * @brief ���󥹥ȥ饯��
   * @else
   * @brief Constructor
   * @endif
   */
  ModuleManager::ModuleManager(coil::Properties& prop)
    : m_properties(prop)
  {
    m_configPath      = coil::split(prop[CONFIG_PATH], ",");
    for_each(m_configPath.begin(), m_configPath.end(), coil::eraseHeadBlank);
    m_loadPath        = coil::split(prop[MOD_LOADPTH], ",");
    for_each(m_loadPath.begin(), m_loadPath.end(), coil::eraseHeadBlank);
    m_absoluteAllowed = coil::toBool(prop[ALLOW_ABSPATH], "yes", "no", false);
    m_downloadAllowed = coil::toBool(prop[ALLOW_URL], "yes", "no", false);
    m_initFuncSuffix  = prop[INITFUNC_SFX];
    m_initFuncPrefix  = prop[INITFUNC_PFX];
  }
  
  /*!
   * @if jp
   * @brief �ǥ��ȥ饯��
   * @else
   * @brief Destructor
   * @endif
   */
  ModuleManager::~ModuleManager()
  {
    unloadAll();
  }
  
  /*!
   * @if jp
   * @brief �⥸�塼��Υ�����
   * @else
   * @brief Load the module
   * @endif
   */
  std::string ModuleManager::load(const std::string& file_name)
  {
    if (file_name == "") throw InvalidArguments("Invalid file name.");
    
    if (coil::isURL(file_name))
      {
	if (!m_downloadAllowed)
	  {
	    throw NotAllowedOperation("Downloading module is not allowed.");
	  }
	else
	  {
	    throw NotFound("Not implemented.");
	  }
      }
    
    // Find local file from load path or absolute directory
    std::string file_path;
    if (coil::isAbsolutePath(file_name))
      {
	if (!m_absoluteAllowed)
	  {
	    throw NotAllowedOperation("Absolute path is not allowed");
	  }
	else
	  {
	    file_path = file_name;
	  }
      }
    else
      {
	file_path = findFile(file_name, m_loadPath);
      }
    
    // Now file_name is valid full path to module
    if (file_path == "") throw InvalidArguments("Invalid file name.");
    if (!fileExist(file_path)) throw FileNotFound(file_path.c_str());
    
    DLLEntity* dll(new DLLEntity());

    int retval =  dll->dll.open(file_path.c_str());
    if (retval != 0)
      {
        delete dll;
	throw Error("DLL open failed.");
      }
    dll->properties["file_path"] = file_path;
    bool ret = m_modules.registerObject(dll);
    if (!ret) {
      delete dll;
    }
    
    return file_path;
  }
  
  /*!
   * @if jp
   * @brief �⥸�塼��Υ����ɡ������
   * @else
   * @brief Load and initialize the module
   * @endif
   */
  std::string ModuleManager::load(const std::string& file_name,
				  const std::string& init_func)
  {
    std::string name;
    name = load(file_name);
    
    if (name == "")
      {
	throw InvalidOperation("Invalid file name");
      }
    
    //  if (!init_func)
    
    ModuleInitFunc init;
    
    init = (ModuleInitFunc)this->symbol(name.c_str(), init_func);
    
    init(&(Manager::instance()));
    
    return name;
  }
  
  /*!
   * @if jp
   * @brief �⥸�塼��Υ��������
   * @else
   * @brief Unload the module
   * @endif
   */
  void ModuleManager::unload(const std::string& file_name)
  {
    DLLEntity* dll(m_modules.find(file_name.c_str()));
    if (dll == NULL)
      throw NotFound(file_name.c_str());
    
    dll->dll.close();
    m_modules.unregisterObject(file_name.c_str());
    
    if (dll != NULL) {
      delete dll;
    }

    return;
  }
  
  /*!
   * @if jp
   * @brief ���⥸�塼��Υ��������
   * @else
   * @brief Unload all modules
   * @endif
   */
  void ModuleManager::unloadAll()
  {
    std::vector<DLLEntity*> dlls(m_modules.getObjects());

    for (int i(0), len(dlls.size()); i < len; ++i)
      {
        std::string ident(dlls[i]->properties["file_path"]);
        DLLEntity* dll(m_modules.unregisterObject(ident.c_str()));
        dll->dll.close();
      }
    return;
  }
  
  /*!
   * @if jp
   * @brief �⥸�塼��Υ���ܥ�λ���
   * @else
   * @brief Refer to the symbol of the module
   * @endif
   */
  void* ModuleManager::symbol(const std::string& file_name,
			      const std::string& func_name)
    throw (ModuleNotFound, SymbolNotFound)
  {
    // "file_name" should be in modules map.
    DLLEntity* dll(m_modules.find(file_name.c_str()));
    if (dll == NULL)
      {
	throw ModuleNotFound(file_name);
      }
    
    void* func;
    func = dll->dll.symbol(func_name.c_str());
    
    if (!func)
      {
	throw SymbolNotFound(func_name);
      }
    
    return func;
  }
  
  /*!
   * @if jp
   * @brief �⥸�塼������ɥѥ�����ꤹ��
   * @else
   * @brief Set the module load path
   * @endif
   */
  void ModuleManager::setLoadpath(const std::vector<std::string>& load_path)
  {
    m_loadPath = load_path;
    return;
  }
  
  /*!
   * @if jp
   * @brief �⥸�塼������ɥѥ����ɲä���
   * @else
   * @brief Add the module load path
   * @endif
   */
  void ModuleManager::addLoadpath(const std::vector<std::string>& load_path)
  {
    StringVectorConstItr it(load_path.begin());
    StringVectorConstItr it_end(load_path.end());
    
    while (it != it_end)
      {
	m_loadPath.push_back(*it);
	++it;
      }
    
    return;
  }
  
  /*!
   * @if jp
   * @brief �����ɺѤߤΥ⥸�塼��ꥹ�Ȥ��������
   * @else
   * @brief Get the module list that has been loaded
   * @endif
   */
  std::vector<coil::Properties> ModuleManager::getLoadedModules()
  {
    std::vector< DLLEntity* > dlls(m_modules.getObjects());
    std::vector<coil::Properties> modules;
    for (int i(0), len(dlls.size()); i < len; ++i)
      {
        modules.push_back(dlls[i]->properties);
      }
           return modules;
         }
  
    /*!
   * @if jp
   * @brief �����ɲ�ǽ�ʥ⥸�塼��ꥹ�Ȥ��������(̤����)
   * @else
   * @brief Get the loadable module list(not implemented)
   * @endif
   */
    std::vector<coil::Properties> ModuleManager::getLoadableModules()
    {
      std::vector<coil::Properties> prop;
      
      return prop;
    }
  
  /*!
   * @if jp
   * @brief LoadPath ����Υե�����θ���
   * @else
   * @brief Search the file from the LoadPath
   * @endif
   */
  std::string ModuleManager::findFile(const std::string& fname,
				      const std::vector<std::string>& load_path)
  {
    StringVectorConstItr it, it_end;
    std::string file_name(fname);
    
    it     = load_path.begin();
    it_end = load_path.end();
    
    while (it != it_end)
      {
	std::string f((*it) + "/" + file_name);
	if (fileExist(f))
	  {
	    return f;
	  }
	++it;
      }
    
    return std::string("");
  }
  
  /*!
   * @if jp
   * @brief �ե����뤬¸�ߤ��뤫�ɤ����Υ����å�
   * @else
   * @brief Check whether the file exists
   * @endif
   */
  bool ModuleManager::fileExist(const std::string& filename)
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
  
  /*!
   * @if jp
   * @brief ������ؿ�����ܥ����������
   * @else
   * @brief Create initialization function symbol
   * @endif
   */
  std::string ModuleManager::getInitFuncName(const std::string& file_path)
  {
    std::string base_name(coil::basename(file_path.c_str()));
    
    return m_initFuncPrefix + base_name + m_initFuncSuffix;
  }
}; // namespace RTC