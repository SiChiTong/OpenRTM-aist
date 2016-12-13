// -*- C++ -*-
/*!
 * @file CORBA_Util.h
 * @brief CORBA utility template classes
 * @date $Date: 2007-04-26 15:24:41 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2006
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id: CORBA_SeqEx.h,v 1.3 2007-04-26 15:24:41 n-ando Exp $
 *
 */

/*
 * $Log: not supported by cvs2svn $
 * Revision 1.2  2006/10/30 08:15:21  n-ando
 * CVS's information tag was revised.
 *
 * Revision 1.1  2006/10/27 09:07:00  n-ando
 * Util.h was moved to CORBA_SeqEx.h.
 *
 * Revision 1.3  2006/10/17 19:07:58  n-ando
 * Helper template function SeqUtil::for_each() for CORBA sequence was added.
 *
 * Revision 1.2  2006/10/17 10:12:18  n-ando
 * SequenceEx's copy constructor and assignment operator were implemented.
 *
 * Revision 1.1  2006/09/11 18:14:53  n-ando
 * The first commit.
 *
 *
 */

#ifndef CORBA_Util_h
#define CORBA_Util_h

#include <rtm/RTC.h>
#include <ace/Guard_T.h>
#include <ace/Thread_Mutex.h>
#include <ace/Recursive_Thread_Mutex.h>


typedef ACE_Guard<ACE_Thread_Mutex> Guard;
typedef ACE_Read_Guard<ACE_Thread_Mutex> Read_Guard;
typedef ACE_Write_Guard<ACE_Thread_Mutex> Write_Guard;

typedef ACE_Guard<ACE_Recursive_Thread_Mutex> Guard_r;
typedef ACE_Read_Guard<ACE_Recursive_Thread_Mutex> Read_Guard_r;
typedef ACE_Write_Guard<ACE_Recursive_Thread_Mutex> Write_Guard_r;


/*!
 * CORBA sequence �إ�ѡ��ƥ�ץ졼�ȴؿ�
 *
 * for_each ()
 * find ()
 * push_back ()
 * insert ()
 * front()
 * back ()
 * erase ()
 * clear ()
 *
 */
namespace CORBA_Sequence_Util
{
  /*!
   * @if jp
   * @brief CORBA sequence ���Ф��� functor ��Ŭ�Ѥ���
   *
   * CORBA sequence ���Ƥ����Ǥ��Ф��ơ�Ϳ����줿 functor ��Ŭ�Ѥ��롣
   * functor �� void functor(CORBA sequence ������) �η�����Ȥ�ɬ�פ����롣
   *
   * @return ���Ƥ����Ǥ�������� Functor
   * @param seq Functor ��Ŭ�Ѥ��� CORBA sequence
   * @param functor CORBA sequence �����Ǥ�������� Functor
   *
   * @else
   *
   * @brief Apply the functor to all CORBA sequence elements
   *
   * Apply the given functor to the given CORBA sequence.
   * functor should be void functor(CORBA sequence element).
   *
   * @return Functor that processed all CORBA sequence elements
   * @param seq CORBA sequence to be applied the functor
   * @param functor A functor to process CORBA sequence elements
   *
   * @endif
   */
  template <class CorbaSequence, class Functor>
  Functor for_each(const CorbaSequence& seq, Functor f)
  {
    CORBA::ULong len;
    len = seq.length();
    for (CORBA::ULong i = 0; i < len; ++i)
      {
	f(seq[i]);
      }
    return f;
  }


