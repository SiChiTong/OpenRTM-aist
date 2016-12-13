// -*- C++ -*-
/*!
 * @file RtcSystemLogger.h
 * @brief RT component logger class
 * @date $Date: 2005-05-12 09:06:18 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2003-2005
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id: RtcSystemLogger.h,v 1.1.1.1 2005-05-12 09:06:18 n-ando Exp $
 *
 */

/*
 * $Log: not supported by cvs2svn $
 *
 */

#ifndef RtcSystemLogger_h
#define RtcSystemLogger_h

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include <time.h>
#include <errno.h>

// ACE
#include <ace/Mutex.h>

#include "rtm/config_rtc.h"

#ifdef RTM_GCC2
#define NO_LOGGING
#endif

namespace RTM
{

#ifndef NO_LOGGING 
  template <typename _CharT, typename _Traits=std::char_traits<_CharT> >
  class sync_callback
  {
  public:
	virtual int operator()(const _CharT* s) = 0;
  };

  /*!
   * @if jp
   *
   * @class basic_logbuf
   *
   * @brief �������Хåե����饹
   *
   * basic_filebuf �饤���ʥ����Хåե����饹����������ե�����ؤΥ����Ǥ��Ф�
   * ���������Хåե����饹��
   *
   * @else
   *
   * @class basic_logbuf
   *
   * @brief Logger buffer class
   *
   *
   * @endif
   */
  template <typename _CharT, typename _Traits=std::char_traits<_CharT> >
  class basic_logbuf
	: public std::basic_filebuf<_CharT, _Traits>
  {
  public:
	// Types:
	typedef _CharT                                    std::char_type;
	typedef _Traits                                   std::traits_type;
	typedef std::basic_filebuf<char_type, traits_type> __filebuf_type;

	/*!
	 * @if jp
	 *
	 * @brief basic_logbuf ���饹���󥹥ȥ饯��
	 *
	 * �ǥե���ȥ��󥹥ȥ饯��
	 *
	 * @else
	 *
	 * @brief basic_logbuf class constructor.
	 *
	 * @endif
	 */
	basic_logbuf()
	  : __filebuf_type(), m_pCallBack(NULL)
	{;}

	/*!
	 * @if jp
	 *
	 * @brief basic_logbuf ���饹���󥹥ȥ饯��
	 *
	 * �ե�����̾����ӥ����ץ�⡼�ɤ���ꤷ�ƥ��󥹥ȥ饯�Ȥ��륳�󥹥ȥ饯��
	 *
	 * @else
	 *
	 * @brief basic_logbuf class constructor.
	 *
	 * @endif
	 */
	basic_logbuf(const char_type* s,
				 std::ios_base::openmode mode = std::ios_base::out, 
				 long protection = 0666)
	  : __filebuf_type(), m_pCallBack(NULL)
	{
	  this->open(s, mode);
	}

	/*!
	 * @if jp
	 *
	 * @brief basic_logbuf ���饹�ǥ��ȥ饯��
	 *
	 * �ǥ��ȥ饯�����ե�����򥯥��������롣
	 *
	 * @else
	 *
	 * @brief basic_logbuf class destractor.
	 *
	 * @endif
	 */
	virtual ~basic_logbuf()
	{
	  this->sync();
	  this->close();
	};

	virtual streamsize sputn(const char_type* s, streamsize n)
	{
	  ACE_Guard<ACE_Thread_Mutex> gaurd(m_Mutex);
	  //	  std::string ss(s, n);
	  //	  std::string sss;
	  //	  sss = "HogeDara" + ss;
	  streamsize ssize = this->xsputn(s, n);
	  /*
	  if (m_pCallBack != NULL)
		{
		  (*m_pCallBack)(sss.c_str());
		}
	  */
	  this->sync();
	  return ssize;
	}

	void setSyncCallBack(sync_callback<char_type>& cb)
	{
	  m_pCallBack = &cb;
	}

  protected:
	/*!
	 * @if jp
	 *
	 * @brief �Хåե�Ʊ��
	 *
	 * �Хåե����ե�å��夵�줿�ݤ˸ƤФ��Ʊ���ؿ���
	 *
	 * @else
	 *
	 * @brief synchronize buffer
	 *
	 * @endif
	 */
	virtual int sync()
	{
	  std::string::basic_string<_CharT> s(this->pbase(),
										  this->pptr() - this->pbase());
	  if (m_pCallBack != NULL)
		{
		  (*m_pCallBack)(s.c_str());
		}

	  // __filebuf_type::sync() resets the pointer
	  int ret = __filebuf_type::sync();

	  return ret;
	}

  private:
	ACE_Thread_Mutex m_Mutex;
	sync_callback<char_type>* m_pCallBack;
	
  };



