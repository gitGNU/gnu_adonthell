/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3u-20010301-1937 (Alpha 5)
 * 
 * This file is not intended to be easily readable and contains a number of 
 * coding conventions designed to improve portability and efficiency. Do not make
 * changes to this file unless you know what you are doing--modify the SWIG 
 * interface file instead. 
 * ----------------------------------------------------------------------------- */

#define SWIGPYTHON
/***********************************************************************
 * common.swg
 *
 *     This file contains generic SWIG runtime support for pointer
 *     type checking as well as a few commonly used macros to control
 *     external linkage.
 *
 * Author : David Beazley (beazley@cs.uchicago.edu)
 *
 * Copyright (c) 1999-2000, The University of Chicago
 * 
 * This file may be freely redistributed without license or fee provided
 * this copyright message remains intact.
 ************************************************************************/

#include <string.h>

#if defined(_WIN32) || defined(__WIN32__)
#       if defined(_MSC_VER)
#               if defined(STATIC_LINKED)
#                       define SWIGEXPORT(a) a
#               else
#                       define SWIGEXPORT(a) __declspec(dllexport) a
#               endif
#       else
#               if defined(__BORLANDC__)
#                       define SWIGEXPORT(a) a _export
#               else
#                       define SWIGEXPORT(a) a
#       endif
#endif
#else
#       define SWIGEXPORT(a) a
#endif

#ifdef SWIG_GLOBAL
#define SWIGRUNTIME(a) SWIGEXPORT(a)
#else
#define SWIGRUNTIME(a) static a
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct swig_type_info {
  char  *name;                 
  void *(*converter)(void *);
  char  *str;
  struct swig_type_info  *next;
  struct swig_type_info  *prev;
} swig_type_info;

#ifdef SWIG_NOINCLUDE
SWIGEXPORT(swig_type_info *) SWIG_TypeRegister(swig_type_info *);
SWIGEXPORT(swig_type_info *) SWIG_TypeCheck(char *c, swig_type_info *);
SWIGEXPORT(void *) SWIG_TypeCast(swig_type_info *, void *);
#else

static swig_type_info *swig_type_list = 0;

/* Register a type mapping with the type-checking */
SWIGRUNTIME(swig_type_info *)
SWIG_TypeRegister(swig_type_info *ti)
{
  swig_type_info *tc, *head, *ret, *next;
  /* Check to see if this type has already been registered */
  tc = swig_type_list;
  while (tc) {
    if (strcmp(tc->name, ti->name) == 0) {
      /* Already exists in the table.  Just add additional types to the list */
      head = tc;
      next = tc->next;
      goto l1;
    }
    tc = tc->prev;
  }
  head = ti;
  next = 0;

  /* Place in list */
  ti->prev = swig_type_list;
  swig_type_list = ti;

  /* Build linked lists */
 l1:
  ret = head;
  tc = ti + 1;
  /* Patch up the rest of the links */
  while (tc->name) {
    head->next = tc;
    tc->prev = head;
    head = tc;
    tc++;
  }
  head->next = next;
  return ret;
}

/* Check the typename */
SWIGRUNTIME(swig_type_info *) 
SWIG_TypeCheck(char *c, swig_type_info *ty)
{
  swig_type_info *s;
  if (!ty) return 0;        /* Void pointer */
  s = ty->next;             /* First element always just a name */
  while (s) {
    if (strcmp(s->name,c) == 0) {
      if (s == ty->next) return s;
      /* Move s to the top of the linked list */
      s->prev->next = s->next;
      if (s->next) {
	s->next->prev = s->prev;
      }
      /* Insert s as second element in the list */
      s->next = ty->next;
      if (ty->next) ty->next->prev = s;
      ty->next = s;
      return s;
    }
    s = s->next;
  }
  return 0;
}