  /*!
   * @if jp
   * @brief CORBA sequence ���椫�� functor ��Ŭ�礹�����ǤΥ���ǥå������֤�
   *
   * CORBA sequence ���Ƥ����Ǥ��Ф��ơ�Ϳ����줿 functor ��Ŭ�Ѥ���
   * functor �� true ���֤��褦���Υ���ǥå������֤���
   * functor �� bool functor(const CORBA sequence ������) �η�����Ȥꡢ
   * Ŭ�礹�����Ǥ��Ф��� true ���֤�ɬ�פ����롣
   *
   * @return Functor ��Ŭ�礹�����ǤΥ���ǥå�����
   *         ���Ĥ���ʤ��Ȥ��� -1 ���֤���
   * @param seq Functor ��Ŭ�Ѥ��� CORBA sequence
   * @param functor CORBA sequence �������Ǥ򸫤Ĥ��� Functor
   *
   * @else
   *
   * @brief Return the index of CORBA sequence element that functor matches 
   *
   * This operation applies the given functor to the given CORBA sequence,
   * and returns the index of the sequence element that the functor matches.
   * The functor should be bool functor(const CORBA sequence element) type,
   * and it would return true, if the element matched the functor.
   *
   * @return The index of the element that functor matches.
   *         If no element found, it would return -1.
   * @param seq CORBA sequence to be applied the functor
   * @param functor A functor to process CORBA sequence elements
   *
   * @endif
   */
  template <class CorbaSequence, class Functor>
  CORBA::Long find(const CorbaSequence& seq, Functor f)
  {
    CORBA::ULong len(seq.length());
    for (CORBA::ULong i = 0; i < len; ++i)
      {
	if (f(seq[i])) return (CORBA::Long)i;
      }
    return -1;
  }

  /*!
   * @if jp
   * @brief CORBA sequence �κǸ�����Ǥ��ɲä���
   *
   * CORBA sequence �κǸ��Ϳ����줿���Ǥ��ɲä��롣
   * CORBA sequence ��Ĺ���ϼ�ưŪ�˳�ĥ����롣
   *
   * @param seq ���Ǥ��ɲä��� CORBA sequence
   * @param elem �ɲä�������
   *
   * @else
   *
   * @brief Push the new element back to the CORBA sequence
   *
   * Add the given element to the last of CORBA sequence.
   * The length of the CORBA sequence will be expanded automatically.
   *
   * @param seq CORBA sequence to be added a new element
   * @param elem The new element to be added to the CORBA sequence
   *
   * @endif
   */
  template <class CorbaSequence, class SequenceElement>
  void push_back(CorbaSequecne& seq, SequenceElement elem)
  {
    CORBA::ULong len(seq.length());
    seq.length(len + 1);
    seq[len] = item;
  }


  /*!
   * @if jp
   * @brief CORBA sequence �����Ǥ���������
   *
   * CORBA sequence �� index �ΰ��֤����Ǥ�ä��롣
   * index �� Ϳ����줿��CORBA sequence �κ���� index ����礭�����
   * �Ǹ�����ǤȤ��Ʋä����롣
   * CORBA sequence ��Ĺ���ϼ�ưŪ�˳�ĥ����롣
   *
   * @param seq ���Ǥ��ɲä��� CORBA sequence
   * @param elem �ɲä�������
   * @param index ���Ǥ��ɲä������
   *
   * @else
   *
   * @brief Insert the element to the CORBA sequence
   *
   * Insert a new element in the given position to the CORBA sequence.
   * If the given index is greater than the length of the sequence,
   * the given element is pushed back to the last of the sequence.
   * The length of the CORBA sequence will be expanded automatically.
   *
   * @param seq The CORBA sequence to be inserted a new element
   * @param elem The new element to be inserted the sequence
   * @param index The inserting position
   *
   * @endif
   */
  template <class CorbaSequence, class SequenceElement>
  void insert(CorbaSequence& seq, SequenceElement& elem, CORBA::ULong index)
  {
    CORBA::ULong len(seq.length());
    if (index > len) push_back(seq, elem);

    seq.length(len + 1);
    for (CORBA::ULong i = len; i >= index; --i)
      {
	seq[i + 1] = seq[i];
      }
    seq[index] = elem;
  }


