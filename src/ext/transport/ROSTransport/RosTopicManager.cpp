﻿// -*- C++ -*-
/*!
 * @file  RosTopicManager.cpp
 * @brief RosTopicManager class
 * @date  $Date: 2019-01-31 03:08:03 $
 * @author Nobuhiko Miyamoto <n-miyamoto@aist.go.jp>
 *
 * Copyright (C) 2019
 *     Nobuhiko Miyamoto
 *     Robot Innovation Research Center,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *
 *     All rights reserved.
 *
 *
 */

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <WinSock2.h>
#endif
#include "RosTopicManager.h"
#include <ros/xmlrpc_manager.h>
#include <ros/network.h>
#include <xmlrpcpp/XmlRpcSocket.h>
#include <ros/connection.h>
#include <ros/connection_manager.h>


namespace ros
{
  namespace network
  {
    void init(const M_string& remappings);
  }
}


namespace RTC
{
  RosTopicManager* RosTopicManager::manager = NULL;
  coil::Mutex RosTopicManager::mutex;


  /*!
   * @if jp
   * @brief コンストラクタ
   *
   * コンストラクタ
   *
   * @else
   * @brief Constructor
   *
   * Constructor
   *
   * @endif
   */
  RosTopicManager::RosTopicManager()
  {

  }

  /*!
   * @if jp
   * @brief コピーコンストラクタ
   *  
   * @param manager RosTopicManager
   *
   * @else
   * @brief Copy Constructor
   *
   * @param manager RosTopicManager
   *
   * @endif
   */
  RosTopicManager::RosTopicManager(const RosTopicManager &/*mgr*/)
  {
    
  }
  /*!
   * @if jp
   * @brief デストラクタ
   *
   * デストラクタ
   *
   * @else
   * @brief Destructor
   *
   * Destructor
   *
   * @endif
   */
  RosTopicManager::~RosTopicManager()
  {
    
  }

  /*!
   * @if jp
   * @brief トピックマネージャ開始
   *
   *
   * @else
   * @brief 
   *
   * 
   *
   * @endif
   */
  void RosTopicManager::start()
  {
    ros::M_string remappings;
    ros::network::init(remappings);

    m_xmlrpc_manager = ros::XMLRPCManager::instance();

    m_xmlrpc_manager->bind("requestTopic", boost::bind(&RosTopicManager::requestTopicCallback, this, _1, _2));
    m_xmlrpc_manager->bind("publisherUpdate", boost::bind(&RosTopicManager::pubUpdateCallback, this, _1, _2));
    

    m_poll_manager = ros::PollManager::instance();
    m_poll_manager->start();
    m_tcpserver_transport = boost::make_shared<ros::TransportTCP>(&m_poll_manager->getPollSet());
    
    m_tcpserver_transport->listen(ros::network::getTCPROSPort(), 
                            100, 
                            boost::bind(&RosTopicManager::tcprosAcceptConnection, this, _1));

    m_xmlrpc_manager->start();
  }

  /*!
   * @if jp
   * @brief 終了処理
   * XML-RPC、TCPサーバー、ポールマネージャを終了する
   *
   * @else
   * @brief
   *
   *
   *
   * @endif
   */
  void RosTopicManager::shutdown()
  {
      m_xmlrpc_manager->shutdown();
      m_tcpserver_transport->close();
      m_poll_manager->shutdown();
  }

