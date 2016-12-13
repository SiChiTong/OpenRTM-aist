// -*- C++ -*-
/*!
 * @file StringUtil.cpp
 * @brief String operation utility
 * @date $Date: 2007-12-31 03:08:07 $
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

#include <algorithm>
#include <iostream>
#include <rtm/StringUtil.h>

/*!
 * @if jp
 * @brief ���ϥ��ȥ꡼�फ��1���ɤ߹���
 * @else
 * @brief Read a line from input stream
 * @endif
 */
int getlinePortable(std::istream& istr, std::string& line)
{
  char c;
  std::stringstream s;

  while (istr.get(c))
    {
      if (c == '\n')
	{
	  break;
	}
      else if (c == '\r')
	{
	  if (istr.peek() == '\n')
	    {
	      istr.ignore();
	    }
	  break;
	}
      else
	{
	  s << c;
	}
    }
  line = s.str();
  return line.size();
}

/*!
 * @if jp
 * @brief ʸ���󤬥��������פ���Ƥ��뤫Ƚ�Ǥ���
 * @else
 * @brief Check whether the character is escaped or not
 * @endif
 */
bool isEscaped(const std::string& str, std::string::size_type pos)
{
  --pos;
  unsigned int i;
  for (i = 0; (pos >= 0) && str[pos] == '\\'; --pos, ++i) ;
  // If the number of \ is odd, delimiter is escaped.
  return (i % 2) == 1;
}

/*!
 * @if jp
 * @brief ʸ����򥨥������פ��뤿���Functor
 * @else
 * @brief Functor to escape string
 * @endif
 */
struct escape_functor
{
  escape_functor() {};
  void operator()(const char c)
  {
    if      (c == '\t')  str += "\\t";
    else if (c == '\n')  str += "\\n";
    else if (c == '\f')  str += "\\f";
    else if (c == '\r')  str += "\\r";
    else if (c == '\\')  str += "\\\\";
    //    else if (c == '\"')  str += "\\\"";
    //    else if (c == '\'')  str += "\\\'";
    else str.push_back(c);
  }
  std::string str;
};

/*!
 * @if jp
 * @brief ʸ����򥨥������פ���
 * @else
 * @brief Escape string
 * @endif
 */
std::string escape(const std::string str)
{
  return for_each(str.begin(), str.end(), escape_functor()).str;
}

/*!
 * @if jp
 * @brief ʸ����򥢥󥨥������פ����Functor
 * @else
 * @brief Functor to unescape string
 * @endif
 */
struct unescape_functor
{
  unescape_functor() : count(0) {};
  void operator()(char c)
  {
    if (c == '\\')
      {
	++count;
	if (!(count % 2))
	  {
	    str.push_back(c);
	  }
      }
    else
      {
	if (count > 0 && (count % 2))
	  {
	    count = 0;
	    if      (c == 't')  str.push_back('\t');
	    else if (c == 'n')  str.push_back('\n');
	    else if (c == 'f')  str.push_back('\f');
	    else if (c == 'r')  str.push_back('\r');
	    else if (c == '\"') str.push_back('\"');
	    else if (c == '\'') str.push_back('\'');
	    else str.push_back(c);
	  }
	else
	  {
	    count = 0;
	    str.push_back(c);
	  }
      }
  }
  std::string str;
  int count;
};

/*!
 * @if jp
 * @brief ʸ����Υ��������פ��᤹
 * @else
 * @brief Unescape string
 * @endif
 */
std::string unescape(const std::string str)
{
  return for_each(str.begin(), str.end(), unescape_functor()).str;
}

/*!
 * @if jp
 * @brief ʸ�������Ƭ�ζ���ʸ����������
 * @else
 * @brief Erase the head blank characters of string
 * @endif
 */
void eraseHeadBlank(std::string& str)
{
  if (str.empty()) return;
  while (str[0] == ' ' || str[0] == '\t') str.erase(0, 1);
}

/*!
 * @if jp
 * @brief ʸ����������ζ���ʸ����������
 * @else
 * @brief Erase the tail blank characters of string
 * @endif
 */
void eraseTailBlank(std::string& str)
{
  if (str.empty()) return;
  while ((str[str.size() - 1] == ' ' || str[str.size() - 1] == '\t') &&
	 !isEscaped(str, str.size() - 1))
    str.erase(str.size() - 1, 1);
}

/*!
 * @if jp
 * @brief ʸ������֤�������
 * @else
 * @brief Replace string
 * @endif
 */
void replaceString(std::string& str, const std::string from,
		   const std::string to)
{
  std::string::size_type pos(0);
  
  while (pos != std::string::npos)
    {
      pos = str.find(from, pos);
      if (pos == std::string::npos) break;
      str.replace(pos, from.size(), to);
      pos += to.size();
    }
}

/*!
 * @if jp
 * @brief ʸ�����ʬ��ʸ����ʬ�䤹��
 * @else
 * @brief Split string by delimiter
 * @endif
 */