  /*!
   * @if jp
   *
   * @class basic_medlogbuf
   *
   * @brief ���������Хåե����饹
   *
   * basic_streambuf �饤���ʥ����Хåե����饹��basic_filebuf �ؤ����򤹤롣
   * stream ���������ä�ʸ����˥إå��ʤɤ��ղä� basic_filebuf ���Ϥ���
   *
   * @else
   *
   * @class basic_medlogbuf
   *
   * @brief Logger buffer class
   *
   *
   * @endif
   */
  template <typename _CharT, typename _Traits=std::char_traits<_CharT> >
  class basic_medlogbuf
	: public std::basic_streambuf<_CharT, _Traits>
  {
  public:
	// Types:
	typedef _CharT                                    std::char_type;
	typedef _Traits                                   std::traits_type;
	typedef std::basic_streambuf<char_type, traits_type> __streambuf_type;
	typedef std::basic_filebuf<char_type, traits_type>   __filebuf_type;

	/*!
	 * @if jp
	 *
	 * @brief basic_medlogbuf ���饹���󥹥ȥ饯��
	 *
	 * ������ basic_filebuf ���֥������Ȥ������롣
	 * ������ä� filebuf ���Ф��ƥإå������ղä���ʸ������Ϥ���
	 *
	 * @else
	 *
	 * @brief basic_medlogbuf class constructor.
	 *
	 * @endif
	 */
	basic_medlogbuf()
	  : __streambuf_type(), m_pLogbuf(NULL)
	{
	  // W3C standard date and time format.
	  m_DateFmt = "[%Y-%m-%dT%H.%M.%S%Z]";
	}

	basic_medlogbuf(__filebuf_type& filebuf)
	  : __streambuf_type(), m_pLogbuf(&filebuf)
	{
	  char *pStart = m_Data;
	  char *pEnd = m_Data + (LINE_MAX - 1);
	  this->setp(pStart, pEnd);            // �񤭹��ߥݥ��󥿡�
	  this->setg(pStart, pStart, pEnd);    // �ɤ߼��ݥ��󥿡�

	  // W3C standard date and time format.
	  m_DateFmt = "[%Y-%m-%dT%H.%M.%S%Z]";
	}

	/*!
	 * @if jp
	 *
	 * @brief basic_medlogbuf ���饹�ǥ��ȥ饯��
	 *
	 * �ǥ��ȥ饯����
	 *
	 * @else
	 *
	 * @brief basic_medlogbuf class destractor.
	 *
	 * @endif
	 */
	virtual ~basic_medlogbuf()
	{
	  this->sync();
	}

	void setBuffer(__filebuf_type& filebuf)
	{
	  m_pLogbuf = &filebuf;
	  char *pStart = m_Data;
	  char *pEnd = m_Data + (LINE_MAX - 1);
	  this->setp(pStart, pEnd);            // �񤭹��ߥݥ��󥿡�
	  this->setg(pStart, pStart, pEnd);    // �ɤ߼��ݥ��󥿡�
	}
	
	/*!
	 * @if jp
	 *
	 * @brief �إå����ղä��������ե����ޥåȤ���ꤹ�롣
	 *
	 * �ե����ޥåȻ���ʸ����ϰʲ��ΤȤ��ꡣ
	 * %a abbreviated weekday name 
	 * %A full weekday name 
	 * %b abbreviated month name 
	 * %B full month name 
	 * %c the standard date and time string 
	 * %d day of the month, as a number (1-31) 
	 * %H hour, 24 hour format (0-23) 
	 * %I hour, 12 hour format (1-12) 
	 * %j day of the year, as a number (1-366) 
	 * %m month as a number (1-12).
	 *    Note: some versions of Microsoft Visual C++ may use values that range
	 *    from 0-11. 
	 * %M minute as a number (0-59) 
	 * %p locale's equivalent of AM or PM 
	 * %S second as a number (0-59) 
	 * %U week of the year, sunday as the first day 
	 * %w weekday as a decimal (0-6, sunday=0) 
	 * %W week of the year, monday as the first day 
	 * %x standard date string 
	 * %X standard time string 
	 * %y year in decimal, without the century (0-99) 
	 * %Y year in decimal, with the century 
	 * %Z time zone name 
	 * %% a percent sign 
	 *
	 * @else
	 *
	 * @brief Set date/time format.
	 *
	 * @endif
	 */
	void setDateFmt(char* fmt)
	{
	  m_DateFmt = std::string(fmt);
	}

	/*!
	 * @if jp
	 *
	 * @brief �إå����ղä��������ե����ޥåȤ���ꤹ�롣
	 *
	 * @else
	 *
	 * @brief Set date/time format
	 *
	 * @endif
	 */
	void setDateFmt(std::string fmt)
	{
	  m_DateFmt = fmt;
	}

	/*!
	 * @if jp
	 *
	 * @brief �ե����ޥåȤ��줿��������ʸ�����������롣
	 *
	 * @else
	 *
	 * @brief Get formatted date/time string.
	 *
	 * @endif
	 */
	std::string getFmtDate()
	{
	  int maxsize(256);
	  char buf[maxsize];
     
	  /*
	  struct timeval tp;
	  struct timezone tzp;
	  gettimeofday(&tp, &tzp);
	  */
	  time_t timer;
	  struct tm* date;

	  timer = time(NULL);
	  date = localtime(&timer);
	  strftime(buf, maxsize, m_DateFmt.c_str(), date);

	  std::string ret(buf);
	  return ret;
	}

	/*!
	 * @if jp
	 *
	 * @brief �إå��������θ���ղä���ʸ��������ꤹ�롣
	 *
	 * @else
	 *
	 * @brief Set suffix of date/time string of header.
	 *
	 * @endif
	 */
	void setSuffix(char* suffix)
	{
	  m_Suffix = std::string(suffix);
	}

	/*!
	 * @if jp
	 *
	 * @brief �إå��������θ���ղä���ʸ��������ꤹ�롣
	 *
	 * @else
	 *
	 * @brief Set suffix of date/time string of header.
	 *
	 * @endif
	 */
	void setSuffix(std::string suffix)
	{
	  m_Suffix = suffix;
	}

	/*!
	 * @if jp
	 *
	 * @brief �إå��������θ���ղä���ʸ�����������롣
	 *
	 * @else
	 *
	 * @brief Get suffix of date/time string of header.
	 *
	 * @endif
	 */
	std::string getSuffix()
	{
	  return m_Suffix;
	}

	/*
	void setCallBack(void (*call_back)(std::string s))
	{
	  
	}
	*/

  protected:
	/*!
	 * @if jp
	 *
	 * @brief �Хåե�Ʊ��
	 *
	 * �Хåե����ե�å��夵�줿�ݤ˸ƤФ��Ʊ���ؿ���
	 *
	 * @else
	 *
	 * @brief synchronize buffer
	 *
	 * @endif
	 */
	virtual int sync()
	{
	  int ret(0); 
	  if (m_pLogbuf != NULL &&
		  (this->pptr() - this->pbase()) > 0)
		{
		  {
			ACE_Guard<ACE_Thread_Mutex> guard(m_Mutex);
			*(this->pptr()) = '\0';
			std::string::basic_string<_CharT> tmp(this->pbase(),
												  this->pptr() - this->pbase());
			
			std::string s = getFmtDate();
			s += ( s.size() > 0 ? " " : "" ) + getSuffix();
			s += ( getSuffix().size() > 0 ? " " : "" ) + tmp;
		  //			s += (getSuffix().size() > 0 ? " " : "" ) + getSuffix();
		  //			s += (s.size() > 0 ? " " : "") + tmp;
			m_pLogbuf->sputn(s.c_str(), s.size());
			m_pLogbuf->pubsync();
			// Reset pptr() pointer to pbase()
			// __streambuf_type::sync() resets the pointer
			ret = __streambuf_type::sync();
			pbump( this->pbase() - this->pptr() );
		  }
		}
	  return ret;
	}

  private:
	__filebuf_type* m_pLogbuf;
	char m_Data[LINE_MAX];
	std::string m_DateFmt;
	std::string m_Suffix;
	ACE_Thread_Mutex m_Mutex;
  };