/* Cast a pointer (needed for C++ inheritance */
SWIGRUNTIME(void *) 
SWIG_TypeCast(swig_type_info *ty, void *ptr) 
{
  if ((!ty) || (!ty->converter)) return ptr;
  return (*ty->converter)(ptr);
}

/* Search for a swig_type_info structure */
SWIGRUNTIME(void *)
SWIG_TypeQuery(const char *name) {
  swig_type_info *ty = swig_type_list;
  while (ty) {
    if (ty->str && (strcmp(name,ty->str) == 0)) return ty;
    if (ty->name && (strcmp(name,ty->name) == 0)) return ty;
    ty = ty->prev;
  }
  return 0;
}

#endif

#ifdef __cplusplus
}
#endif



/***********************************************************************
 * python.swg
 *
 *     This file contains the runtime support for Python modules
 *     and includes code for managing global variables and pointer
 *     type checking.
 *
 * Author : David Beazley (beazley@cs.uchicago.edu)
 ************************************************************************/

#include <stdlib.h>
#include "Python.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SWIG_PY_INT     1
#define SWIG_PY_FLOAT   2
#define SWIG_PY_STRING  3
#define SWIG_PY_POINTER 4

/* Constant information structure */
typedef struct swig_const_info {
    int type;
    char *name;
    long lvalue;
    double dvalue;
    void   *pvalue;
    swig_type_info **ptype;
} swig_const_info;

#ifdef SWIG_NOINCLUDE

SWIGEXPORT(PyObject *)        SWIG_newvarlink();
SWIGEXPORT(void)              SWIG_addvarlink(PyObject *, char *, PyObject *(*)(void), int (*)(PyObject *));
SWIGEXPORT(int)               SWIG_ConvertPtr(PyObject *, void **, swig_type_info *, int);
SWIGEXPORT(void)              SWIG_MakePtr(char *c, void *, swig_type_info *);
SWIGEXPORT(PyObject *)        SWIG_NewPointerObj(void *, swig_type_info *);
SWIGEXPORT(void)              SWIG_InstallConstants(PyObject *d, swig_const_info constants[]);

#else

/* -----------------------------------------------------------------------------
 * global variable support code.
 * ----------------------------------------------------------------------------- */

typedef struct swig_globalvar {   
  char       *name;                  /* Name of global variable */
  PyObject *(*get_attr)(void);       /* Return the current value */
  int       (*set_attr)(PyObject *); /* Set the value */
  struct swig_globalvar *next;
} swig_globalvar;

typedef struct swig_varlinkobject {
  PyObject_HEAD
  swig_globalvar *vars;
} swig_varlinkobject;

static PyObject *
swig_varlink_repr(swig_varlinkobject *v) {
  v = v;
  return PyString_FromString("<Global variables>");
}

static int
swig_varlink_print(swig_varlinkobject *v, FILE *fp, int flags) {
  swig_globalvar  *var;
  flags = flags;
  fprintf(fp,"Global variables { ");
  for (var = v->vars; var; var=var->next) {
    fprintf(fp,"%s", var->name);
    if (var->next) fprintf(fp,", ");
  }
  fprintf(fp," }\n");
  return 0;
}

static PyObject *
swig_varlink_getattr(swig_varlinkobject *v, char *n) {
  swig_globalvar *var = v->vars;
  while (var) {
    if (strcmp(var->name,n) == 0) {
      return (*var->get_attr)();
    }
    var = var->next;
  }
  PyErr_SetString(PyExc_NameError,"Unknown C global variable");
  return NULL;
}

static int
swig_varlink_setattr(swig_varlinkobject *v, char *n, PyObject *p) {
  swig_globalvar *var = v->vars;
  while (var) {
    if (strcmp(var->name,n) == 0) {
      return (*var->set_attr)(p);
    }
    var = var->next;
  }
  PyErr_SetString(PyExc_NameError,"Unknown C global variable");
  return 1;
}

