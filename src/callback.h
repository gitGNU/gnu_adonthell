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

/**
 * @file   callback_slot.h
 * @author Joel Vennin <jol@linuxgames.com>
 * 
 * @brief  Declares the callback template class.
 * 
 * 
 */


#ifndef _CALL_BACK_H_
#define _CALL_BACK_H_

#include <string.h> 
#include <stddef.h>                     


/**
 * All stuff is here is inspirated from Rich Hickey callback mechanics. Everybody know what is
 * callback, so i explain just how use it in example.
 *
 *  class A{                            
 *   public:
 *    void print() { cout << "Adonthell callback\n"; }
 * 
 *    int count(int i) { return ++i; }
 *   };
 *  
 *  void main()
 *  {
 *    Functor0 func0;
 *    Functor1wRet<int,int> func1;
 *    A a;
 * 
 *    func0 = makeFunctor(a, &A::print);
 *    func1 = makeFunctor(&func1,a,&A::count);
 * 
 *    func0();
 *    cout << func1(10000) << endl;
 *   }
 *
 * With different callback template you can have 0,1,2,3 args and a return value, enjoy !
 */


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


/* No Argument, No return */
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





/* Arg, with return */

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
	MemberTranslator0wRet(Callee &c,const MemFunc &m):
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
	FunctionTranslator0wRet(Func f):Functor0wRet<RT>(thunk,0,(PFunc)f,0,0){}
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




/* 1 Argument, no return value */
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
    MemberTranslator1 (Callee & c, const MemFunc & m)
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
    MemberOf1stArgTranslator1 (const MemFunc & m) :
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




/* one arg, with return */
template <class P1,class RT>
class Functor1wRet:public FunctorBase{
public:
	Functor1wRet(){}
	RT operator()(P1 p1)const
		{
		return (thunk(*this,p1));
		}
	//for STL
	typedef P1 argument_type;
	typedef RT result_type;
protected:
	typedef RT (*Thunk)(const FunctorBase &,P1);
	Functor1wRet(Thunk t,const void *c,PFunc f,const void *mf,size_t sz):
		FunctorBase(c,f,mf,sz),thunk(t){}
private:
	Thunk thunk;
};

template <class P1,class RT,class Callee, class MemFunc>
class MemberTranslator1wRet:public Functor1wRet<P1,RT>{
public:
	MemberTranslator1wRet(Callee &c,const MemFunc &m):
		Functor1wRet<P1,RT>(thunk,&c,0,&m,sizeof(MemFunc)){}
	static RT thunk(const FunctorBase &ftor,P1 p1)
		{
		Callee *callee = (Callee *)ftor.getCallee();
		MemFunc &memFunc 
			(*(MemFunc*)(void *)(ftor.getMemFunc()));
		return ((callee->*memFunc)(p1));
		}
};

template <class P1,class RT,class Func>
class FunctionTranslator1wRet:public Functor1wRet<P1,RT>{
public:
	FunctionTranslator1wRet(Func f):
		Functor1wRet<P1,RT>(thunk,0,(PFunc)f,0,0){}
	static RT thunk(const FunctorBase &ftor,P1 p1)
		{
		return (Func(ftor.getFunc()))(p1);
		}
};


template <class P1,class RT,
	class Callee,class TRT,class CallType,class TP1>
inline MemberTranslator1wRet<P1,RT,Callee,TRT (CallType::*)(TP1)>
makeFunctor(Functor1wRet<P1,RT>*,Callee &c,
	TRT (CallType::*  f)(TP1))
	{
	typedef TRT (CallType::*MemFunc)(TP1);
	return MemberTranslator1wRet<P1,RT,Callee,MemFunc>(c,f);
	}


template <class P1,class RT,
	class Callee,class TRT,class CallType,class TP1>
inline MemberTranslator1wRet<P1,RT,
	const Callee,TRT (CallType::*)(TP1)const>