  /*!
   * @if jp
   *
   * @class basic_dummybuf
   *
   * @brief ���ߡ��Хåե����饹
   *
   * ���⤷�ʤ����ߡ��Хåե���
   *
   * @else
   *
   * @class basic_dammybuf
   *
   * @brief Dummy buffer class
   *
   *
   * @endif
   */
  template <typename _CharT, typename _Traits=std::char_traits<_CharT> >
  class basic_dummybuf
	: public std::basic_streambuf<_CharT, _Traits>
  {
  public:
	// Types:
	typedef _CharT                                    std::char_type;
	typedef _Traits                                   std::traits_type;
	typedef typename traits_type::int_type       int_type;
	typedef typename traits_type::pos_type       pos_type;
	typedef typename traits_type::off_type       off_type;
	typedef std::basic_streambuf<char_type, traits_type> __streambuf_type;
	typedef std::basic_filebuf<char_type, traits_type>   __filebuf_type;
	
	basic_dummybuf()
	{
	  char *pStart = m_Data;
	  char *pEnd = m_Data + (LINE_MAX - 1);
	  this->setp(pStart, pEnd);
	  this->setg(pStart, pStart, pEnd);
	}

	
	int_type overflow(int_type c = _Traits::eof() )
	{
	  pbump( this->pbase() - this->pptr() );
	  return _Traits::not_eof(c);
	}
	
	virtual int sync()
	{
	  pbump( this->pbase() - this->pptr() );
	  return 0;
	}
	
  private:
	char m_Data[255];
  };