  /*!
   * @if jp
   * @brief CORBA sequence ����Ƭ���Ǥ��������
   *
   * seq[0] ��Ʊ����
   *
   * @param seq ���Ǥ�������� CORBA sequence
   *
   * @else
   *
   * @brief Get the front element of the CORBA sequence
   *
   * This operation returns seq[0].
   *
   * @param seq The CORBA sequence to be get the element
   *
   * @endif
   */
  template <class CorbaSequence, class SequenceElement>
  SequenceElement& front(CorbaSequence& seq)
  {
    return seq[0];
  }


  /*!
   * @if jp
   * @brief CORBA sequence ���������Ǥ��������
   *
   * seq[seq.length() - 1] ��Ʊ����
   *
   * @param seq ���Ǥ�������� CORBA sequence
   *
   * @else
   *
   * @brief Get the last element of the CORBA sequence
   *
   * This operation returns seq[seq.length() - 1].
   *
   * @param seq The CORBA sequence to be get the element
   *
   * @endif
   */	
  template <class CorbaSequence, class SequenceElement>
  SequenceElement& back(CorbaSequence& seq)
  {
    return seq[seq.length() - 1];
  }


  /*!
   * @if jp
   * @brief CORBA sequence �λ��ꤵ�줿���֤����Ǥ�������
   *
   * ���ꤵ�줿����ǥå��������Ǥ������롣
   * ������줿���Ǥϵͤ��졢sequence ��Ĺ����1���롣
   *
   * @param seq ���Ǥ������� CORBA sequence
   * @param index ����������ǤΥ���ǥå���
   *
   * @else
   *
   * @brief Erase the element of the specified index
   *
   * This operation removes the element of the given index.
   * The other elements are closed up around the hole.
   *
   * @param seq The CORBA sequence to be get the element
   * @param index The index of the element to be removed
   *
   * @endif
   */
  template <class CorbaSequence>
  void erase(CorbaSequence& seq, CORBA::ULong index)
  {
    CORBA::ULong len(seq.length());
    if (index > len) return;

    for (CORBA::ULong i = index; i < len - 1; ++i)
      {
	seq[i] = seq[i + 1];
      }
    seq.length(len - 1);
  }

  /*!
   * @if jp
   * @brief CORBA sequence �������Ǥ���
   *   
   * seq.length(0) ��Ʊ����
   *
   * @else
   *
   * @brief Erase all the elements of the CORBA sequence
   *
   * same as seq.length(0).
   *
   * @endif
   */
  template <class CorbaSequence>
  void clear(CorbaSequence& seq)
  {
    seq.length(0);
  }
  // End of CORBA sequence helper functions



  //============================================================
  // CORBA sequence extention class
  //============================================================

  template <class T>
  struct LockedStruct
  {
    ACE_Thread_Mutex lock;
    T                data;
  };
  