std::vector<std::string> split(const std::string& input,
			       const std::string& delimiter)
{
  typedef std::string::size_type size;
  std::vector<std::string> results;
  size delim_size = delimiter.size();
  size found_pos(0), begin_pos(0), pre_pos(0), substr_size(0);
  
  if (input.empty()) return results;
  
  //  if (input.substr(0, delim_size) == delimiter)
  //    begin_pos = pre_pos = delim_size;
  
  while (1)
    {
      //    REFIND:
      found_pos = input.find(delimiter, begin_pos);
      if (found_pos == std::string::npos) 
	{
	  std::string substr(input.substr(pre_pos));
	  eraseHeadBlank(substr);
	  eraseTailBlank(substr);
	  if (!substr.empty()) results.push_back(substr);
	  break;
	}
      /*
	if (isEscaped(input, found_pos))
	{
	begin_pos = found_pos + delim_size;
	goto REFIND;
	}
      */
      substr_size = found_pos - pre_pos;
      if (substr_size >= 0)
	{
	  std::string substr(input.substr(pre_pos, substr_size));
	  eraseHeadBlank(substr);
	  eraseTailBlank(substr);
	  if (!substr.empty()) results.push_back(substr);
	}
      begin_pos = found_pos + delim_size;
      pre_pos   = found_pos + delim_size;
    }
  return results;
}

/*!
 * @if jp
 * @brief ��ʸ�����Ѵ����� Functor
 * @else
 * @brief Functor to convert to capital letters
 * @endif
 */
struct Toupper
{
  void operator()(char &c)
  {
    c = toupper(c);
  }
};

/*!
 * @if jp
 * @brief Ϳ����줿ʸ�����bool�ͤ��Ѵ�����
 * @else
 * @brief Convert given string into bool value
 * @endif
 */
bool toBool(std::string str, std::string yes, std::string no, 
	    bool default_value)
{
  std::for_each(str.begin(), str.end(), Toupper());
  std::for_each(yes.begin(), yes.end(), Toupper());
  std::for_each(no.begin(),  no.end(),  Toupper());
  
  if (str.find(yes) != std::string::npos)
    return true;
  else if (str.find(no) != std::string::npos)
    return false;
  else
    return default_value;
}

/*!
 * @if jp
 * @brief Ϳ����줿ʸ�������Хѥ����ɤ�����Ƚ�Ǥ���
 * @else
 * @brief Investigate whether the given string is absolute path or not
 * @endif
 */
bool isAbsolutePath(const std::string& str)
{
  // UNIX absolute path is begun from '/'
  if (str[0] == '/') return true;
  // Windows absolute path is begun from '[a-zA-Z]:\'
  if (isalpha(str[0]) && (str[1] == ':') && str[2] == '\\') return true;
  // Windows network file path is begun from '\\'
  if (str[0] == '\\' && str[1] == '\\') return true;
  
  return false;
}

/*!
 * @if jp
 * @brief Ϳ����줿ʸ����URL���ɤ�����Ƚ�Ǥ���
 * @else
 * @brief Investigate whether the given string is URL or not
 * @endif
 */
bool isURL(const std::string& str)
{
  typedef std::string::size_type size;
  size pos;
  if (str.empty()) return false;
  pos = str.find(":");
  if ((pos != 0) &&
      (pos != std::string::npos) &&
      (str[pos + 1] == '/') &&
      (str[pos + 2] == '/'))
    return true;
  return false;
}

/*!
 * @if jp
 * @brief �ꥹ�����ʸ����򸡺����� Functor
 * @else
 * @brief Functor to find string in a list
 * @endif
 */
struct unique_strvec
{
  void operator()(const std::string& s)
  {
    if (std::find(str.begin(), str.end(), s) == str.end())
      return str.push_back(s);
  }
  std::vector<std::string> str;
};

/*!
 * @if jp
 * @brief Ϳ����줿ʸ����ꥹ�Ȥ����ʣ����
 * @else
 * @brief Eliminate duplication from the given string list
 * @endif
 */
std::vector<std::string> unique_sv(std::vector<std::string> sv)
{
  return std::for_each(sv.begin(), sv.end(), unique_strvec()).str;
}

/*!
 * @if jp
 * @brief Ϳ����줿ʸ����ꥹ�Ȥ���CSV������
 * @else
 * @brief Create CSV file from the given string list
 * @endif
 */
std::string flatten(std::vector<std::string> sv)
{
  if (sv.size() == 0) return "";
  
  std::string str;
  for (int i(0), len(sv.size() - 1); i < len; ++i)
    {
      str += sv[i] + ", ";
    }
  return str + sv.back();
}

/*!
 * @if jp
 * @brief Ϳ����줿ʸ����ꥹ�Ȥ�����ꥹ�Ȥ��Ѵ�
 * @else
 * @brief Convert the given string list into the argument list
 * @endif
 */
char** toArgv(const std::vector<std::string>& args)
{
  char** argv;
  int argc(args.size());
  
  argv = new char*[argc];
  
  for (int i(0); i < argc; ++i)
    {
      int sz(args[i].size());
      argv[i] = new char[sz + 1];
      args[i].copy(argv[i], sz);
      argv[i][sz] = '\0';
    }
  return argv;
}