  /*!
   * @if jp
   *
   * @class basic_logstream
   *
   * @brief �������ե����ޥåȥ��饹
   *
   * basic_ostream �饤���ʥ����ե����ޥåȥ��饹��
   *
   * @else
   *
   * @class basic_logstream
   *
   * @brief Logger format class
   *
   *
   * @endif
   */
  template <typename _CharT, typename _Traits=std::char_traits<_CharT> >
  class basic_logstream
	: public std::basic_ostream<_CharT, _Traits>
  {
  public:
	// Loglevel
	enum
	  {          // No: Write out messages include the following.
		SILENT,  // 0: ()
		ERROR,   // 1: (ERROR)
		WARN,    // 2: (ERROR, WARN)
		INFO,    // 3: (ERROR, WARN, INFO)
		NORMAL,  // 4: (ERROR, WARN, INFO, NORMAL)
		DEBUG,   // 5: (ERROR, WARN, INFO, NORMAL, DEBUG)
		TRACE,   // 6: (ERROR, WARN, INFO, NORMAL, DEBUG, TRACE)
		VERBOSE, // 7: (ERROR, WARN, INFO, NORMAL, DEBUG, TRACE, VERBOSE)
		PARANOID,// 8: (ERROR, WARN, INFO, NORMAL, DEBUG, TRACE, VERBOSE, PARA)
		MANDATORY// This level is used for only LogLockLevel
	  };

	static int strToLogLevel(std::string lv)
	{
	  if (lv == "SILENT")
		return basic_logstream::SILENT;
	  else if (lv == "ERROR")
		  return basic_logstream::ERROR;
	  else if (lv == "WARN")
		  return basic_logstream::WARN;
	  else if (lv == "INFO")
		  return basic_logstream::INFO;
	  else if (lv == "NORNAL")
		  return basic_logstream::NORMAL;
	  else if (lv == "DEBUG")
		  return basic_logstream::DEBUG;
	  else if (lv == "TRACE")
		  return basic_logstream::TRACE;
	  else if (lv == "VERBOSE")
		  return basic_logstream::VERBOSE;
	  else if (lv == "PARANOID")
		  return basic_logstream::PARANOID;
	  else if (lv == "MANDATORY")
		  return basic_logstream::MANDATORY;
	  else
		return basic_logstream::NORMAL;
	}
	
	// Types:
	typedef _CharT                                    std::char_type;
	typedef _Traits                                   std::traits_type;
	typedef basic_logbuf<char_type, traits_type>         __logbuf_type;
	typedef basic_dummybuf<char_type, traits_type>       __dummybuf_type;
	typedef basic_logstream<char_type, traits_type>      __logstream_type;
	typedef std::basic_ostream<char_type, traits_type>   __ostream_type;
	typedef std::basic_streambuf<char_type, traits_type> __streambuf_type;

	/*!
	 * @if jp
	 *
	 * @brief basic_logstream ���饹���󥹥ȥ饯��
	 *
	 * �ǥե���ȥ��󥹥ȥ饯��
	 *
	 * @param streambuf basic_streambuf �����֥�������
	 *
	 * @else
	 *
	 * @brief basic_logbuf class constructor.
	 *
	 * @endif
	 */
	basic_logstream(__streambuf_type& streambuf)
	  : __ostream_type(&streambuf),
		m_DummyStream(new __dummybuf_type()),
		m_LogLevel(NORMAL), m_LogLock(false)
	{
	  this->init(&streambuf);
	}

	/*!
	 * @if jp
	 *
	 * @brief basic_logstream ���饹�ǥ��ȥ饯��
	 *
	 * �ǥ��ȥ饯����
	 *
	 * @else
	 *
	 * @brief basic_logstream class destractor.
	 *
	 * @endif
	 */
	~basic_logstream()
	{
	}
	
	/*!
	 * @if jp
	 *
	 * @brief �����ݻ����Ƥ���Хåե��ؤΥݥ��󥿤��֤���
	 *
	 * @else
	 *
	 * @brief return a pointer of a current buffer.
	 *
	 * @endif
	 */
	/*
	__logbuf_type*	rdbuf() const
	{
	  return const_cast<__logbuf_type*>(&m_Streambuf);
	}
	*/
	
	/*!
	 * @if jp
	 *
	 * @brief printf �ե����ޥåȽ���
	 *
	 * printf�饤���ʽ񼰤ǥ������Ϥ��롣
	 *
	 * @else
	 *
	 * @brief Formatted output like printf
	 *
	 * @endif
	 */
	static std::string printf(char const * __restrict fmt, ...)
	{
	  char str[LINE_MAX];
	  va_list ap;

	  va_start(ap, fmt);
	  int ret = vsnprintf(str, LINE_MAX - 1, fmt, ap);
	  va_end(ap);
	  std::string s(str);

	  return s;
	}

	
	void setLogLevel(int level)
	{
	  m_LogLevel = level;
	}

	void setLogLock(int lock)
	{
	  if      (lock == 1)
		{
		  m_LogLock = true;
		}
	  else if (lock == 0)
		{
		  m_LogLock = false;
		}
	}


	void enableLogLock()
	{
	  m_LogLock = true;
	}

	void disableLogLock()
	{
	  m_LogLock = false;
	}

	__ostream_type& level(int level)
	{
	  if (m_LogLevel >= level)
		{
		  return *this;
		}
	  else
		{
		  return m_DummyStream;
		}

	}

	inline void acquire()
	{
	  if (m_LogLock) m_Mutex.acquire();
	}

	inline void release()
	{
	  if (m_LogLock) m_Mutex.release();
	}

	__ostream_type m_DummyStream;

  private:
	int m_LogLevel;
	bool m_LogLock;
	ACE_Thread_Mutex m_Mutex;

  };
  typedef sync_callback<char> RtcSyncCallback;
  typedef basic_logbuf<char> RtcLogbuf;
  typedef basic_medlogbuf<char> RtcMedLogbuf;
  typedef basic_logstream<char> RtcLogStream;

#else //// NO_LOGGING  