makeFunctor(Functor1wRet<P1,RT>*,
	const Callee &c,TRT (CallType::*  f)(TP1)const)
	{
	typedef TRT (CallType::*MemFunc)(TP1)const;
	return MemberTranslator1wRet<P1,RT,const Callee,MemFunc>(c,f);
	}

template <class P1,class RT,class TRT,class TP1>
inline FunctionTranslator1wRet<P1,RT,TRT (*)(TP1)>
makeFunctor(Functor1wRet<P1,RT>*,TRT (*f)(TP1))
	{
	return FunctionTranslator1wRet<P1,RT,TRT (*)(TP1)>(f);
	}

template <class P1,class RT,class MemFunc>
class MemberOf1stArgTranslator1wRet:public Functor1wRet<P1,RT>{
public:
	MemberOf1stArgTranslator1wRet(const MemFunc &m):
		Functor1wRet<P1,RT>(thunk,(void *)1,0,&m,sizeof(MemFunc)){}
	static RT thunk(const FunctorBase &ftor,P1 p1)
		{
		MemFunc &memFunc 
			(*(MemFunc*)(void *)(ftor.getMemFunc()));
		return ((p1.*memFunc)());
		}
};


template <class P1,class RT,class TRT,class CallType>
inline MemberOf1stArgTranslator1wRet<P1,RT,TRT (CallType::*)()>
makeFunctor(Functor1wRet<P1,RT>*,TRT (CallType::*  f)())
	{
	typedef TRT (CallType::*MemFunc)();
	return MemberOf1stArgTranslator1wRet<P1,RT,MemFunc>(f);
	}


template <class P1,class RT,class TRT,class CallType>
inline MemberOf1stArgTranslator1wRet<P1,RT,TRT (CallType::*)()const>
makeFunctor(Functor1wRet<P1,RT>*,TRT (CallType::*  f)()const)
	{
	typedef TRT (CallType::*MemFunc)()const;
	return MemberOf1stArgTranslator1wRet<P1,RT,MemFunc>(f);
	}



/* two args, with return */
template <class P1,class P2,class RT>
class Functor2wRet:public FunctorBase{
public:
	Functor2wRet(){}
	RT operator()(P1 p1,P2 p2)const
		{
		return (thunk(*this,p1,p2));
		}
	//for STL
	typedef P1 first_argument_type;
	typedef P2 second_argument_type;
	typedef RT result_type;
protected:
	typedef RT (*Thunk)(const FunctorBase &,P1,P2);
	Functor2wRet(Thunk t,const void *c,PFunc f,const void *mf,size_t sz):
		FunctorBase(c,f,mf,sz),thunk(t){}
private:
	Thunk thunk;
};

template <class P1,class P2,class RT,class Callee, class MemFunc>
class MemberTranslator2wRet:public Functor2wRet<P1,P2,RT>{
public:
	MemberTranslator2wRet(Callee &c,const MemFunc &m):
		Functor2wRet<P1,P2,RT>(thunk,&c,0,&m,sizeof(MemFunc)){}
	static RT thunk(const FunctorBase &ftor,P1 p1,P2 p2)
		{
		Callee *callee = (Callee *)ftor.getCallee();
		MemFunc &memFunc 
			(*(MemFunc*)(void *)(ftor.getMemFunc()));
		return ((callee->*memFunc)(p1,p2));
		}
};

template <class P1,class P2,class RT,class Func>
class FunctionTranslator2wRet:public Functor2wRet<P1,P2,RT>{
public:
	FunctionTranslator2wRet(Func f):
		Functor2wRet<P1,P2,RT>(thunk,0,(PFunc)f,0,0){}
	static RT thunk(const FunctorBase &ftor,P1 p1,P2 p2)
		{
		return (Func(ftor.getFunc()))(p1,p2);
		}
};


template <class P1,class P2,class RT,class Callee,
	class TRT,class CallType,class TP1,class TP2>
inline MemberTranslator2wRet<P1,P2,RT,Callee,
	TRT (CallType::*)(TP1,TP2)>