statichere PyTypeObject varlinktype = {
  PyObject_HEAD_INIT(0)              
  0,
  "swigvarlink",                      /* Type name    */
  sizeof(swig_varlinkobject),         /* Basic size   */
  0,                                  /* Itemsize     */
  0,                                  /* Deallocator  */ 
  (printfunc) swig_varlink_print,     /* Print        */
  (getattrfunc) swig_varlink_getattr, /* get attr     */
  (setattrfunc) swig_varlink_setattr, /* Set attr     */
  0,                                  /* tp_compare   */
  (reprfunc) swig_varlink_repr,       /* tp_repr      */    
  0,                                  /* tp_as_number */
  0,                                  /* tp_as_mapping*/
  0,                                  /* tp_hash      */
};

/* Create a variable linking object for use later */
SWIGRUNTIME(PyObject *)
SWIG_newvarlink(void) {
  swig_varlinkobject *result = 0;
  result = PyMem_NEW(swig_varlinkobject,1);
  varlinktype.ob_type = &PyType_Type;    /* Patch varlinktype into a PyType */
  result->ob_type = &varlinktype;
  result->vars = 0;
  result->ob_refcnt = 0;
  Py_XINCREF((PyObject *) result);
  return ((PyObject*) result);
}

SWIGRUNTIME(void)
SWIG_addvarlink(PyObject *p, char *name,
	   PyObject *(*get_attr)(void), int (*set_attr)(PyObject *p)) {
  swig_varlinkobject *v;
  swig_globalvar *gv;
  v= (swig_varlinkobject *) p;
  gv = (swig_globalvar *) malloc(sizeof(swig_globalvar));
  gv->name = (char *) malloc(strlen(name)+1);
  strcpy(gv->name,name);
  gv->get_attr = get_attr;
  gv->set_attr = set_attr;
  gv->next = v->vars;
  v->vars = gv;
}
/* Convert a pointer value */
SWIGRUNTIME(int)
SWIG_ConvertPtr(PyObject *obj, void **ptr, swig_type_info *ty, int flags) {
  unsigned long p;
  register int d;
  swig_type_info *tc;
  char  *c;
  static PyObject *SWIG_this = 0;
  int    newref = 0;

  if (!obj || (obj == Py_None)) {
    *ptr = 0;
    return 0;
  }
#ifdef SWIG_COBJECT_TYPES
  if (!(PyCObject_Check(obj))) {
    if (!SWIG_this)
      SWIG_this = PyString_InternFromString("this");
    obj = PyObject_GetAttr(obj,SWIG_this);
    newref = 1;
    if (!obj) goto type_error;
    if (!PyCObject_Check(obj)) {
      Py_DECREF(obj);
      goto type_error;
    }
  } 
  *ptr = PyCObject_AsVoidPtr(obj);
  c = (char *) PyCObject_GetDesc(obj);
  if (newref) Py_DECREF(obj);
  goto cobject;
#else
  if (!(PyString_Check(obj))) {
    if (!SWIG_this)
      SWIG_this = PyString_InternFromString("this");
    obj = PyObject_GetAttr(obj,SWIG_this);
    newref = 1;
    if (!obj) goto type_error;
    if (!PyString_Check(obj)) {
      Py_DECREF(obj);
      goto type_error;
    }
  } 
  c = PyString_AsString(obj);
  p = 0;
  /* Pointer values must start with leading underscore */
  if (*c != '_') {
    *ptr = (void *) 0;
    if (strcmp(c,"NULL") == 0) {
      if (newref) Py_DECREF(obj);
      return 0;
    } else {
      if (newref) Py_DECREF(obj);
      goto type_error;
    }
  }
  c++;
  /* Extract hex value from pointer */
  while ((d = *c)) {
    if ((d >= '0') && (d <= '9'))
      p = (p << 4) + (d - '0');
    else if ((d >= 'a') && (d <= 'f'))
      p = (p << 4) + (d - ('a'-10));
    else
      break; 
    c++;
  }
  *ptr = (void *) p;
  if (newref) Py_DECREF(obj);
#endif

#ifdef SWIG_COBJECT_TYPES
cobject:
#endif

  if (ty) {
    tc = SWIG_TypeCheck(c,ty);
    if (!tc) goto type_error;
    *ptr = SWIG_TypeCast(tc,(void*)p);
  }
  return 0;

type_error:

  if (flags) {
    if (ty) {
      char *temp = (char *) malloc(64+strlen(ty->name));
      sprintf(temp,"Type error. Expected %s", ty->name);
      PyErr_SetString(PyExc_TypeError, temp);
      free((char *) temp);
    } else {
      PyErr_SetString(PyExc_TypeError,"Expected a pointer");
    }
  }
  return -1;
}