  class RtcSyncCallback
  {
  public:
	RtcSyncCallback() {;};
	virtual int operator()(const char* s) {;};
  };

  class RtcLogbuf
  {
  public:
	RtcLogbuf() {;};
	RtcLogbuf(const char* s, int m) {;};
	void open(const char* s, int m) {;};
	void setSyncCallBack(RtcSyncCallback& cb) {;};
  };

  class RtcMedLogbuf
  {
  public:
	RtcMedLogbuf() {;};
	RtcMedLogbuf(RtcLogbuf& f) {;};
	void  setDateFmt(char* fmt) {;};
	void  setDateFmt(std::string fmt) {;};
	void setSuffix(char* suffix) {;};
	void setSuffix(std::string suffix) {;};
  };

  class RtcLogStream
	: public ostream
  {
  public:
	enum {SILENT, ERROR, WARN, INFO, NORMAL, DEBUG, TRACE, VERBOSE, PARANOID, MANDATORY };
	static int strToLogLevel(std::string lv){return NORMAL;}
	RtcLogStream(RtcMedLogbuf& buf) {;};
	void setLogLevel(int level) {;};
	void setLogLock(int lock) {;};
	RtcLogStream& level(int level) {return *this;};
	/*
	RtcLogStream& operator<<(const char* s) {return *this;};
	RtcLogStream& operator<<(const string s) {return *this;};
	RtcLogStream& operator<<(const ostream& o) {return *this;};
	RtcLogStream& operator<<(const ostream o) {return *this;};
	*/
  };
  
#endif // NO_LOGGING  
  