makeFunctor(Functor2wRet<P1,P2,RT>*,Callee &c,
	TRT (CallType::*  f)(TP1,TP2))
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2);
	return MemberTranslator2wRet<P1,P2,RT,Callee,MemFunc>(c,f);
	}


template <class P1,class P2,class RT,class Callee,
	class TRT,class CallType,class TP1,class TP2>
inline MemberTranslator2wRet<P1,P2,RT,const Callee,
	TRT (CallType::*)(TP1,TP2)const>
makeFunctor(Functor2wRet<P1,P2,RT>*,const Callee &c,
	TRT (CallType::*  f)(TP1,TP2)const)
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2)const;
	return MemberTranslator2wRet<P1,P2,RT,const Callee,MemFunc>(c,f);
	}

template <class P1,class P2,class RT,class TRT,class TP1,class TP2>
inline FunctionTranslator2wRet<P1,P2,RT,TRT (*)(TP1,TP2)>
makeFunctor(Functor2wRet<P1,P2,RT>*,TRT (*f)(TP1,TP2))
	{
	return FunctionTranslator2wRet<P1,P2,RT,TRT (*)(TP1,TP2)>(f);
	}

template <class P1,class P2,class RT,class MemFunc>
class MemberOf1stArgTranslator2wRet:public Functor2wRet<P1,P2,RT>{
public:
	MemberOf1stArgTranslator2wRet(const MemFunc &m):
		Functor2wRet<P1,P2,RT>(thunk,(void *)1,0,&m,sizeof(MemFunc)){}
	static RT thunk(const FunctorBase &ftor,P1 p1,P2 p2)
		{
		MemFunc &memFunc 
			(*(MemFunc*)(void *)(ftor.getMemFunc()));
		return ((p1.*memFunc)(p2));
		}
};



template <class P1,class P2,class RT,class TRT,class CallType,class TP1>
inline MemberOf1stArgTranslator2wRet<P1,P2,RT,TRT (CallType::*)(TP1)>
makeFunctor(Functor2wRet<P1,P2,RT>*,TRT (CallType::* f)(TP1))
	{
	typedef TRT (CallType::*MemFunc)(TP1);
	return MemberOf1stArgTranslator2wRet<P1,P2,RT,MemFunc>(f);
	}


template <class P1,class P2,class RT,class TRT,class CallType,class TP1>
inline MemberOf1stArgTranslator2wRet<P1,P2,RT,TRT (CallType::*)(TP1)const>
makeFunctor(Functor2wRet<P1,P2,RT>*,
	TRT (CallType::* f)(TP1)const)
	{
	typedef TRT (CallType::*MemFunc)(TP1)const;
	return MemberOf1stArgTranslator2wRet<P1,P2,RT,MemFunc>(f);
	}




/* three args, with return */
template <class P1,class P2,class P3,class RT>
class Functor3wRet:public FunctorBase{
public:
	Functor3wRet(){}
	RT operator()(P1 p1,P2 p2,P3 p3)const
		{
		return (thunk(*this,p1,p2,p3));
		}
protected:
	typedef RT (*Thunk)(const FunctorBase &,P1,P2,P3);
	Functor3wRet(Thunk t,const void *c,PFunc f,const void *mf,size_t sz):
		FunctorBase(c,f,mf,sz),thunk(t){}
private:
	Thunk thunk;
};

template <class P1,class P2,class P3,
	class RT,class Callee, class MemFunc>
class MemberTranslator3wRet:public Functor3wRet<P1,P2,P3,RT>{
public:
	MemberTranslator3wRet(Callee &c,const MemFunc &m):
		Functor3wRet<P1,P2,P3,RT>(thunk,&c,0,&m,sizeof(MemFunc)){}
	static RT thunk(const FunctorBase &ftor,P1 p1,P2 p2,P3 p3)
		{
		Callee *callee = (Callee *)ftor.getCallee();
		MemFunc &memFunc
			(*(MemFunc*)(void *)(ftor.getMemFunc()));
		return ((callee->*memFunc)(p1,p2,p3));
		}
};