/* Take a pointer and convert it to a string */
SWIGRUNTIME(void) 
SWIG_MakePtr(char *c, void *ptr, swig_type_info *ty) {
  static char hex[17] = "0123456789abcdef";
  unsigned long p, s;
  char result[32], *r; 
  r = result;
  p = (unsigned long) ptr;
  if (p > 0) {
    while (p > 0) {
      s = p & 0xf;
      *(r++) = hex[s];
      p = p >> 4;
    }
    *r = '_';
    while (r >= result)
      *(c++) = *(r--);
    strcpy (c, ty->name);
  } else {
    strcpy (c, "NULL");
  }
}

/* Create a new pointer object */
SWIGRUNTIME(PyObject *)
SWIG_NewPointerObj(void *ptr, swig_type_info *type) {
  char result[512];
  PyObject *robj;
  if (!ptr) {
    Py_INCREF(Py_None);
    return Py_None;
  }
#ifdef SWIG_COBJECT_TYPES
  robj = PyCObject_FromVoidPtrAndDesc((void *) ptr, type->name, NULL);
#else
  SWIG_MakePtr(result,ptr,type);
  robj = PyString_FromString(result);
#endif
  return robj;
}

/* Install Constants */
SWIGRUNTIME(void)
SWIG_InstallConstants(PyObject *d, swig_const_info constants[]) {
  int i;
  PyObject *obj;
  for (i = 0; constants[i].type; i++) {
    switch(constants[i].type) {
    case SWIG_PY_INT:
      obj = PyInt_FromLong(constants[i].lvalue);
      break;
    case SWIG_PY_FLOAT:
      obj = PyFloat_FromDouble(constants[i].dvalue);
      break;
    case SWIG_PY_STRING:
      obj = PyString_FromString((char *) constants[i].pvalue);
      break;
    case SWIG_PY_POINTER:
      obj = SWIG_NewPointerObj(constants[i].pvalue, *(constants[i]).ptype);
      break;
    default:
      obj = 0;
      break;
    }
    if (obj) {
      PyDict_SetItemString(d,constants[i].name,obj);
      Py_DECREF(obj);
    }
  }
}

#endif

#ifdef __cplusplus
}
#endif



/* -------- TYPES TABLE (BEGIN) -------- */

#define  SWIGTYPE_p_SDL_Surface swig_types[0] 
#define  SWIGTYPE_p_drawing_area swig_types[1] 
#define  SWIGTYPE_p_image swig_types[2] 
#define  SWIGTYPE_p_screen swig_types[3] 
#define  SWIGTYPE_p_config swig_types[4] 
static swig_type_info *swig_types[6];

/* -------- TYPES TABLE (END) -------- */


/*-----------------------------------------------
              @(target):= screenc.so
  ------------------------------------------------*/
#define SWIG_init    initscreenc

#define SWIG_name    "screenc"


#include "types.h"
#include "screen.h"

