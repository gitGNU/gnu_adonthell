/*
   (C) Copyright 2000 Joel Vennin
   Part of the Adonthell Project http://adonthell.linuxgames.com
   Special Thanks to Rich Hickey
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details
*/

#ifndef _CALL_BACK_H_
#define _CALL_BACK_H_

#include <string.h> 
#include <stddef.h> 

class FunctorBase
{
 protected:
  typedef void (FunctorBase::*PMemFunc)();
  typedef void (*PFunc)();
  enum {MEM_FUNC_SIZE = sizeof(PMemFunc)};
  union{
    PFunc func;
    char memFunc[MEM_FUNC_SIZE];
  };
  void *callee;
  FunctorBase():callee(0),func(0){}
  FunctorBase(const void *c,PFunc f, const void *mf,size_t sz):callee((void *)c)
    {
      if(c)	
	{
	  memcpy(memFunc,mf,sz);
	  if(sz<MEM_FUNC_SIZE)	
	    memset(memFunc+sz,0,MEM_FUNC_SIZE-sz);
	}	
      else func = f;
    }
  
 public:
  operator bool()const{return callee||func;}
  friend bool operator==(const FunctorBase &lhs,const FunctorBase &rhs);
  friend bool operator!=(const FunctorBase &lhs,const FunctorBase &rhs);
  friend bool operator<(const FunctorBase &lhs,const FunctorBase &rhs);
  PFunc	getFunc() const {return func;}
  void *getCallee() const {return callee;}
  const char *getMemFunc() const {return memFunc;}
};

class Functor0:public FunctorBase
{
 private:
  typedef void (*Thunk)(const FunctorBase &);
  Thunk thunk;
 protected:
  Functor0(Thunk t,const void *c,PFunc f,const void *mf,size_t sz):FunctorBase(c,f,mf,sz),thunk(t){}
 public:
  Functor0(){}
  void operator()()const
    {
      thunk(*this);
    }
};

template <class Callee, class MemFunc>
class MemberTranslator0:public Functor0{
 public:
  MemberTranslator0(Callee &c,const MemFunc &m):Functor0(thunk,&c,0,&m,sizeof(MemFunc)){}
  static void thunk(const FunctorBase &ftor)
    {
      Callee *callee = (Callee *)ftor.getCallee();
      MemFunc &memFunc = (*(MemFunc*)(void *)(ftor.getMemFunc()));
      (callee->*memFunc)();
    }
};

template <class Func>
class FunctionTranslator0:public Functor0{
 public:
  FunctionTranslator0(Func f):Functor0(thunk,0,(PFunc)f,0,0){}
  static void thunk(const FunctorBase &ftor)
    {
      (Func(ftor.getFunc()))();
    }
};

template <class Callee,class TRT,class CallType>
inline MemberTranslator0<Callee,TRT (CallType::*)()>
makeFunctor(Callee &c,TRT (CallType::* f)())
{
  typedef TRT (CallType::*MemFunc)();
  return MemberTranslator0<Callee,MemFunc>(c,f);
}


template <class Callee,class TRT,class CallType>
inline MemberTranslator0<const Callee,TRT (CallType::*)()const>
makeFunctor(const Callee &c,
	    TRT (CallType::* f)()const)
{
  typedef TRT (CallType::*MemFunc)()const;
  return MemberTranslator0<const Callee,MemFunc>(c,f);
}

template <class TRT>
inline FunctionTranslator0<TRT (*)()>
makeFunctor(TRT (*f)())
{
  return FunctionTranslator0<TRT (*)()>(f);
}
#endif