template <class P1,class P2,class P3,class RT,class Func>
class FunctionTranslator3wRet:public Functor3wRet<P1,P2,P3,RT>{
public:
	FunctionTranslator3wRet(Func f):
		Functor3wRet<P1,P2,P3,RT>(thunk,0,(PFunc)f,0,0){}
	static RT thunk(const FunctorBase &ftor,P1 p1,P2 p2,P3 p3)
		{
		return (Func(ftor.getFunc()))(p1,p2,p3);
		}
};



template <class P1,class P2,class P3,class RT,class Callee,
	class TRT,class CallType,class TP1,class TP2,class TP3>
inline MemberTranslator3wRet<P1,P2,P3,RT,Callee,
	TRT (CallType::*)(TP1,TP2,TP3)>
makeFunctor(Functor3wRet<P1,P2,P3,RT>*,Callee &c,
	TRT (CallType::* f)(TP1,TP2,TP3))
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3);
	return MemberTranslator3wRet<P1,P2,P3,RT,Callee,MemFunc>(c,f);
	}


template <class P1,class P2,class P3,class RT,class Callee,
	class TRT,class CallType,class TP1,class TP2,class TP3>
inline MemberTranslator3wRet<P1,P2,P3,RT,const Callee,
	TRT (CallType::*)(TP1,TP2,TP3)const>
makeFunctor(Functor3wRet<P1,P2,P3,RT>*,const Callee &c,
	TRT (CallType::* f)(TP1,TP2,TP3)const)
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2,TP3)const;
	return MemberTranslator3wRet<P1,P2,P3,RT,const Callee,MemFunc>(c,f);
	}

template <class P1,class P2,class P3,class RT,
	class TRT,class TP1,class TP2,class TP3>
inline FunctionTranslator3wRet<P1,P2,P3,RT,TRT (*)(TP1,TP2,TP3)>
makeFunctor(Functor3wRet<P1,P2,P3,RT>*,TRT (*f)(TP1,TP2,TP3))
	{
	return FunctionTranslator3wRet<P1,P2,P3,RT,TRT (*)(TP1,TP2,TP3)>(f);
	}

template <class P1,class P2,class P3,class RT,class MemFunc>
class MemberOf1stArgTranslator3wRet:public Functor3wRet<P1,P2,P3,RT>{
public:
	MemberOf1stArgTranslator3wRet(const MemFunc &m):
		Functor3wRet<P1,P2,P3,RT>(thunk,(void *)1,0,&m,sizeof(MemFunc)){}
	static RT thunk(const FunctorBase &ftor,P1 p1,P2 p2,P3 p3)
		{
		MemFunc &memFunc
			(*(MemFunc*)(void *)(ftor.getMemFunc()));
		return ((p1.*memFunc)(p2,p3));
		}
};


template <class P1,class P2,class P3,class RT,class TRT,class CallType,
	class TP1,class TP2>
inline MemberOf1stArgTranslator3wRet<P1,P2,P3,RT,TRT (CallType::*)(TP1,TP2)>
makeFunctor(Functor3wRet<P1,P2,P3,RT>*,
	TRT (CallType::* f)(TP1,TP2))
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2);
	return MemberOf1stArgTranslator3wRet<P1,P2,P3,RT,MemFunc>(f);
	}


template <class P1,class P2,class P3,class RT,class TRT,class CallType,
	class TP1,class TP2>
inline MemberOf1stArgTranslator3wRet<P1,P2,P3,RT,
	TRT (CallType::*)(TP1,TP2)const>
makeFunctor(Functor3wRet<P1,P2,P3,RT>*,
	TRT (CallType::* f)(TP1,TP2)const)
	{
	typedef TRT (CallType::*MemFunc)(TP1,TP2)const;
	return MemberOf1stArgTranslator3wRet<P1,P2,P3,RT,MemFunc>(f);
	}

#endif