#ifdef __cplusplus
extern "C" {
#endif
static int _wrap_screen__bytes_per_pixel_set(PyObject *val) {
    unsigned char  tval ;
    
    tval = (unsigned char ) PyInt_AsLong(val);
    if (PyErr_Occurred()) {
        PyErr_SetString(PyExc_TypeError,"C variable 'screen_bytes_per_pixel'(unsigned char )");
        return 1; 
    }
    screen::bytes_per_pixel = tval;
    return 0;
}


static PyObject *_wrap_screen__bytes_per_pixel_get() {
    PyObject *pyobj;
    
    pyobj = PyInt_FromLong((long) screen::bytes_per_pixel);
    return pyobj;
}


static int _wrap_screen__trans_set(PyObject *val) {
    unsigned long  tval ;
    
    tval = (unsigned long ) PyInt_AsLong(val);
    if (PyErr_Occurred()) {
        PyErr_SetString(PyExc_TypeError,"C variable 'screen_trans'(unsigned long )");
        return 1; 
    }
    screen::trans = tval;
    return 0;
}


static PyObject *_wrap_screen__trans_get() {
    PyObject *pyobj;
    
    pyobj = PyInt_FromLong((long) screen::trans);
    return pyobj;
}


static int _wrap_screen__trans_pix_set(PyObject *val) {
    unsigned long  tval ;
    
    tval = (unsigned long ) PyInt_AsLong(val);
    if (PyErr_Occurred()) {
        PyErr_SetString(PyExc_TypeError,"C variable 'screen_trans_pix'(unsigned long )");
        return 1; 
    }
    screen::trans_pix = tval;
    return 0;
}


static PyObject *_wrap_screen__trans_pix_get() {
    PyObject *pyobj;
    
    pyobj = PyInt_FromLong((long) screen::trans_pix);
    return pyobj;
}


static int _wrap_screen__vis_set(PyObject *val) {
    SDL_Surface * temp ;
    
    if ((SWIG_ConvertPtr(val,(void **) &temp,SWIGTYPE_p_SDL_Surface,1)) == -1) return 1;
    screen::vis = temp;
    return 0;
}


static PyObject *_wrap_screen__vis_get() {
    PyObject *pyobj;
    
    pyobj = SWIG_NewPointerObj((void *)screen::vis, SWIGTYPE_p_SDL_Surface);
    return pyobj;
}


static PyObject *_wrap_screen_set_video_mode(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    unsigned short arg0 ;
    unsigned short arg1 ;
    config *arg2 = NULL ;
    PyObject * argo2 =0 ;
    
    if(!PyArg_ParseTuple(args,"hh|O:screen_set_video_mode",&arg0,&arg1,&argo2)) return NULL;
    if ((SWIG_ConvertPtr(argo2,(void **) &arg2,SWIGTYPE_p_config,1)) == -1) return NULL;
    screen::set_video_mode(arg0,arg1,arg2);
    Py_INCREF(Py_None);
    resultobj = Py_None;
    return resultobj;
}


static PyObject *_wrap_screen_set_fullscreen(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    bool arg0 ;
    int tempbool0 ;
    
    if(!PyArg_ParseTuple(args,"i:screen_set_fullscreen",&tempbool0)) return NULL;
    arg0 = (bool ) tempbool0;
    screen::set_fullscreen(arg0);
    Py_INCREF(Py_None);
    resultobj = Py_None;
    return resultobj;
}


static PyObject *_wrap_screen_get_fullscreen(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    bool result ;
    
    if(!PyArg_ParseTuple(args,":screen_get_fullscreen")) return NULL;
    result = (bool )screen::get_fullscreen();
    resultobj = PyInt_FromLong((long)result);
    return resultobj;
}


static PyObject *_wrap_screen_show(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    
    if(!PyArg_ParseTuple(args,":screen_show")) return NULL;
    screen::show();
    Py_INCREF(Py_None);
    resultobj = Py_None;
    return resultobj;
}


static PyObject *_wrap_screen_frames_to_do(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    unsigned char result ;
    
    if(!PyArg_ParseTuple(args,":screen_frames_to_do")) return NULL;
    result = (unsigned char )screen::frames_to_do();
    resultobj = PyInt_FromLong((long)result);
    return resultobj;
}


static PyObject *_wrap_screen_drawbox(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    unsigned short arg0 ;
    unsigned short arg1 ;
    unsigned short arg2 ;
    unsigned short arg3 ;
    unsigned long arg4 ;
    drawing_area *arg5 = NULL ;
    PyObject * argo5 =0 ;
    
    if(!PyArg_ParseTuple(args,"hhhhl|O:screen_drawbox",&arg0,&arg1,&arg2,&arg3,&arg4,&argo5)) return NULL;
    if ((SWIG_ConvertPtr(argo5,(void **) &arg5,SWIGTYPE_p_drawing_area,1)) == -1) return NULL;
    screen::drawbox(arg0,arg1,arg2,arg3,arg4,arg5);
    Py_INCREF(Py_None);
    resultobj = Py_None;
    return resultobj;
}


static PyObject *_wrap_screen_shot(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    image *result ;
    
    if(!PyArg_ParseTuple(args,":screen_shot")) return NULL;
    result = (image *)screen::shot();
    resultobj = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_image);
    return resultobj;
}


