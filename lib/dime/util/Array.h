/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#ifndef DIME_ARRAY_H
#define DIME_ARRAY_H

#include <stdlib.h>

#include <dime/Basic.h>

// FIXME: the #pragmas below is just a quick hack to avoid heaps of
// irritating warning messages from the compiler for client code
// compiled under MSVC++. Should try to find the real reason for the
// warnings and fix the cause of the problem instead. 20020730 mortene.
//
// UPDATE 20030617 mortene: there is a Microsoft Knowledge Base
// article at <URL:http://support.microsoft.com> which is related to
// this problem. It's article number KB168958.
//
// In short, the general solution is that classes that exposes usage
// of dimeArray<type> needs to declare the specific template instance
// with "extern" and __declspec(dllimport/export).
//
// That is a lot of work to change, tho'. Another possibility which
// might be better is to simply avoid using (exposing) dimeArray from
// any of the other public classes. Judging from a quick look, this
// seems feasible, and just a couple of hours or so of work.
//
#ifdef _MSC_VER // Microsoft Visual C++
#pragma warning(disable:4251)
#pragma warning(disable:4275)
#endif // _MSC_VER


template <class T>
class dimeArray
{
public:
  dimeArray(const int initsize = 4);
  ~dimeArray();

  void append(const T &value);
  void append(const dimeArray<T> &array);
  void prepend(const dimeArray<T> &array);
  void insertElem(const int idx, const T &value);
  void setElem(const int index, const T &value);
  T getElem(const int index) const;
  void getElem(const int index, T &elem) const;
  T    getLastElem() const;
  void getLastElem(T &elem) const;
  T &operator [](const int index);
  T operator [](const int index) const;
  void removeElem(const int index);
  void removeElemFast(const int index);
  void reverse();
  void setCount(const int count);
  void makeEmpty(const int initsize = 4);
  void freeMemory();
  int  count() const;
  int  allocSize() const;
  T   *arrayPointer();
  const T *constArrayPointer() const;
  void shrinkToFit();

private:
  void growArray();
  T *array;
  int num;
  int size;

}; // class dimeArray<>

template <class T> inline 
dimeArray<T>::dimeArray(const int size)
{
  this->array = new T[size];
  this->size = size;
  this->num = 0;
}

template <class T> inline 
dimeArray<T>::~dimeArray()
{
  delete [] this->array;
}

template <class T> inline void 
dimeArray<T>::growArray()
{
  int oldsize = this->size;
  T *oldarray = this->array;
  this->size <<= 1;
  this->array = new T[this->size];
  for (int i = 0; i < oldsize; i++) this->array[i] = oldarray[i];
  delete [] oldarray;
}

template <class T> inline void 
dimeArray<T>::append(const T &elem)
{
  if (this->num >= this->size) growArray();
  this->array[this->num++] = elem;
}


template <class T> inline void 
dimeArray<T>::append(const dimeArray<T> &array)
{
  while (this->size <= this->num+array.count()) growArray();
  for (int i=0;i<array.count();i++)
    this->array[this->num++] = array[i];
}

template <class T> inline void 
dimeArray<T>::prepend(const dimeArray<T> &array)
{
  int newsize=this->num+array.count();
  int i;
  if (this->size<=newsize) {
    T *oldarray=this->array;
    this->array=new T[newsize];
    this->size=newsize;
    for (i=0;i<array.count(); i++) this->array[i] = array[i];
    for (i=0;i<this->num;i++) this->array[i+array.count()] = oldarray[i];
    delete[] oldarray;
  }
  else {
    for (i=0;i<this->num;i++) this->array[array.count()+i]=this->array[i];
    for (i=0;i<array.count();i++) this->array[i] = array[i];
  }
  this->num+=array.count();
}

template <class T> inline void 
dimeArray<T>::insertElem(const int idx, const T &elem)
{
  int n = this->num;
  this->append(elem); // make room for one more
  if (idx < n) {
    for (int i = n; i > idx; i--) {
      this->array[i] = this->array[i-1]; 
    }
    this->array[idx] = elem;
  }
}

template <class T> inline void 
dimeArray<T>::setElem(const int index, const T &elem)
{
  while (index >= this->size) growArray();
  if (this->num <= index) this->num = index+1;
  this->array[index] = elem;
}

template <class T> inline T 
dimeArray<T>::getElem(const int index) const
{
  return this->array[index];
}

template <class T> inline void 
dimeArray<T>::getElem(const int index, T &elem) const
{
  elem = this->array[index];
}

template <class T> inline T 
dimeArray<T>::getLastElem() const
{
  return this->array[this->num-1];
}

template <class T> inline void 
dimeArray<T>::getLastElem(T &elem) const
{
  elem = this->array[this->num-1];
}

template <class T> inline T &
dimeArray<T>::operator [](const int index)
{
  while (index >= this->size) growArray();   
  if (this->num <= index) this->num = index + 1;
  return this->array[index];
}

template <class T> inline T 
dimeArray<T>::operator [](const int index) const
{
  return this->array[index];
}

template <class T> inline void 
dimeArray<T>::removeElem(const int index)
{
  if (this->num <= 0 || index >= this->num) return; 
  for (int i = index; i < this->num-1; i++)
    this->array[i] = this->array[i+1];
  this->num--;
}

template <class T> inline void 
dimeArray<T>::removeElemFast(const int index)
{
  this->array[index] = this->array[--this->num];
}

template <class T> inline void 
dimeArray<T>::reverse()
{
  T tmp;
  for (int i=0;i<this->num/2;i++) {
    tmp=this->array[i];
    this->array[i]=this->array[this->num-1-i];
    this->array[this->num-1-i]=tmp;
  }
}

template <class T> inline void 
dimeArray<T>::setCount(const int count)
{
  if (count < this->num)
    this->num = count;  
}

template <class T> inline int 
dimeArray<T>::count() const
{
  return this->num;
}

template <class T> inline int 
dimeArray<T>::allocSize() const
{
  return this->size;
}

template <class T> inline T *
dimeArray<T>::arrayPointer()
{
  return this->array;
}

template <class T> inline const T *
dimeArray<T>::constArrayPointer() const
{
  return this->array;
}

template <class T> inline void 
dimeArray<T>::makeEmpty(const int initsize)
{
  delete [] this->array;
  this->array = new T[initsize];
  this->size = initsize;
  this->num = 0; 
}

template <class T> inline void 
dimeArray<T>::freeMemory()
{
  delete [] this->array;
  this->array = NULL;
  this->size = 0;
  this->num = 0;
}

template <class T> inline void 
dimeArray<T>::shrinkToFit()
{
  T *oldarray = this->array;
  this->array = new T[this->num];
  for (int i = 0; i < this->num; i++) this->array[i] = oldarray[i];
  this->size = this->num;
  delete [] oldarray;
}

#endif // ! DIME_ARRAY_H

