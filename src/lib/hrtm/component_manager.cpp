// -*- C++ -*-
/*!
 * @file component_manager.cpp
 * @brief RTC::ComponentManager wrapper class for hrtm::ComponentManager
 * @date $Date$
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2016
 *     Noriaki Ando
 *     National Institute of
 *     Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id$
 *
 */

//#include <rtm/Manager.h>
//#include <rtm/Factory.h>
//#include <rtm/RTObject.h>
#include <hrtm/data_flow_component.h>
#include <hrtm/component_manager.h>
//#include <hrtm/logger.h>

namespace hrtm
{
  // static members initialization
  ModuleInitProc ComponentManager::initProc;
  ComponentManager* ComponentManager::manager = NULL;
  coil::Mutex ComponentManager::mutex;
  
  hrtm::DataFlowComponent*
  ComponentManager::create_component(const char* component_name)
  {
    RTC::RTObject_impl* rtcomp(RTC::Manager::createComponent(component_name));
    return dynamic_cast<hrtm::DataFlowComponent*>(rtcomp);
  }

  void
  ComponentManager::set_init_proc(void(*init_proc)(hrtm::ComponentManager*))
  {
    initProc = init_proc;
    setModuleInitProc(hrtm::ComponentManager::init_proc);
  }
  bool ComponentManager::activate()
  {
    return activateManager();
  }
  void ComponentManager::run(bool non_block)
  {
    runManager(non_block);
  }
  void ComponentManager::init_proc(RTC::Manager* mgr)
  {
    ::hrtm::ComponentManager::initProc(manager);
  }
  ComponentManager& ComponentManager::instance(int argc, char** argv)
  {
   
    // DCL for singleton
    if (!manager)
      {
        Guard guard(mutex);
        if (!manager)
          {
            manager = new ComponentManager();
            RTC::Manager::manager = manager;
            printf("init ComponentManager: %x\n", manager);
            manager->initManager(argc, argv);
            manager->initLogger();
            manager->initORB();
            manager->initNaming();
            manager->initFactories();
            manager->initExecContext();
            manager->initComposite();
            manager->initTimer();
            manager->initManagerServant();
          }
      }
    return *manager;
  }

  ComponentManager::ComponentManager()
    : RTC::Manager()
  {
  }
} // namespace hrtm