static PyObject *_wrap_screen_clear(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    
    if(!PyArg_ParseTuple(args,":screen_clear")) return NULL;
    screen::clear();
    Py_INCREF(Py_None);
    resultobj = Py_None;
    return resultobj;
}


static PyObject *_wrap_screen_makesquare(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    unsigned short arg0 ;
    unsigned short arg1 ;
    unsigned short arg2 ;
    
    if(!PyArg_ParseTuple(args,"hhh:screen_makesquare",&arg0,&arg1,&arg2)) return NULL;
    screen::makesquare(arg0,arg1,arg2);
    Py_INCREF(Py_None);
    resultobj = Py_None;
    return resultobj;
}


static PyObject *_wrap_screen_mouse_cursor_off(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    
    if(!PyArg_ParseTuple(args,":screen_mouse_cursor_off")) return NULL;
    screen::mouse_cursor_off();
    Py_INCREF(Py_None);
    resultobj = Py_None;
    return resultobj;
}


static PyObject *_wrap_screen_mouse_cursor_on(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    
    if(!PyArg_ParseTuple(args,":screen_mouse_cursor_on")) return NULL;
    screen::mouse_cursor_on();
    Py_INCREF(Py_None);
    resultobj = Py_None;
    return resultobj;
}


static PyObject *_wrap_screen_length(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    unsigned short result ;
    
    if(!PyArg_ParseTuple(args,":screen_length")) return NULL;
    result = (unsigned short )screen::length();
    resultobj = PyInt_FromLong((long)result);
    return resultobj;
}


static PyObject *_wrap_screen_height(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    unsigned short result ;
    
    if(!PyArg_ParseTuple(args,":screen_height")) return NULL;
    result = (unsigned short )screen::height();
    resultobj = PyInt_FromLong((long)result);
    return resultobj;
}


static PyObject *_wrap_new_screen(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    screen *result ;
    
    if(!PyArg_ParseTuple(args,":new_screen")) return NULL;
    result = (screen *)new screen();
    resultobj = SWIG_NewPointerObj((void *) result, SWIGTYPE_p_screen);
    return resultobj;
}


static PyObject *_wrap_delete_screen(PyObject *self, PyObject *args) {
    PyObject *resultobj;
    screen *arg0 ;
    PyObject * argo0 =0 ;
    
    if(!PyArg_ParseTuple(args,"O:delete_screen",&argo0)) return NULL;
    if ((SWIG_ConvertPtr(argo0,(void **) &arg0,SWIGTYPE_p_screen,1)) == -1) return NULL;
    delete arg0;
    Py_INCREF(Py_None);
    resultobj = Py_None;
    return resultobj;
}