  // __VA_ARGS__ cannot be used in VC	
#if 0
#define RTC_LOG(LV, fmt, ...)										\
  rtcout.level(LV) << rtcout.printf(fmt, __VA_ARGS__) << std::endl;
#define RTC_ERROR(fmt, ...)    RTC_LOG(RtcLogStream::ERROR, fmt, __VA_ARGS__)
#define RTC_WARN(fmt, ...)     RTC_LOG(RtcLogStream::WARN, fmt, __VA_ARGS__)
#define RTC_NORMAL(fmt, ...)   RTC_LOG(RtcLogStream::NORMAL, fmt, __VA_ARGS__)
#define RTC_INFO(fmt, ...)     RTC_LOG(RtcLogStream::INFO, fmt, __VA_ARGS__)
#define RTC_DEBUG(fmt, ...)    RTC_LOG(RtcLogStream::DEBUG, fmt, __VA_ARGS__)
#define RTC_TRACE(fmt, ...)    RTC_LOG(RtcLogStream::TRACE, fmt, __VA_ARGS__)
#define RTC_VERBOSE(fmt, ...)  RTC_LOG(RtcLogStream::VERBOSE, fmt, __VA_ARGS__)
#define RTC_PARANOID(fmt, ...) RTC_LOG(RtcLogStream::PARANOID, fmt, __VA_ARGS__)
#endif

#ifndef NO_LOGGING

  /*!
   * @if jp
   *
   * @brief ���ѥ������ϥޥ���
   *
   * ������٥뤪��ӽ��ϥե����ޥå�ʸ���������Ȥ��ƤȤ롣
   *
   * @else
   *
   * @brief Log output macro
   *
   * @endif
   */
#define RTC_LOG(LV, fmt)												\
  rtcout.acquire();														\
	rtcout.level(LV) << rtcout.printf fmt << std::endl;					\
	rtcout.release()

  /*!
   * @if jp
   *
   * @brief ���顼�������ϥޥ�����
   *
   * ���顼��٥�Υ������ϥޥ�����������٥뤬
   * ERROR, WARN, INFO, NORMAL, DEBUG, TRACE, VERBOSE, PARANOID
   * �ξ��˥������Ϥ���롣
   *
   * @else
   *
   * @brief Error log output macro.
   *
   * @endif
   */
#define RTC_ERROR(fmt)													\
  rtcout.acquire();														\
	rtcout.level(RtcLogStream::ERROR)    << rtcout.printf fmt << std::endl;	\
	rtcout.release()

  /*!
   * @if jp
   *
   * @brief ��˥󥰥������ϥޥ�����
   *
   * �������˥󥰥�٥�Υ������ϥޥ�����������٥뤬
   * ( WARN, INFO, NORMAL, DEBUG, TRACE, VERBOSE, PARANOID )
   * �ξ��˥������Ϥ���롣
   *
   * @else
   *
   * @brief Warning log output macro.
   *
   * If logging levels are
   * ( WARN, INFO, NORMAL, DEBUG, TRACE, VERBOSE, PARANOID ),
   * message will be output to log.
   *
   * @endif
   */
#define RTC_WARN(fmt)													\
  rtcout.acquire();														\
	rtcout.level(RtcLogStream::WARN)     << rtcout.printf fmt << std::endl;	\
	rtcout.release()

  /*!
   * @if jp
   *
   * @brief ����ե��������ϥޥ�����
   *
   * ����ե���٥�Υ������ϥޥ�����������٥뤬
   * ( INFO, NORMAL, DEBUG, TRACE, VERBOSE, PARANOID )
   * �ξ��˥������Ϥ���롣
   *
   * @else
   *
   * @brief Infomation level log output macro.
   *
   *  If logging levels are
   * ( INFO, NORMAL, DEBUG, TRACE, VERBOSE, PARANOID ),
   * message will be output to log.
   *
   * @endif
   */
#define RTC_INFO(fmt)													\
  rtcout.acquire();														\
	rtcout.level(RtcLogStream::INFO)     << rtcout.printf fmt << std::endl;	\
	rtcout.release()

