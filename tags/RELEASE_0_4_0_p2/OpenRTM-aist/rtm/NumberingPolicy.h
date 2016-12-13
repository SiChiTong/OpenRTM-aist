// -*- C++ -*-
/*!
 * @file NumberingPolicy.h
 * @brief Object numbering policy class
 * @date $Date: 2007-01-21 09:52:28 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2006
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id: NumberingPolicy.h,v 1.2 2007-01-21 09:52:28 n-ando Exp $
 *
 */

/*
 * $Log: not supported by cvs2svn $
 * Revision 1.1  2006/11/04 21:07:40  n-ando
 * Component numbering rule was derived into policy class.
 *
 */

#ifndef NumberingPolicy_h
#define NumberingPolicy_h

#include <string>
#include <vector>

class NumberingPolicy
{
 public:
  struct ObjectNotFound {};
  virtual ~NumberingPolicy(){};
  virtual std::string onCreate(void* obj) = 0;
  virtual void onDelete(void* obj) = 0;
};


class DefaultNumberingPolicy
  : public NumberingPolicy 
{
public:
  DefaultNumberingPolicy() : m_num(0) {};
  virtual ~DefaultNumberingPolicy(){};
  virtual std::string onCreate(void* obj);
  virtual void onDelete(void* obj);

protected:
  long int find(void* obj);

private:
  int m_num;
  std::vector<void*> m_objects;
};


#endif // NumberingPolicy_h