  /*!
   * @if jp
   * @brief requestTopic関数リモート呼び出し時のコールバック関数
   * サブスクライバーからの接続時に呼ばれる
   * 
   * @param params
   * @param result
   *
   * @else
   * @brief 
   *
   * @param params
   * @param result
   * 
   *
   * @endif
   */
  void RosTopicManager::requestTopicCallback(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result)
  {
      //RTC_PARANOID(("requestTopicCallback()"));
      if (params.getType() != XmlRpc::XmlRpcValue::TypeArray)
      {
          result = ros::xmlrpc::responseInt(0, ros::console::g_last_error_message, 0);
          return;
      }
      if (params.size() < 3)
      {
          result = ros::xmlrpc::responseInt(0, ros::console::g_last_error_message, 0);
          return;
      }

      if (params[0].getType() != XmlRpc::XmlRpcValue::TypeString)
      {
          result = ros::xmlrpc::responseInt(0, ros::console::g_last_error_message, 0);
          return;
      }
      if (params[1].getType() != XmlRpc::XmlRpcValue::TypeString)
      {
          result = ros::xmlrpc::responseInt(0, ros::console::g_last_error_message, 0);
          return;
      }

      std::string callerid = params[0];
      std::string topic = params[1];
      
      //RTC_VERBOSE(("Caller ID:%s",callerid.c_str()));
      //RTC_VERBOSE(("Topic Name:%s",topic.c_str()));
      
      XmlRpc::XmlRpcValue protocols = params[2];
      if (protocols.getType() != XmlRpc::XmlRpcValue::TypeArray)
      {
          result = ros::xmlrpc::responseInt(0, ros::console::g_last_error_message, 0);
          return;
      }
      for(int i=0;i < protocols.size();i++)
      {
        if (protocols[i].getType() != XmlRpc::XmlRpcValue::TypeArray)
        {
          result = ros::xmlrpc::responseInt(0, ros::console::g_last_error_message, 0);
          //RTC_ERROR(("Protocol Name is invalid value"));
          return;
        }
        if (protocols[i][0].getType() != XmlRpc::XmlRpcValue::TypeString)
        {
            result = ros::xmlrpc::responseInt(0, ros::console::g_last_error_message, 0);
            return;
        }
        std::string protocol = std::string(protocols[i][0]);
        //RTC_VERBOSE(("Protocol Type:%s",protocol.c_str()));
      
        if (protocol == std::string("TCPROS"))
        {
          //RTC_VERBOSE(("TCPROS Connection create."));
          XmlRpc::XmlRpcValue tcpros_params;
          
          tcpros_params[0] = std::string("TCPROS");
          tcpros_params[1] = ros::network::getHost();
          tcpros_params[2] = int(m_tcpserver_transport->getServerPort());
          result.setSize(3);
          result[0] = int(1);
          result[1] = std::string();
          result[2] = tcpros_params;
          return;
        }
      }
      result = ros::xmlrpc::responseInt(0, ros::console::g_last_error_message, 0);

  }

  /*!
   * @if jp
   * @brief pubUpdate関数リモート呼び出し時のコールバック関数
   * パブリッシャーの更新時に呼ばれる
   * 
   * @param params
   * @param result
   *
   * @else
   * @brief 
   *
   * @param params
   * @param result
   * 
   *
   * @endif
   */
  void RosTopicManager::pubUpdateCallback(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result)
  {
      //RTC_PARANOID(("pubUpdateCallback()"));
      if (params.getType() != XmlRpc::XmlRpcValue::TypeArray)
      {
          result = ros::xmlrpc::responseInt(0, ros::console::g_last_error_message, 0);
          return;
      }
      if (params.size() < 3)
      {
          result = ros::xmlrpc::responseInt(0, ros::console::g_last_error_message, 0);
          return;
      }

      if (params[0].getType() != XmlRpc::XmlRpcValue::TypeString)
      {
          result = ros::xmlrpc::responseInt(0, ros::console::g_last_error_message, 0);
          return;
      }
      if (params[1].getType() != XmlRpc::XmlRpcValue::TypeString)
      {
          result = ros::xmlrpc::responseInt(0, ros::console::g_last_error_message, 0);
          return;
      }

      std::string caller_id = params[0];
      std::string topic = params[1];
      
      //RTC_VERBOSE(("Caller ID:%s",caller_id.c_str()));
      //RTC_VERBOSE(("Topic Name:%s",topic.c_str()));
      
      
      if(m_cons.count(topic) == 0)
      {
        m_cons[topic] = std::vector<std::string>();
      }
      
      if (params[2].getType() != XmlRpc::XmlRpcValue::TypeArray)
      {
        //RTC_ERROR(("Protcol is invalid value."));
      
        result = ros::xmlrpc::responseInt(0, ros::console::g_last_error_message, 0);
        return;
      }
      
      std::vector<std::string> new_ = std::vector<std::string>();
      std::vector<std::string> old_ = m_cons[topic];
      
      for (int i = 0; i < params[2].size(); i++)
      {
        if (params[2][i].getType() != XmlRpc::XmlRpcValue::TypeString)
        {
            result = ros::xmlrpc::responseInt(0, ros::console::g_last_error_message, 0);
            return;
        }
        std::string xmlrpc_uri = params[2][i];
        if (xmlrpc_uri.empty())
        {
            result = ros::xmlrpc::responseInt(0, ros::console::g_last_error_message, 0);
            return;
        }

        for(auto & subscriber : m_subscribers)
        {
          //RTC_VERBOSE(("Connect TCP"));
          //RTC_VERBOSE(("Caller ID:%s",caller_id.c_str()));
          //RTC_VERBOSE(("Topic Name:%s",topic.c_str()));
          //RTC_VERBOSE(("URI:%s",xmlrpc_uri.c_str()));
          subscriber->connectTCP(caller_id, topic, xmlrpc_uri);
        }
        new_.push_back(xmlrpc_uri);
      }
      
      for(auto & old_uri : old_)
      {
        std::vector<std::string>::iterator itr_uri = std::find(new_.begin(), new_.end(), old_uri);
        size_t index = std::distance( new_.begin(), itr_uri );
      
        if (index == new_.size()) 
        {
          //RTC_INFO(("Delete Connector:%s %s %s", caller_id.c_str(), topic.c_str(), old_uri->c_str()));
          for(auto & subscriber : m_subscribers)
          {
            subscriber->deleteTCPConnector(caller_id, topic, old_uri);
          }
        }
      }

      m_cons[topic] = new_;
      
      result = ros::xmlrpc::responseInt(1, "", 0);
      
      
      
  }