  /*!
   * @if jp
   *
   * @brief �Ρ��ޥ�������ϥޥ�����
   *
   * �Ρ��ޥ��٥�Υ������ϥޥ�����������٥뤬
   * ( NORMAL, DEBUG, TRACE, VERBOSE, PARANOID )
   * �ξ��˥������Ϥ���롣
   *
   * @else
   *
   * @brief Normal level log output macro.
   *
   * If logging levels are
   * ( NORMAL, DEBUG, TRACE, VERBOSE, PARANOID ),
   * message will be output to log.
   *
   * @endif
   */
#define RTC_NORMAL(fmt)													\
  rtcout.acquire();														\
	rtcout.level(RtcLogStream::NORMAL)   << rtcout.printf fmt << std::endl;	\
	rtcout.release()

  /*!
   * @if jp
   *
   * @brief �ǥХå��������ϥޥ�����
   *
   * �ǥХå���٥�Υ������ϥޥ�����������٥뤬
   * ( DEBUG, TRACE, VERBOSE, PARANOID )
   * �ξ��˥������Ϥ���롣
   *
   * @else
   *
   * @brief Debug level log output macro.
   *
   * If logging levels are
   * ( DEBUG, TRACE, VERBOSE, PARANOID ),
   * message will be output to log.
   *
   * @endif
   */
#define RTC_DEBUG(fmt)													\
  rtcout.acquire();														\
	rtcout.level(RtcLogStream::DEBUG)    << rtcout.printf fmt << std::endl;	\
	rtcout.release()

  /*!
   * @if jp
   *
   * @brief �ȥ졼���������ϥޥ�����
   *
   * �ȥ졼����٥�Υ������ϥޥ�����������٥뤬
   * ( TRACE, VERBOSE, PARANOID )
   * �ξ��˥������Ϥ���롣
   *
   * @else
   *
   * @brief Trace level log output macro.
   *
   * If logging levels are
   * ( TRACE, VERBOSE, PARANOID ),
   * message will be output to log.
   *
   * @endif
   */
#define RTC_TRACE(fmt)													\
  rtcout.acquire();														\
	rtcout.level(RtcLogStream::TRACE)    << rtcout.printf fmt << std::endl;	\
	rtcout.release()

  /*!
   * @if jp
   *
   * @brief �٥�ܡ����������ϥޥ�����
   *
   * �٥�ܡ�����٥�Υ������ϥޥ�����������٥뤬
   * ( VERBOSE, PARANOID )
   * �ξ��˥������Ϥ���롣
   *
   * @else
   *
   * @brief Verbose level log output macro.
   *
   * If logging levels are
   * ( VERBOSE, PARANOID ),
   * message will be output to log.
   *
   * @endif
   */
#define RTC_VERBOSE(fmt)												\
  rtcout.acquire();														\
	rtcout.level(RtcLogStream::VERBOSE)  << rtcout.printf fmt << std::endl;	\
	rtcout.release()

  /*!
   * @if jp
   *
   * @brief �ѥ�Υ��ɥ������ϥޥ�����
   *
   * �ѥ�Υ��ɥ�٥�Υ������ϥޥ�����������٥뤬
   * ( PARANOID )
   * �ξ��˥������Ϥ���롣
   *
   * @else
   *
   * @brief Paranoid level log output macro.
   *
   * If logging levels are
   * ( PARANOID ),
   * message will be output to log.
   *
   * @endif
   */
#define RTC_PARANOID(fmt)												\
  rtcout.acquire();														\
	rtcout.level(RtcLogStream::PARANOID) << rtcout.printf fmt << std::endl;	\
	rtcout.release()

#else
#define RTC_ERROR(fmt)
#define RTC_WARN(fmt)
#define RTC_NORMAL(fmt)
#define RTC_INFO(fmt)
#define RTC_DEBUG(fmt)
#define RTC_TRACE(fmt)
#define RTC_VERBOSE(fmt)
#define RTC_PARANOID(fmt)
#endif

}; // namespace RTM

#endif  // RtcSystemLogger_h