static PyMethodDef screencMethods[] = {
	 { "screen_set_video_mode", _wrap_screen_set_video_mode, METH_VARARGS },
	 { "screen_set_fullscreen", _wrap_screen_set_fullscreen, METH_VARARGS },
	 { "screen_get_fullscreen", _wrap_screen_get_fullscreen, METH_VARARGS },
	 { "screen_show", _wrap_screen_show, METH_VARARGS },
	 { "screen_frames_to_do", _wrap_screen_frames_to_do, METH_VARARGS },
	 { "screen_drawbox", _wrap_screen_drawbox, METH_VARARGS },
	 { "screen_shot", _wrap_screen_shot, METH_VARARGS },
	 { "screen_clear", _wrap_screen_clear, METH_VARARGS },
	 { "screen_makesquare", _wrap_screen_makesquare, METH_VARARGS },
	 { "screen_mouse_cursor_off", _wrap_screen_mouse_cursor_off, METH_VARARGS },
	 { "screen_mouse_cursor_on", _wrap_screen_mouse_cursor_on, METH_VARARGS },
	 { "screen_length", _wrap_screen_length, METH_VARARGS },
	 { "screen_height", _wrap_screen_height, METH_VARARGS },
	 { "new_screen", _wrap_new_screen, METH_VARARGS },
	 { "delete_screen", _wrap_delete_screen, METH_VARARGS },
	 { NULL, NULL }
};

#ifdef __cplusplus
}
#endif

/* -------- TYPE CONVERSION AND EQUIVALENCE RULES (BEGIN) -------- */

static swig_type_info _swigt__p_SDL_Surface[] = {{"_p_SDL_Surface", 0, "SDL_Surface *"},{"_p_SDL_Surface"},{0}};
static swig_type_info _swigt__p_drawing_area[] = {{"_p_drawing_area", 0, "drawing_area *"},{"_p_drawing_area"},{0}};
static swig_type_info _swigt__p_image[] = {{"_p_image", 0, "image *"},{"_p_image"},{0}};
static swig_type_info _swigt__p_screen[] = {{"_p_screen", 0, "screen *"},{"_p_screen"},{0}};
static swig_type_info _swigt__p_config[] = {{"_p_config", 0, "config *"},{"_p_config"},{0}};

static swig_type_info *swig_types_initial[] = {
_swigt__p_SDL_Surface, 
_swigt__p_drawing_area, 
_swigt__p_image, 
_swigt__p_screen, 
_swigt__p_config, 
0
};


/* -------- TYPE CONVERSION AND EQUIVALENCE RULES (END) -------- */

static swig_const_info swig_const_table[] = {
{0}};

static PyObject *SWIG_globals;
#ifdef __cplusplus
extern "C" 
#endif
SWIGEXPORT(void) initscreenc(void) {
    PyObject *m, *d;
    int i;
    SWIG_globals = SWIG_newvarlink();
    m = Py_InitModule("screenc", screencMethods);
    d = PyModule_GetDict(m);
    for (i = 0; swig_types_initial[i]; i++) {
        swig_types[i] = SWIG_TypeRegister(swig_types_initial[i]);
    }
    PyDict_SetItemString(d,"cvar", SWIG_globals);
    SWIG_addvarlink(SWIG_globals,"screen_bytes_per_pixel",_wrap_screen__bytes_per_pixel_get, _wrap_screen__bytes_per_pixel_set);
    SWIG_addvarlink(SWIG_globals,"screen_trans",_wrap_screen__trans_get, _wrap_screen__trans_set);
    SWIG_addvarlink(SWIG_globals,"screen_trans_pix",_wrap_screen__trans_pix_get, _wrap_screen__trans_pix_set);
    SWIG_addvarlink(SWIG_globals,"screen_vis",_wrap_screen__vis_get, _wrap_screen__vis_set);
    SWIG_InstallConstants(d,swig_const_table);
}

