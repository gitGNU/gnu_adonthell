/*
   $Id$

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
  FunctorBase():func(0),callee(0){}
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
  MemberTranslator0(Callee &c, MemFunc &m):Functor0(thunk,&c,0,&m,sizeof(MemFunc)){}
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
makeFunctor(const Callee &c, TRT (CallType::* f)()const)
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


//NO ARG WITH RETURN

template <class RT>
class Functor0wRet:public FunctorBase{
public:
  Functor0wRet(){}
	RT operator()()const
		{
		return thunk(*this);
		}
protected:
	typedef RT (*Thunk)(const FunctorBase &);
	Functor0wRet(Thunk t,const void *c,PFunc f,const void *mf,size_t sz):
		FunctorBase(c,f,mf,sz),thunk(t){}
private:
	Thunk thunk;
};


template <class RT,class Callee, class MemFunc>
class MemberTranslator0wRet:public Functor0wRet<RT>{
public:
	MemberTranslator0wRet(Callee &c,MemFunc &m):
		Functor0wRet<RT>(thunk,&c,0,&m,sizeof(MemFunc)){}
	static RT thunk(const FunctorBase &ftor)
		{
		Callee *callee = (Callee *)ftor.getCallee();
		MemFunc &memFunc = (*(MemFunc*)(void *)(ftor.getMemFunc()));
		return ((callee->*memFunc)());
		}
};


template <class RT,class Func>
class FunctionTranslator0wRet:public Functor0wRet<RT>{
public:
	FunctionTranslator0wRet(Func f):Functor0wRet<RT>(thunk,0,(typename FunctionTranslator0wRet<RT, Func>::PFunc)f,0,0){}
	static RT thunk(const FunctorBase &ftor)
		{
		return (Func(ftor.getFunc()))();
		}
};



template <class RT,class Callee,class TRT,class CallType>
inline MemberTranslator0wRet<RT,Callee,TRT (CallType::*)()>
makeFunctor(Functor0wRet<RT>*,Callee &c,TRT (CallType::* f)())
{
  typedef TRT (CallType::*MemFunc)();
  return MemberTranslator0wRet<RT,Callee,MemFunc>(c,f);
}



template <class RT,class Callee,class TRT,class CallType>
inline MemberTranslator0wRet<RT,const Callee,TRT (CallType::*)()const>
makeFunctor(Functor0wRet<RT>*,const Callee &c, TRT (CallType::* f)()const)
{
  typedef TRT (CallType::*MemFunc)()const;
  return MemberTranslator0wRet<RT,const Callee,MemFunc>(c,f);
}


template <class RT,class TRT>
inline FunctionTranslator0wRet<RT,TRT (*)()>
makeFunctor(Functor0wRet<RT>*,TRT (*f)())
{
  return FunctionTranslator0wRet<RT,TRT (*)()>(f);
}

// 1 Argument, no return value
template <class P1> class Functor1 : public FunctorBase
{
public:
    Functor1() {}
    void operator () (P1 p1) const
    {
        thunk (*this, p1);
    }
protected:
    typedef void (*Thunk) (const FunctorBase &, P1);
    Functor1 (Thunk t, const void *c, PFunc f, const void *mf, size_t sz)
        : FunctorBase (c, f, mf, sz), thunk (t) { }
private:
    Thunk thunk;
};

template <class P1, class Callee, class MemFunc>
class MemberTranslator1 : public Functor1<P1>
{
  public:
    MemberTranslator1 (Callee & c, MemFunc & m)
        : Functor1<P1> (thunk, &c, 0, &m, sizeof (MemFunc)) { }
    static void thunk (const FunctorBase & ftor, P1 p1)
    {
        Callee *callee = (Callee *) ftor.getCallee ();
        MemFunc & memFunc (*(MemFunc *) (void *)(ftor.getMemFunc ()));
        (callee->*memFunc) (p1);
    }
};

template <class P1, class Func> 
class FunctionTranslator1 : public Functor1 <P1>
{
public:
  FunctionTranslator1 (Func f) : Functor1 <P1> (thunk, 0, f, 0, 0) { }
    static void thunk (const FunctorBase & ftor, P1 p1)
    {
        (Func (ftor.func)) (p1);
    }
};

template <class P1, class Callee, class TRT, class CallType>
inline MemberTranslator1 <P1, Callee, TRT (CallType::*)(P1)>
makeFunctor (Callee & c, TRT (CallType::* f) (P1))
{
    typedef TRT (CallType::*MemFunc) (P1);
    return MemberTranslator1 <P1, Callee, MemFunc> (c, f);
}

template <class P1, class Callee, class TRT, class CallType, class TP1>
inline MemberTranslator1 <P1, const Callee, TRT (CallType::*) (TP1) const>
makeFunctor (const Callee & c, TRT (CallType::*const &f) (TP1) const)
{
    typedef TRT (CallType::*MemFunc) (TP1) const;
    return MemberTranslator1 <P1, const Callee, MemFunc> (c, f);
}

template <class P1, class TRT, class TP1>
inline FunctionTranslator1 <P1, TRT (*)(TP1)>
makeFunctor (TRT (*f) (TP1))
{
    return FunctionTranslator1 <P1, TRT (*)(TP1) > (f);
}

template <class P1, class MemFunc> class MemberOf1stArgTranslator1 
: public Functor1 <P1>
{
public:
    MemberOf1stArgTranslator1 (MemFunc & m) :
    Functor1 < P1 > (thunk, (void *)1, 0, &m, sizeof (MemFunc)) { }
    static void thunk (const FunctorBase & ftor, P1 p1)
    {
        MemFunc & memFunc (*(MemFunc *) (void *)(ftor.memFunc));
        (p1.*memFunc) ();
    }
};

template <class P1, class TRT, class CallType>
inline MemberOf1stArgTranslator1 <P1, TRT (CallType::*)()>
makeFunctor (TRT (CallType::* &f) ())
{
    typedef TRT (CallType::*MemFunc) ();
    return MemberOf1stArgTranslator1 <P1, MemFunc> (f);
}

template <class P1, class TRT, class CallType>
inline MemberOf1stArgTranslator1 < P1, TRT (CallType::*)() const>
makeFunctor (TRT (CallType::*const &f) () const)
{
    typedef TRT (CallType::*MemFunc) () const;
    return MemberOf1stArgTranslator1 <P1, MemFunc> (f);
}


#endif