  /*!
   * @if jp
   * @brief パブリッシャーの追加
   * 
   * @param publisher パブリッシャー
   *
   * @else
   * @brief add publisher
   *
   * @param publisher 
   * 
   *
   * @endif
   */
  void RosTopicManager::addPublisher(ROSOutPort *publisher)
  {

    if(!existPublisher(publisher))
    {
      m_publishers.push_back(publisher);
    }
  }


  /*!
   * @if jp
   * @brief サブスクライバーの追加
   * 
   * @param subscriber サブスクライバー
   *
   * @else
   * @brief add subscriber
   *
   * @param subscriber 
   * 
   *
   * @endif
   */
  void RosTopicManager::addSubscriber(ROSInPort *subscriber)
  {
    if(!existSubscriber(subscriber))
    {
      m_subscribers.push_back(subscriber);
    }
    
  }

  /*!
   * @if jp
   * @brief パブリッシャーの削除
   * 
   * @param publisher パブリッシャー
   *
   * @else
   * @brief remove publisher
   *
   * @param publisher 
   * 
   *
   * @endif
   */
  bool RosTopicManager::removePublisher(ROSOutPort *publisher)
  {
    if(existPublisher(publisher))
    {
      m_publishers.erase(remove(m_publishers.begin(), m_publishers.end(), publisher), m_publishers.end());
      return true;
    }
    return false;
  }

  /*!
   * @if jp
   * @brief サブスクライバーの削除
   * 
   * @param subscriber サブスクライバー
   *
   * @else
   * @brief remove subscriber
   *
   * @param subscriber 
   * 
   *
   * @endif
   */
  bool RosTopicManager::removeSubscriber(ROSInPort *subscriber)
  {
    if(existSubscriber(subscriber))
    {
      m_subscribers.erase(remove(m_subscribers.begin(), m_subscribers.end(), subscriber), m_subscribers.end());
      return true;
    }
    return false;
  }


  /*!
   * @if jp
   * @brief パブリッシャーの存在確認
   * 
   * @param publisher パブリッシャー
   * @return True：存在する
   *
   * @else
   * @brief 
   *
   * @param publisher 
   * @return 
   * 
   *
   * @endif
   */
  bool RosTopicManager::existPublisher(ROSOutPort *publisher)
  {
    std::vector<ROSOutPort*>::iterator itr = std::find(m_publishers.begin(), m_publishers.end(), publisher);
    size_t index = std::distance( m_publishers.begin(), itr );
    
    return (index != m_publishers.size());
  }


  /*!
   * @if jp
   * @brief サブスクライバーの存在確認
   * 
   * @param subscriber サブスクライバー
   * @return True：存在する
   *
   * @else
   * @brief 
   *
   * @param subscriber 
   * @return 
   * 
   *
   * @endif
   */
  bool RosTopicManager::existSubscriber(ROSInPort *subscriber)
  {
    std::vector<ROSInPort*>::iterator itr = std::find(m_subscribers.begin(), m_subscribers.end(), subscriber);
    size_t index = std::distance( m_subscribers.begin(), itr );
    return (index != m_subscribers.size());
  }

  /*!
   * @if jp
   * @brief 初期化関数
   * 
   * @return インスタンス
   *
   * @else
   * @brief 
   *
   * @return 
   * 
   *
   * @endif
   */
  RosTopicManager* RosTopicManager::init()
  {
    Guard guard(mutex);
    if (!manager)
    {
      manager = new RosTopicManager();
      manager->start();
    }
    return manager;
  }

  /*!
   * @if jp
   * @brief インスタンス取得
   * 
   * @return インスタンス
   *
   * @else
   * @brief 
   *
   * @return 
   * 
   *
   * @endif
   */
  RosTopicManager& RosTopicManager::instance()
  {
    Guard guard(mutex);
    if (!manager)
    {
      manager = new RosTopicManager();
      manager->start();
    }
    return *manager;
  }

  /*!
   * @if jp
   * @brief RosTopicManagerが初期化されている場合に終了処理を呼び出す
   *
   *
   * @else
   * @brief
   *
   * @return
   *
   *
   * @endif
   */
  void RosTopicManager::shutdown_global()
  {
      Guard guard(mutex);
      if (manager)
      {
          manager->shutdown();
      }
  }
}