  /*!
   * @if jp
   * @class SequenceEx
   * @brief CORBA::sequence ��ĥ���饹
   *
   * ���Υ��饹�� CORBA �� sequence �����ĥ�� std::vector �Υ��󥿡��ե�������
   * �󶡤��� (�㤨�� size(), max_size(), empty(), resize(), insert(),
   * erase(), erase_if(), push_back(), pop_back(), find()).
   * CORBA �� sequence ����Ѿ����Ƥ��뤿�ᡢCORBA �� sequence ����
   * ���ڥ졼�����(like operator=(), maximum(), length(), operator[])��
   * ���Ѳ�ǽ�Ǥ��롣
   * .
   * @else
   *
   * @brief CORBA::sequence extention class
   *
   * This class extends CORBA sequence type, and provides std::vector like
   * interfaces (like size(), max_size(), empty(), resize(), insert(),
   * erase(), erase_if(), push_back(), pop_back(), find()).
   * Since this class inherits CORBA sequence class, user can also use CORBA
   * sequence interfaces (like operator=(), maximum(), length(), operator[]).
   *
   * @endif
   */ 
  template <class CorbaSequence, class SequenceItem, class Mutex>
  class SequenceEx
    : public CorbaSequence
  {
  public:
    /*!
     * @if jp
     *
     * @brief �ǥե���ȥ��󥹥ȥ饯��
     *
     * �ǥե���ȥ��󥹥ȥ饯�����ǥե���ȥ��󥹥ȥ饯���Ǥϡ�
     * ���쥯�饹��CORBA::sequence ��Ĺ���� 0 �˥��åȤ��롣
     * 
     * @param CorbaSequence CORBA IDL ���������� sequence ��
     * @param SeuenceItem CorbaSequence �����Ƿ�
     * @param Mutex Mutex�η�
     *
     * @else
     *
     * @brief Default constructor
     *
     * Default constructor. This default constructor sets the sequence length
     * of CorbaSequence base class.
     *
     * @param CorbaSequence Sequence type defined in CORBA IDL
     * @param SeuenceItem The item type of CorbaSequence
     * @param Mutex The type of mutex
     *
     * @endif
     */
    SequenceEx(): CorbaSequence(0) {};
    
    
    /*!
     * @if jp
     *
     * @brief CorbaSequence ����Υ��ԡ����󥹥ȥ饯��
     *
     * CorbaSequence������Υ��ԡ����󥹥ȥ饯����
     * Ϳ����줿 CorbaSequence �����Ƥ򥳥ԡ����롣
     *
     * @param _sq CorbaSequence ���Υ��ԡ���
     *
     * @else
     *
     * @brief Copy constructor from CorbaSequence
     *
     * This constructor copies sequence contents from given CorbaSequence
     * to this object.
     *
     * @param _sq Copy source of CorbaSequence type
     *
     * @endif
     */
    SequenceEx(const CorbaSequence& _sq)
      : CorbaSequence(0)
    {
      this->length(_sq.length());
      CORBA::ULong len(this->length());
      for (CORBA::ULong i = 0; i < len; ++i)
	(*this)[i] = _sq[i];
    };
    
    
    /*!
     * @if jp
     *
     * @brief ���ԡ����󥹥ȥ饯��
     *
     * @param _sq ���ԡ���
     *
     * @else
     *
     * @brief Copy constructor
     *
     * @param _sq Copy source.
     *
     * @endif
     */
    SequenceEx(const SequenceEx& _sq)
      : CorbaSequence(0)
    {
      this->length(_sq.length());
      CORBA::ULong len(this->length());
      for (CORBA::ULong i = 0; i < len; ++i)
	(*this)[i] = _sq[i];
    };
    
    
    /*!
     * @if jp
     *
     * @brief �����黻��
     *
     * @param _sq ���ԡ���
     *
     * @else
     *
     * @brief Assignment operator
     *
     * @param _sq Copy source.
     *
     * @endif
     */
    SequenceEx& operator=(const SequenceEx& _sq)
    {
      this->length(_sq.length());
      CORBA::ULong len(this->length());
      for (CORBA::ULong i = 0; i < len; ++i)
	(*this)[i] = _sq[i];
      return *this;
    };
    
    
    /*!
     * @if jp
     *
     * @brief CorbaSequence ����������黻��
     *
     * @param _sq ���ԡ���
     *
     * @else
     *
     * @brief Assignment operator from CorbaSequence
     *
     * @param _sq Copy source.
     *
     * @endif
     */
    SequenceEx& operator=(const CorbaSequence& _sq)
    {
      this->length(_sq.length());
      CORBA::ULong len(this->length());
      for (CORBA::ULong i = 0; i < len; ++i)
	(*this)[i] = _sq[i];
      return *this;
    };
    
    
    /*!
     * @if jp
     *
     * @brief �ǥ��ȥ饯��
     *
     * @else
     *
     * @brief Destructor
     *
     * @endif
     */
    virtual ~SequenceEx()
    {
      this->length(0);
    };
    
    
    /*!
     * @if jp
     *
     * @brief ���������������
     *
     * ���Υ��ڥ졼�����ϥ������󥹤Υ��������֤���
     * CorbaSequence::length() ��Ʊ����
     *
     * @return �������󥹤Υ�����
     *
     * @else
     *
     * @brief Get size of this sequence
     *
     * This operation returns the size of the sequence.
     * This is same as CorbaSequence::length().
     *
     * @return The size of the sequence.
     *
     * @endif
     */
    inline CORBA::ULong size() { return this->length(); }
    
    
    /*!
     * @if jp
     *
     * @brief ��Ǽ��ǽ�ʺ���Υ��������������
     *
     * ���Υ��ڥ졼�����ϥ������󥹤θ��ߤγ�Ǽ��ǽ�ʺ���Υ��������֤���
     * CorbaSequence::maximum() ��Ʊ����
     *
     * @return �������󥹤˳�Ǽ��ǽ�ʺ���Υ�����
     *
     * @else
     *
     * @brief Get current maximum size of this sequence
     *
     * This operation returns the current maximum size of the sequence.
     * This is same as CorbaSequence::maximum().
     *
     * @return The maximum size of the sequence.
     *
     * @endif
     */
    inline CORBA::ULong max_size() { return this->maximum(); }
    
    
    /*!
     * @if jp
     *
     * @brief �������󥹤������ɤ���Ĵ�٤�
     *
     * ���Υ��ڥ졼�����ϥ������󥹤������ɤ����� bool �ͤ��֤���
     * �������� 0 �ʤ� true�������Ǥʤ���� false ���֤���
     *
     * @return �������󥹤������ɤ����� bool ��
     *
     * @else
     *
     * @brief Test whether the sequence is empty
     *
     * This operation returns bool value whether the sequence is empty.
     * If the size of the sequence is 0, this operation returns true,
     * and in other case this operation returns false.
     *
     * @return The bool value whether the sequence is empty.
     *
     * @endif
     */
    inline bool empty() { return (this->length() == 0) ? true : false; }
    
    
    /*!
     * @if jp
     *
     * @brief �������󥹤�ꥵ��������
     *
     * ���Υ��ڥ졼�����ϥ������󥹤�Ĺ�����ѹ����롣
     * ���ߤ�Ĺ������礭�ʥ�������Ϳ����줿��硢���� x �ǡ�
     * �����˥��������Ȥ��줿��ʬ�������롣
     * ���ߤ�Ĺ����꾮������������Ϳ����줿��硢CorabSequence ��Ʊ�ͤ�
     * ;ʬ�ʥ������󥹤����ǤϺ������롣
     *
     * @param new_size �������������󥹤Υ�����
     * @param item��Ĺ���ʤä�ʬ�Υ������󥹤���������
     *
     * @else
     *
     * @brief Resize the length of the sequence
     *
     * This operation resizes the length of the sequence.
     * If longer length than current sequence length is given,
     * newly allocated rooms will be assigned by element given by the argument.
     * If shorter length than current sequence length is given,
     * the excessive element of a sequence is deleted like behavior of
     * CorabSequence
     *
     * @param new_size The new size of the sequence
     * @param item��   Sequence element to be assigned to new rooms.
     *
     * @endif
     */
    void resize(CORBA::ULong new_size, SequenceItem& item)
    {
      ACE_Write_Guard<Mutex> gaurd(lock);
      CORBA::ULong len(this->length);
      if (new_size > len) // Widen sequence
	{
	  this->length(new_size);
	  for (CORBA::ULong i = len; i < new_size; ++i)
	    (*this)[i] = item;
	}
      else if (new_size < len) // shorten sequence
	{
	  this->length(new_size);
	}
    }
    
    
    /*!
     * @if jp
     *
     * @brief �������󥹤����Ǥ���������
     *
     * ���Υ��ڥ졼�����ϥ������󥹤���������Ǥ��������롣
     *
     * @param position ���������Ǥ�����������
     * @param item���������륷�����󥹤�����
     *
     * @else
     *
     * @brief Insert a new item to the sequence
     *
     * This operation inserts a new item to the sequence.
     *
     * @param position The position of new inserted item.
     * @param item��   Sequence element to be inserted.
     *
     * @endif
     */
    void insert(CORBA::ULong position, const SequenceItem& item)
    {
      ACE_Write_Guard<Mutex> gaurd(lock);
      CORBA::ULong len(this->length());
      // allowed position: [0, len]
      if (position > len) throw; // InvalidParameter("Invalid index");
      
      // widen sequence
      this->length(len + 1);
      // shift items
      for (CORBA::ULong i = (len - 1); i > position; --i)
	{
	  (*this)[i] = (*this)[i-1];
	}
      (*this)[position] = item;
    }
    
    
    /*!
     * @if jp
     *
     * @brief �������󥹤����Ǥ�������
     *
     * ���Υ��ڥ졼�����ϥ������󥹤����Ǥ�������
     *
     * @param position ������륷���������Ǥξ��
     *
     * @else
     *
     * @brief Erase an item of the sequence
     *
     * This operation erases an item from the sequence.
     *
     * @param position The position of erased item.
     *
     * @endif
     */
    SequenceItem erase(CORBA::ULong position)
    {
      ACE_Write_Guard<Mutex> gaurd(lock);
      CORBA::ULong len(this->length());
      // allowed position: [0, len)
      if (position > (len - 1)) throw; // InvalidParameter("Invalid index");
      
      // keep erased item
      SequenceItem erased((*this)[position]);
      // shift remaining items
      for (CORBA::ULong i = position; i < (len - 1); ++i)
	{
	  (*this)[i] = (*this)[i+1];
	}
      // shorten sequence
      this->length(len - 1);
      return erased;
    }
    
    /*!
     * @if jp
     *
     * @brief �������󥹤����Ǥ�Ҹ�Τ������äƺ������
     *
     * ���Υ��ڥ졼�����ϽҸ�Ȥ���Ϳ����줿�ؿ����֥������Ȥ�
     * ��郎���ΤȤ������Υ������󥹤����Ǥ������롣
     *
     * @param f ������륷�����󥹤���ꤹ��Ѹ�
     *
     * @else
     *
     * @brief Erase an item according to the given predicate
     *
     * This operation erases an item according to the given predicate.
     *
     * @param f The predicate functor to decide deletion.
     *
     * @endif
     */
    template <class Predicate>
    SequenceItem erase_if(Predicate f)
    {
      ACE_Write_Guard<Mutex> gaurd(lock);
      CORBA::ULong len(this->length());
      for (CORBA::ULong i = 0; i < len; ++i)
	if (f((*this)[i]))
	  return erase(i);
      throw;
    }
    
    
    /*!
     * @if jp
     *
     * @brief ���Ǥ�Ǹ������ɲä���
     *
     * ���Υ��ڥ졼������Ϳ����줿���Ǥ򥷡����󥹤κǸ���ɲä��롣
     *
     * @param item �ɲä��뤹�륪�֥�������
     *
     * @else
     *
     * @brief Append an item to the end of the sequence.
     *
     * This operation push back an item to the of the sequence.
     *
     * @param item The object to be added to the end of the sequnce.
     *
     * @endif
     */ 
    void push_back(const SequenceItem& item)
    {
      ACE_Write_Guard<Mutex> gaurd(lock);
      CORBA::ULong len(this->length());
      this->length(len + 1);
      (*this)[len] = item;
    }
    
    void pop_back()
    {
      ACE_Write_Guard<Mutex> gaurd(lock);
      CORBA::ULong len(this->length());
      this->len(len - 1);
    }
    
    template <class F>
    SequenceItem find(F f) const
    {
      ACE_Read_Guard<Mutex> guard(lock);
      CORBA::ULong len(this->length());
      for (CORBA::ULong i = 0; i < len; ++i)
	if (f((*this)[i]))
	  return (*this)[i];
      throw; // InvalidParameter("Not found.");
    }
    
    mutable Mutex lock;
  }; // class SequenceEx
  
  
}; // namespace CORBA_Sequence_Util

#endif // CORBA_Util_h